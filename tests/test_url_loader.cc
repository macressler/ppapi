// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_url_loader.h"

#include <stdio.h>

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_testing.h"
#include "ppapi/c/ppb_url_loader.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/url_loader.h"
#include "ppapi/cpp/url_request_info.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/tests/test_instance.h"

REGISTER_TEST_CASE(URLLoader);

namespace {

const PPB_Testing* g_testing_interface;

class TestCompletionCallback {
 public:
  TestCompletionCallback() : result_(PP_ERROR_WOULDBLOCK) {
  }

  operator pp::CompletionCallback() const {
    return pp::CompletionCallback(&TestCompletionCallback::Handler,
                                  const_cast<TestCompletionCallback*>(this));
  }

  int32_t WaitForResult() {
    result_ = PP_ERROR_WOULDBLOCK;  // Reset
    g_testing_interface->RunMessageLoop();
    return result_;
  }

 private:
  static void Handler(void* user_data, int32_t result) {
    static_cast<TestCompletionCallback*>(user_data)->result_ = result;
    g_testing_interface->QuitMessageLoop();
  }

  int32_t result_;
};

std::string ReportError(const char* method, int32_t error) {
  char error_as_string[12];
  sprintf(error_as_string, "%d", error);
  return method + std::string(" failed with error: ") + error_as_string;
}

}  // namespace

bool TestURLLoader::Init() {
  g_testing_interface = reinterpret_cast<PPB_Testing const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_TESTING_INTERFACE));
  if (!g_testing_interface) {
    // Give a more helpful error message for the testing interface being gone
    // since that needs special enabling in Chrome.
    instance_->AppendError("This test needs the testing interface, which is "
        "not currently available. In Chrome, use --enable-pepper-testing when "
        "launching.");
  }
  return !!g_testing_interface;
}

void TestURLLoader::RunTest() {
  instance_->LogTest("BasicGET", TestBasicGET());
}

std::string TestURLLoader::TestBasicGET() {
  pp::URLRequestInfo request;
  request.SetURL("test_url_loader_data/hello.txt");
  request.SetMethod("GET");

  TestCompletionCallback callback;

  pp::URLLoader loader(*instance_);
  int32_t rv = loader.Open(request, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("URLLoader::Open", rv);

  char buf[256];
  rv = loader.ReadResponseBody(buf, sizeof(buf), callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv < 0)
    return ReportError("URLLoader::ReadResponseBody", rv);
  if (rv == 0)
    return "URLLoader::ReadResponseBody returned end-of-file prematurely.";
  
  char expected[] = "hello\n";
  if (rv != sizeof(expected)-1)
    return "URLLoader::ReadResponseBody returned unexpected content length";
  if (memcmp(expected, buf, rv) != 0)
    return "URLLoader::ReadResponseBody returned unexpected content";

  return "";
}

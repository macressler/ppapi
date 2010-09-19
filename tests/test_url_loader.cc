// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_url_loader.h"

#include <stdio.h>
#include <string.h>

#include "ppapi/c/dev/ppb_file_io_dev.h"
#include "ppapi/c/dev/ppb_testing_dev.h"
#include "ppapi/c/dev/ppb_url_loader_dev.h"
#include "ppapi/c/pp_errors.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/dev/file_io_dev.h"
#include "ppapi/cpp/dev/file_ref_dev.h"
#include "ppapi/cpp/dev/url_loader_dev.h"
#include "ppapi/cpp/dev/url_request_info_dev.h"
#include "ppapi/cpp/dev/url_response_info_dev.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/tests/testing_instance.h"

REGISTER_TEST_CASE(URLLoader);

namespace {

const PPB_Testing_Dev* g_testing_interface;

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
  g_testing_interface = reinterpret_cast<PPB_Testing_Dev const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_TESTING_DEV_INTERFACE));
  if (!g_testing_interface) {
    // Give a more helpful error message for the testing interface being gone
    // since that needs special enabling in Chrome.
    instance_->AppendError("This test needs the testing interface, which is "
        "not currently available. In Chrome, use --enable-pepper-testing when "
        "launching.");
    return false;
  }

  // Make sure we're running over HTTP.
  pp::Var window = instance_->GetWindowObject();
  pp::Var location = window.GetProperty("location");
  pp::Var protocol = location.GetProperty("protocol");
  if (!protocol.is_string() || protocol.AsString() != "http:") {
    instance_->AppendError("This test needs to be run over HTTP.");
    return false;
  }

  return true;
}

void TestURLLoader::RunTest() {
  RUN_TEST(BasicGET);
  RUN_TEST(BasicPOST);
  RUN_TEST(CompoundBodyPOST);
  RUN_TEST(EmptyDataPOST);
  RUN_TEST(CustomRequestHeader);
  RUN_TEST(IgnoresBogusContentLength);
  RUN_TEST(OpenBadFileRef);
  RUN_TEST(SameOriginRestriction);

  // TODO(dumi): Enable this test once we have support for clearing the
  // temporary files created by the stream-to-file option.
#if 0
  RUN_TEST(StreamToFile);
#endif
}

std::string TestURLLoader::ReadEntireFile(pp::FileIO_Dev* file_io,
                                          std::string* data) {
  TestCompletionCallback callback;
  char buf[256];
  int64_t offset = 0;

  for (;;) {
    int32_t rv = file_io->Read(offset, buf, sizeof(buf), callback);
    if (rv == PP_ERROR_WOULDBLOCK)
      rv = callback.WaitForResult();
    if (rv < 0)
      return ReportError("FileIO::Read", rv);
    if (rv == 0)
      break;
    offset += rv;
    data->append(buf, rv);
  }

  return "";
}

std::string TestURLLoader::ReadEntireResponseBody(pp::URLLoader_Dev* loader,
                                                  std::string* body) {
  TestCompletionCallback callback;
  char buf[256];

  for (;;) {
    int32_t rv = loader->ReadResponseBody(buf, sizeof(buf), callback);
    if (rv == PP_ERROR_WOULDBLOCK)
      rv = callback.WaitForResult();
    if (rv < 0)
      return ReportError("URLLoader::ReadResponseBody", rv);
    if (rv == 0)
      break;
    body->append(buf, rv);
  }

  return "";
}

std::string TestURLLoader::LoadAndCompareBody(
    const pp::URLRequestInfo_Dev& request,
    const std::string& expected_body) {
  TestCompletionCallback callback;

  pp::URLLoader_Dev loader(*instance_);
  int32_t rv = loader.Open(request, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("URLLoader::Open", rv);

  pp::URLResponseInfo_Dev response_info(loader.GetResponseInfo());
  if (response_info.is_null())
    return "URLLoader::GetResponseInfo returned null";
  int32_t status_code = response_info.GetStatusCode();
  if (status_code != 200)
    return "Unexpected HTTP status code";

  std::string body;
  std::string error = ReadEntireResponseBody(&loader, &body);
  if (!error.empty())
    return error;

  if (body.size() != expected_body.size())
    return "URLLoader::ReadResponseBody returned unexpected content length";
  if (body != expected_body)
    return "URLLoader::ReadResponseBody returned unexpected content";

  return "";
}

std::string TestURLLoader::TestBasicGET() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("test_url_loader_data/hello.txt");
  return LoadAndCompareBody(request, "hello\n");
}

std::string TestURLLoader::TestBasicPOST() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("/echo");
  request.SetMethod("POST");
  request.AppendDataToBody("postdata");
  return LoadAndCompareBody(request, "postdata");
}

std::string TestURLLoader::TestCompoundBodyPOST() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("/echo");
  request.SetMethod("POST");
  request.AppendDataToBody("post");
  request.AppendDataToBody("data");
  return LoadAndCompareBody(request, "postdata");
}

std::string TestURLLoader::TestEmptyDataPOST() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("/echo");
  request.SetMethod("POST");
  request.AppendDataToBody("");
  return LoadAndCompareBody(request, "");
}

std::string TestURLLoader::TestCustomRequestHeader() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("/echoheader?Foo");
  request.SetHeaders("Foo: 1");
  return LoadAndCompareBody(request, "1");
}

std::string TestURLLoader::TestIgnoresBogusContentLength() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("/echo");
  request.SetMethod("POST");
  request.SetHeaders("Content-Length: 400");
  request.AppendDataToBody("postdata");
  return LoadAndCompareBody(request, "postdata");
}

std::string TestURLLoader::TestOpenBadFileRef() {
  PP_Module module = pp::Module::Get()->pp_module();
  TestCompletionCallback callback;

  // Try opening a file that doesn't exist.
  pp::FileRef_Dev nonexistent_file_ref(
      g_testing_interface->GetNonexistentFileRef(module));
  pp::FileIO_Dev nonexistent_file_reader;
  int32_t rv = nonexistent_file_reader.Open(
      nonexistent_file_ref, PP_FILEOPENFLAG_READ, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_ERROR_NOACCESS)
    return "PP_ERROR_NOACCESS was expected for a nonexistent file";

  // Try opening a file to which we don't have access.
  pp::FileRef_Dev inaccessible_file_ref(
      g_testing_interface->GetInaccessibleFileRef(module));
  pp::FileIO_Dev inaccessible_file_reader;
  rv = inaccessible_file_reader.Open(
      inaccessible_file_ref, PP_FILEOPENFLAG_READ, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_ERROR_NOACCESS)
    return "PP_ERROR_NOACCESS was expected for an inaccessible file.";

  return "";
}

std::string TestURLLoader::TestStreamToFile() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("test_url_loader_data/hello.txt");
  request.SetStreamToFile(true);

  TestCompletionCallback callback;

  pp::URLLoader_Dev loader(*instance_);
  int32_t rv = loader.Open(request, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("URLLoader::Open", rv);

  pp::URLResponseInfo_Dev response_info(loader.GetResponseInfo());
  if (response_info.is_null())
    return "URLLoader::GetResponseInfo returned null";
  int32_t status_code = response_info.GetStatusCode();
  if (status_code != 200)
    return "Unexpected HTTP status code";

  pp::FileRef_Dev body(response_info.GetBody());
  if (body.is_null())
    return "URLResponseInfo::GetBody returned null";

  rv = loader.FinishStreamingToFile(callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("URLLoader::FinishStreamingToFile", rv);


  pp::FileIO_Dev reader;
  rv = reader.Open(body, PP_FILEOPENFLAG_READ, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();
  if (rv != PP_OK)
    return ReportError("FileIO::Open", rv);

  std::string data;
  std::string error = ReadEntireFile(&reader, &data);
  if (!error.empty())
    return error;

  std::string expected_body = "hello\n";
  if (data.size() != expected_body.size())
    return "ReadEntireFile returned unexpected content length";
  if (data != expected_body)
    return "ReadEntireFile returned unexpected content";

  int32_t file_descriptor = reader.GetOSFileDescriptor();
  if (file_descriptor < 0)
    return "FileIO::GetOSFileDescriptor() returned a bad file descriptor.";

  return "";
}

std::string TestURLLoader::TestSameOriginRestriction() {
  pp::URLRequestInfo_Dev request;
  request.SetURL("http://www.google.com/");

  TestCompletionCallback callback;

  pp::URLLoader_Dev loader(*instance_);
  int32_t rv = loader.Open(request, callback);
  if (rv == PP_ERROR_WOULDBLOCK)
    rv = callback.WaitForResult();

  // We expect a failure.
  if (rv != PP_ERROR_NOACCESS) {
    if (rv == PP_OK) {
      return "URLLoader::Open() failed to block a cross-origin request.";
    } else {
      return ReportError("URLLoader::Open()", rv);
    }
  }

  return "";
}

// TODO(darin): Add a test for GrantUniversalAccess.

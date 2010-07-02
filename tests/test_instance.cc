// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_instance.h"

#include <string.h>

#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/tests/test_case.h"

TestCaseFactory* TestCaseFactory::head_ = NULL;

// Returns a new heap-allocated test case for the given test, or NULL on
// failure.
TestInstance::TestInstance(PP_Instance instance)
    : pp::Instance(instance),
      current_case_(NULL),
      executed_tests_(false) {
}

bool TestInstance::Init(uint32_t argc, const char* argn[], const char* argv[]) {
  // Create the proper test case from the argument.
  for (uint32_t i = 0; i < argc; i++) {
    if (strcmp(argn[i], "testcase") == 0) {
      current_case_ = CaseForTestName(argv[i]);
      if (!current_case_)
        errors_.append(std::string("Unknown test case ") + argv[i]);
      else if (!current_case_->Init())
        errors_.append(" Test case could not initialize.");
      return true;
    }
  }

  // Always return true since ViewChanged will actually output the error
  // message to the page (if possible) and to the cookie with what went wrong,
  // and that will only work if the plugin actually runs.
  errors_.append("No TestCase argument given in the page for this plugin");
  return true;
}

void TestInstance::ViewChanged(const PP_Rect& position, const PP_Rect& clip) {
  if (!executed_tests_) {
    executed_tests_ = true;

    // Clear the console.
    // This does: window.document.getElementById("console").innerHTML = "";
    GetWindowObject().GetProperty("document").
        Call("getElementById", "console").SetProperty("innerHTML", "");

    if (!errors_.empty()) {
      // Catch initialization errors and output the current error string to
      // the console.
      AppendHTML("Plugin initialization failed: " + errors_);
    } else {
      current_case_->RunTest();
    }

    // Declare we're done by setting a cookie to either "PASS" or the errors.
    SetCookie("COMPLETION_COOKIE", errors_.empty() ? "PASS" : errors_);
  }
}

void TestInstance::LogTest(const std::string& test_name,
                           const std::string& error_message) {
  std::string html;
  html.append("<div class=\"test_line\"><span class=\"test_name\">");
  html.append(test_name);
  html.append("</span> ");
  if (error_message.empty()) {
    html.append("<span class=\"pass\">PASS</span>");
  } else {
    html.append("<span class=\"fail\">FAIL</span>: <span class=\"err_msg\">");
    html.append(error_message);
    html.append("</span>");

    if (!errors_.empty())
      errors_.append(", ");  // Separator for different error messages.
    errors_.append(test_name + " FAIL: " + error_message);
  }
  html.append("</div>");
  AppendHTML(html);
}

void TestInstance::AppendError(const std::string& message) {
  //AppendHTML(std::string("<div><i>") + message + "</i></div>");
  if (!errors_.empty())
    errors_.append(", ");
  errors_.append(message);
}

TestCase* TestInstance::CaseForTestName(const char* name) {
  TestCaseFactory* iter = TestCaseFactory::head_;
  while (iter != NULL) {
    if (strcmp(name, iter->name_) == 0)
      return iter->method_(this);
    iter = iter->next_;
  }
  return NULL;
}

void TestInstance::AppendHTML(const std::string& html) {
  // This does: window.document.getElementById("console").innerHTML += html
  pp::Var console = GetWindowObject().GetProperty("document").
      Call("getElementById", "console");
  pp::Var inner_html = console.GetProperty("innerHTML");
  console.SetProperty("innerHTML", inner_html.AsString() + html);
}

void TestInstance::SetCookie(const std::string& name,
                             const std::string& value) {
  // window.document.cookie = "<name>=<value>; path=/"
  std::string cookie_string = name + "=" + value + "; path=/";
  pp::Var document = GetWindowObject().GetProperty("document");
  document.SetProperty("cookie", cookie_string);
}

class Module : public pp::Module {
 public:
  Module() : pp::Module() {}
  virtual ~Module() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new TestInstance(instance);
  }
};

namespace pp {

Module* CreateModule() {
  return new ::Module();
}

}  // namespace pp

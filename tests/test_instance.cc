// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_instance.h"

#include "ppapi/cpp/var.h"

TestInstance::TestInstance(PP_Instance instance)
    : pp::Instance(instance),
      executed_tests_(false) {
}

void TestInstance::ViewChanged(const PP_Rect& position, const PP_Rect& clip) {
  if (!executed_tests_) {
    executed_tests_ = true;
    
    // Clear the console.
    // This does: window.document.getElementById("console").innerHTML = "";
    GetWindowObject().GetProperty("document").
        Call("getElementById", "console").SetProperty("innerHTML", "");
    
    RunTest();
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
  }
  html.append("</div>");
  AppendHTML(html);
}

void TestInstance::AppendHTML(const std::string& html) {
  // This does: window.document.getElementById("console").innerHTML += html
  pp::Var console = GetWindowObject().GetProperty("document").
      Call("getElementById", "console");
  pp::Var inner_html = console.GetProperty("innerHTML");
  console.SetProperty("innerHTML", inner_html.AsString() + html);
}

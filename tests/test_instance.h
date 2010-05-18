// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_INSTANCE_H_
#define PPAPI_TEST_TEST_INSTANCE_H_

#include <string>

#include "ppapi/cpp/instance.h"

class TestInstance : public pp::Instance {
 public:
  TestInstance(PP_Instance instance);

  // pp::Instance override.
  virtual void ViewChanged(const PP_Rect& position, const PP_Rect& clip);

  // Override to return the name of your test case.
  virtual std::string GetTestCaseName() const = 0;

  // Override to implement the test. It will be called after the plugin is
  // first displayed.
  virtual void RunTest() = 0;

  // Outputs the information from one test run, using the format
  //   <GetTestCaseName()>.<test_name> [PASS|FAIL <error_message>]
  // If error_message is empty, we say the test passed and emit PASS. If
  // error_message is nonempty, the test failed with that message as the error
  // string.
  //
  // Intended usage:
  //   LogTest("Foo", FooTest());
  //
  // Where FooTest is defined as:
  //   std::string FooTest() {
  //     if (something_horrible_happened)
  //       return "Something horrible happened";
  //     return "";
  //   }
  void LogTest(const std::string& test_name, const std::string& error_message);

 private:
  void AppendHTML(const std::string& html);

  // Set once the tests are run so we know not to re-run when the view is sized.
  bool executed_tests_;
};

#endif  // PPAPI_TEST_TEST_INSTANCE_H_

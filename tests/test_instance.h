// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_INSTANCE_H_
#define PPAPI_TEST_TEST_INSTANCE_H_

#include <string>

#include "ppapi/cpp/instance.h"

class TestCase;

class TestInstance : public pp::Instance {
 public:
  TestInstance(PP_Instance instance);

  // pp::Instance override.
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);
  virtual void ViewChanged(const PP_Rect& position, const PP_Rect& clip);

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
  // Creates a new TestCase for the give test name, or NULL if there is no such
  // test. Ownership is passed to the caller.
  TestCase* CaseForTestName(const char* name);
 
  // Appends the given HTML string to the console in the document.
  void AppendHTML(const std::string& html);

  // Sets the given cookie in the current document.
  void SetCookie(const std::string& name, const std::string& value);

  // Owning pointer to the current test case. Valid after Init has been called.
  TestCase* current_case_;

  // Set once the tests are run so we know not to re-run when the view is sized.
  bool executed_tests_;

  // Collects all errors to send the the browser. Empty indicates no error yet.
  std::string errors_;
};

#endif  // PPAPI_TEST_TEST_INSTANCE_H_

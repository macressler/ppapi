// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_CASE_H_
#define PPAPI_TEST_TEST_CASE_H_

#include <string>

class TestInstance;

// Individual classes of tests derive from this generic test case.
class TestCase {
 public:
  TestCase(TestInstance* instance) : instance_(instance) {}
  virtual ~TestCase() {}

  // Optionally override to do testcase specific initialization.
  virtual bool Init() { return true; }

  // Override to return the name of your test case.
  virtual std::string GetTestCaseName() const = 0;

  // Override to implement the test. It will be called after the plugin is
  // first displayed.
  virtual void RunTest() = 0;

 protected:
  // Pointer to the instance that owns us.
  TestInstance* instance_;
};

#endif  // PPAPI_TEST_TEST_CASE_H_

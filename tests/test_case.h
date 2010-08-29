// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_CASE_H_
#define PPAPI_TEST_TEST_CASE_H_

#include <string>

#include "ppapi/c/pp_resource.h"
#include "ppapi/cpp/dev/scrollbar_dev.h"

class TestInstance;

// Individual classes of tests derive from this generic test case.
class TestCase {
 public:
  TestCase(TestInstance* instance) : instance_(instance) {}
  virtual ~TestCase() {}

  // Optionally override to do testcase specific initialization.
  virtual bool Init() { return true; }

  // Override to implement the test. It will be called after the plugin is
  // first displayed.
  virtual void RunTest() = 0;

  std::string MakeFailureMessage(const char* file, int line, const char* cmd);

  // Override to get scrollbar notifications.
  virtual void ScrollbarValueChanged(pp::Scrollbar_Dev, uint32_t) {}

 protected:
  // Pointer to the instance that owns us.
  TestInstance* instance_;
};

// This class is an implementation detail.
class TestCaseFactory {
 public:
  typedef TestCase* (*Method)(TestInstance* instance);

  TestCaseFactory(const char* name, Method method)
      : next_(head_),
        name_(name),
        method_(method) {
    head_ = this;
  }

 private:
  friend class TestInstance;

  TestCaseFactory* next_;
  const char* name_;
  Method method_;

  static TestCaseFactory* head_;
};

// Use the REGISTER_TEST_CASE macro in your TestCase implementation file to
// register your TestCase.  If your test is named TestFoo, then add the
// following to test_foo.cc:
//
//   REGISTER_TEST_CASE(Foo);
//
// This will cause your test to be included in the set of known tests.
//
#define REGISTER_TEST_CASE(name)                                         \
  static TestCase* Test##name##_FactoryMethod(TestInstance* instance) {  \
    return new Test##name(instance);                                     \
  }                                                                      \
  static TestCaseFactory g_Test##name_factory(                           \
    #name, &Test##name##_FactoryMethod                                   \
  )

// Helper macro for calling functions implementing specific tests in the
// RunTest function. This assumes the function name is TestFoo where Foo is the
// test name,
#define RUN_TEST(name) \
  instance_->LogTest(#name, Test##name());

// Helper macros for checking values in tests, and returning a location
// description of the test fails.
#define ASSERT_TRUE(cmd) \
  if (!(cmd)) { \
    return MakeFailureMessage(__FILE__, __LINE__, #cmd); \
  }
#define ASSERT_FALSE(cmd) ASSERT_TRUE(!(cmd))

#endif  // PPAPI_TEST_TEST_CASE_H_

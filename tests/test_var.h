// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_TEST_TEST_VAR_H_
#define PPAPI_TEST_TEST_VAR_H_

#include <string>

#include "ppapi/tests/test_case.h"

struct PPB_Var;

class TestVar : public TestCase {
 public:
  explicit TestVar(TestInstance* instance) : TestCase(instance) {}

  // TestCase implementation.
  virtual bool Init();
  virtual void RunTest();

 private:
  std::string TestInvalidUtf8();
  std::string TestNullInputInUtf8Conversion();
  std::string TestValidUtf8();
  std::string TestUtf8WithEmbeddedNulls();
  std::string TestVarToUtf8ForWrongType();

  // Used by the tests that access the C API directly.
  const PPB_Var* var_interface_;
};

#endif  // PPAPI_TEST_TEST_VAR_H_

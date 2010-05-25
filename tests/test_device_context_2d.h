// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_DEVICE_CONTEXT_2D_H_
#define PAPPI_TESTS_TEST_DEVICE_CONTEXT_2D_H_

#include "ppapi/tests/test_case.h"

typedef struct _ppb_DeviceContext2D PPB_DeviceContext2D;
typedef struct _ppb_ImageData PPB_ImageData;

class TestDeviceContext2D : public TestCase {
 public:
  TestDeviceContext2D(TestInstance* instance) : TestCase(instance) {}

  // TestCase implementation.
  virtual bool Init();
  virtual std::string GetTestCaseName() const;
  virtual void RunTest();

 private:
  std::string TestInvalidResource();
  std::string TestInvalidSize();
  std::string TestHumongous();
  std::string TestInitToZero();
  std::string TestDescribe();
  std::string TestPaint();
  //std::string TestScroll();  // TODO(brettw) implement.
  std::string TestReplace();

  // Used by the tests that access the C API directly.
  const PPB_DeviceContext2D* device_context_interface_;
  const PPB_ImageData* image_data_interface_;
};

#endif  // PAPPI_TESTS_TEST_DEVICE_CONTEXT_2D_H_

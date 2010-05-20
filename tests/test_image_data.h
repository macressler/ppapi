// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_IMAGE_DATA_H_
#define PAPPI_TESTS_TEST_IMAGE_DATA_H_

#include "ppapi/tests/test_case.h"

typedef struct _ppb_ImageData PPB_ImageData;

class TestImageData : public TestCase {
 public:
  TestImageData(TestInstance* instance) : TestCase(instance) {}

  // TestCase implementation.
  virtual bool Init();
  virtual std::string GetTestCaseName() const;
  virtual void RunTest();

 private:
  std::string TestInvalidFormat();
  std::string TestInvalidSize();
  std::string TestHugeSize();
  std::string TestInitToZero();
  std::string TestIsImageData();

  // Used by the tests that access the C API directly.
  const PPB_ImageData* image_data_interface_;
};

#endif  // PAPPI_TESTS_TEST_IMAGE_DATA_H_

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_URL_LOADER_H_
#define PAPPI_TESTS_TEST_URL_LOADER_H_

#include <string>

#include "ppapi/tests/test_case.h"

typedef struct _ppb_URLLoader PPB_URLLoader;

class TestURLLoader : public TestCase {
 public:
  explicit TestURLLoader(TestInstance* instance) : TestCase(instance) {}

  // TestCase implementation.
  virtual bool Init();
  virtual void RunTest();

 private:
  // Used by the tests that access the C API directly.
  const PPB_URLLoader* url_loader_interface_;
};

#endif  // PAPPI_TESTS_TEST_URL_LOADER_H_

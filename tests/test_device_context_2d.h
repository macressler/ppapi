// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PAPPI_TESTS_TEST_DEVICE_CONTEXT_2D_H_
#define PAPPI_TESTS_TEST_DEVICE_CONTEXT_2D_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/tests/test_case.h"

typedef struct _ppb_DeviceContext2D PPB_DeviceContext2D;
typedef struct _ppb_ImageData PPB_ImageData;
typedef struct _ppb_Testing PPB_Testing;

namespace pp {
class DeviceContext2D;
class ImageData;
class Point;
class Rect;
}

class TestDeviceContext2D : public TestCase {
 public:
  TestDeviceContext2D(TestInstance* instance) : TestCase(instance) {}

  // TestCase implementation.
  virtual bool Init();
  virtual void RunTest();

  void QuitMessageLoop();

 private:
  bool ReadImageData(const pp::DeviceContext2D& dc,
                     pp::ImageData* image,
                     const pp::Point& top_left) const;

  void FillRectInImage(pp::ImageData* image,
                       const pp::Rect& rect,
                       uint32_t color) const;

  // Validates that the given image is a single color with a square of another
  // color inside it.
  bool IsSquareInImage(const pp::ImageData& image_data,
                       uint32_t background_color,
                       const pp::Rect& square, uint32_t square_color) const;

  // Validates that the given device context is a single color with a square of
  // another color inside it.
  bool IsSquareInDC(const pp::DeviceContext2D& dc, uint32_t background_color,
                    const pp::Rect& square, uint32_t square_color) const;

  // Validates that the given device context is filled with the given color.
  bool IsDCUniformColor(const pp::DeviceContext2D& dc, uint32_t color) const;

  // Issues a flush on the given device context and blocks until the flush
  // has issued its callback. Returns true on success.
  bool FlushAndWaitForDone(pp::DeviceContext2D* context);

  std::string TestInvalidResource();
  std::string TestInvalidSize();
  std::string TestHumongous();
  std::string TestInitToZero();
  std::string TestDescribe();
  std::string TestPaint();
  //std::string TestScroll();  // TODO(brettw) implement.
  std::string TestReplace();
  std::string TestFlush();

  // Used by the tests that access the C API directly.
  const PPB_DeviceContext2D* device_context_interface_;
  const PPB_ImageData* image_data_interface_;
  const PPB_Testing* testing_interface_;
};

#endif  // PAPPI_TESTS_TEST_DEVICE_CONTEXT_2D_H_

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_device_context_2d.h"

#include <string.h>

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_testing.h"
#include "ppapi/c/ppb_device_context_2d.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/tests/test_instance.h"

REGISTER_TEST_CASE(DeviceContext2D);

namespace {

// A NOP flush callback for use in various tests.
void FlushCallbackNOP(void* data, int32_t result) {
}

void FlushCallbackQuitMessageLoop(void* data, int32_t result) {
  reinterpret_cast<TestDeviceContext2D*>(data)->QuitMessageLoop();
}

}  // namespace

bool TestDeviceContext2D::Init() {
  device_context_interface_ = reinterpret_cast<PPB_DeviceContext2D const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_DEVICECONTEXT2D_INTERFACE));
  image_data_interface_ = reinterpret_cast<PPB_ImageData const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_IMAGEDATA_INTERFACE));
  testing_interface_ = reinterpret_cast<PPB_Testing const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_TESTING_INTERFACE));
  if (!testing_interface_) {
    // Give a more helpful error message for the testing interface being gone
    // since that needs special enabling in Chrome.
    instance_->AppendError("This test needs the testing interface, which is "
        "not currently available. In Chrome, use --enable-pepper-testing when "
        "launching.");
  }
  return device_context_interface_ && image_data_interface_ &&
         testing_interface_;
}

void TestDeviceContext2D::RunTest() {
  instance_->LogTest("InvalidResource", TestInvalidResource());
  instance_->LogTest("InvalidSize", TestInvalidSize());
  instance_->LogTest("Humongous", TestHumongous());
  instance_->LogTest("InitToZero", TestInitToZero());
  instance_->LogTest("Describe", TestDescribe());
  instance_->LogTest("Paint", TestPaint());
  //instance_->LogTest("Scroll", TestScroll());  // TODO(brettw) implement.
  instance_->LogTest("Replace", TestReplace());
  instance_->LogTest("Flush", TestFlush());
}

void TestDeviceContext2D::QuitMessageLoop() {
  testing_interface_->QuitMessageLoop();
}

bool TestDeviceContext2D::ReadImageData(const pp::DeviceContext2D& dc,
                                        pp::ImageData* image,
                                        const pp::Point& top_left) const {
  return testing_interface_->ReadImageData(dc.pp_resource(),
                                           image->pp_resource(),
                                           &top_left.pp_point());
}

bool TestDeviceContext2D::IsDCUniformColor(const pp::DeviceContext2D& dc,
                                           uint32_t color) const {
  pp::ImageData readback(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                         dc.size(), false);
  if (readback.is_null())
    return false;
  if (!ReadImageData(dc, &readback, pp::Point(0, 0)))
    return false;
  return IsSquareInImage(readback, 0, pp::Rect(dc.size()), color);
}

bool TestDeviceContext2D::FlushAndWaitForDone(pp::DeviceContext2D* context) {
  pp::CompletionCallback cc(&FlushCallbackQuitMessageLoop, this);
  int32_t rv = context->Flush(cc);
  if (rv == PP_OK)
    return true;
  if (rv != PP_ERROR_WOULDBLOCK)
    return false;
  testing_interface_->RunMessageLoop();
  return true;
}

void TestDeviceContext2D::FillRectInImage(pp::ImageData* image,
                                          const pp::Rect& rect,
                                          uint32_t color) const {
  for (int y = rect.y(); y < rect.bottom(); y++) {
    uint32_t* row = image->GetAddr32(pp::Point(rect.x(), y));
    for (int pixel = 0; pixel < rect.width(); pixel++)
      row[pixel] = color;
  }
}

bool TestDeviceContext2D::IsSquareInImage(const pp::ImageData& image_data,
                                          uint32_t background_color,
                                          const pp::Rect& square,
                                          uint32_t square_color) const {
  for (int y = 0; y < image_data.size().height(); y++) {
    for (int x = 0; x < image_data.size().width(); x++) {
      uint32_t pixel = *image_data.GetAddr32(pp::Point(x, y));
      uint32_t desired_color;
      if (square.Contains(x, y))
        desired_color = square_color;
      else
        desired_color = background_color;
      if (pixel != desired_color)
        return false;
    }
  }
  return true;
}

bool TestDeviceContext2D::IsSquareInDC(const pp::DeviceContext2D& dc,
                                       uint32_t background_color,
                                       const pp::Rect& square,
                                       uint32_t square_color) const {
  pp::ImageData readback(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                         dc.size(), false);
  if (readback.is_null())
    return false;
  if (!ReadImageData(dc, &readback, pp::Point(0, 0)))
    return false;
  return IsSquareInImage(readback, background_color, square, square_color);
}

// Test all the functions with an invalid handle.
std::string TestDeviceContext2D::TestInvalidResource() {
  pp::DeviceContext2D null_context;
  pp::ImageData image(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(16, 16), true);

  // Describe.
  PP_Size size;
  bool opaque;
  if (device_context_interface_->Describe(image.pp_resource(),
                                          &size, &opaque))
    return "Describe succeeded with a different resource";
  if (device_context_interface_->Describe(null_context.pp_resource(),
                                          &size, &opaque))
    return "Describe succeeded with a NULL resource";

  // PaintImageData.
  PP_Point zero_zero;
  zero_zero.x = 0;
  zero_zero.y = 0;
  if (device_context_interface_->PaintImageData(image.pp_resource(),
                                                image.pp_resource(),
                                                &zero_zero, NULL))
    return "PaintImageData succeeded with a different resource";
  if (device_context_interface_->PaintImageData(null_context.pp_resource(),
                                                image.pp_resource(),
                                                &zero_zero, NULL))
    return "PaintImageData succeeded with a NULL resource";

  // Scroll.
  PP_Point zero_ten;
  zero_ten.x = 0;
  zero_ten.y = 10;
  if (device_context_interface_->Scroll(image.pp_resource(), NULL, &zero_ten))
    return "Scroll succeeded with a different resource";
  if (device_context_interface_->Scroll(null_context.pp_resource(),
                                        NULL, &zero_ten))
    return "Scroll succeeded with a NULL resource";

  // ReplaceContents.
  if (device_context_interface_->ReplaceContents(image.pp_resource(),
                                                 image.pp_resource()))
    return "ReplaceContents succeeded with a different resource";
  if (device_context_interface_->ReplaceContents(null_context.pp_resource(),
                                                 image.pp_resource()))
    return "ReplaceContents succeeded with a NULL resource";

  // Flush.
  if (device_context_interface_->Flush(
          image.pp_resource(),
          PP_MakeCompletionCallback(&FlushCallbackNOP, NULL)) == PP_OK)
    return "Flush succeeded with a different resource";
  if (device_context_interface_->Flush(
          null_context.pp_resource(),
          PP_MakeCompletionCallback(&FlushCallbackNOP, NULL)) == PP_OK)
    return "Flush succeeded with a NULL resource";

  // ReadImageData.
  if (testing_interface_->ReadImageData(image.pp_resource(),
                                        image.pp_resource(),
                                        &zero_zero))
    return "ReadImageData succeeded with a different resource";
  if (testing_interface_->ReadImageData(null_context.pp_resource(),
                                        image.pp_resource(),
                                        &zero_zero))
    return "ReadImageData succeeded with a NULL resource";

  return "";
}

std::string TestDeviceContext2D::TestInvalidSize() {
  pp::DeviceContext2D a(pp::Size(16, 0), false);
  if (!a.is_null())
    return "0 height accepted";

  pp::DeviceContext2D b(pp::Size(0, 16), false);
  if (!b.is_null())
    return "0 width accepted";

  // Need to use the C API since pp::Size prevents negative sizes.
  PP_Size size;
  size.width = 16;
  size.height = -16;
  ASSERT_FALSE(!!device_context_interface_->Create(
      pp::Module::Get()->pp_module(), &size, false));

  size.width = -16;
  size.height = 16;
  ASSERT_FALSE(!!device_context_interface_->Create(
      pp::Module::Get()->pp_module(), &size, false));

  return "";
}

std::string TestDeviceContext2D::TestHumongous() {
  pp::DeviceContext2D a(pp::Size(100000, 100000), false);
  if (!a.is_null())
    return "Humongous device created";
  return "";
}

std::string TestDeviceContext2D::TestInitToZero() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(pp::Size(w, h), false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Make an image with nonzero data in it (so we can test that zeros were
  // actually read versus ReadImageData being a NOP).
  pp::ImageData image(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(w, h), true);
  if (image.is_null())
    return "Failure to allocate an image";
  memset(image.data(), 0xFF, image.stride() * image.size().height() * 4);

  // Read out the initial data from the device & check.
  if (!ReadImageData(dc, &image, pp::Point(0, 0)))
    return "Couldn't read image data";
  if (!IsSquareInImage(image, 0, pp::Rect(0, 0, w, h), 0))
    return "Got a nonzero pixel";

  return "";
}

std::string TestDeviceContext2D::TestDescribe() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(pp::Size(w, h), false);
  if (dc.is_null())
    return "Failure creating a boring device";

  PP_Size size;
  size.width = -1;
  size.height = -1;
  bool is_always_opaque = true;
  if (!device_context_interface_->Describe(dc.pp_resource(), &size,
                                           &is_always_opaque))
    return "Describe failed";
  if (size.width != w || size.height != h || is_always_opaque != false)
    return "Mismatch of data.";

  return "";
}

std::string TestDeviceContext2D::TestPaint() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(pp::Size(w, h), false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Make sure the device background is 0.
  if (!IsDCUniformColor(dc, 0))
    return "Bad initial color";

  // Fill the backing store with white.
  const uint32_t background_color = 0xFFFFFFFF;
  pp::ImageData background(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(w, h),
                           false);
  FillRectInImage(&background, pp::Rect(0, 0, w, h), background_color);
  if (!dc.PaintImageData(background, pp::Point(0, 0)))
    return "Couldn't fill background";
  if (!FlushAndWaitForDone(&dc))
    return "Couldn't flush to fill backing store";

  // Try painting where the dirty rect is outside of the bitmap bounds, this
  // should fail.
  const int fill_w = 2, fill_h = 3;
  pp::ImageData invalid_clip(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                             pp::Size(fill_w, fill_h), false);
  if (invalid_clip.is_null())
    return "Failure to allocate invalid_clip image";
  if (dc.PaintImageData(invalid_clip, pp::Point(0, 0),
                        pp::Rect(-1, 0, fill_w, fill_h)))
    return "Accepted a negative dirty rect";
  if (dc.PaintImageData(invalid_clip, pp::Point(0, 0),
                        pp::Rect(0, 0, fill_w, fill_h + 1)))
    return "Accepted a too-big dirty rect";

  // Make an image to paint with that's opaque white and enqueue a paint.
  pp::ImageData fill(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(fill_w, fill_h),
                     true);
  if (fill.is_null())
    return "Failure to allocate fill image";
  FillRectInImage(&fill, pp::Rect(fill.size()), background_color);
  const int paint_x = 4, paint_y = 5;
  if (!dc.PaintImageData(fill, pp::Point(paint_x, paint_y)))
    return "Couldn't paint the rect.";

  // Validate that nothing has been actually painted.
  if (!IsDCUniformColor(dc, background_color))
    return "Image updated before flush (or failure in readback).";

  // The paint hasn't been flushed so we can still change the bitmap. Fill with
  // 50% blue. This will also verify that the backing store is replaced
  // with the contents rather than blended.
  const uint32_t fill_color = 0x80000080;
  FillRectInImage(&fill, pp::Rect(fill.size()), fill_color);
  if (!FlushAndWaitForDone(&dc))
    return "Couldn't flush 50% blue paint";

  if (!IsSquareInDC(dc, background_color,
                    pp::Rect(paint_x, paint_y, fill_w, fill_h),
                    fill_color))
    return "Image not painted properly.";

  // Reset the DC to blank white & paint our image slightly off the buffer.
  // This should succeed. We also try painting the same thing where the
  // dirty rect falls outeside of the device, which should fail.
  if (!dc.PaintImageData(background, pp::Point(0, 0)))
    return "Couldn't fill background";
  const int second_paint_x = -1, second_paint_y = -2;
  if (dc.PaintImageData(fill, pp::Point(second_paint_x, second_paint_y)))
    return "Trying to paint outside of the image.";
  if (!dc.PaintImageData(fill, pp::Point(second_paint_x, second_paint_y),
                         pp::Rect(-second_paint_x, -second_paint_y, 1, 1)))
    return "Painting failed.";
  if (!FlushAndWaitForDone(&dc))
    return "Couldn't flush second paint";

  // Now we should have a little bit of the image peeking out the top left.
  if (!IsSquareInDC(dc, background_color, pp::Rect(0, 0, 1, 1),
                    fill_color))
    return "Partially offscreen paint failed.";

  // Now repaint that top left pixel by doing a subset of the source image.
  pp::ImageData subset(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(w, h), false);
  uint32_t subset_color = 0x80808080;
  const int subset_x = 2, subset_y = 1;
  *subset.GetAddr32(pp::Point(subset_x, subset_y)) = subset_color;
  if (!dc.PaintImageData(subset, pp::Point(-subset_x, -subset_y),
                         pp::Rect(subset_x, subset_y, 1, 1)))
    return "Couldn't paint the subset.";
  if (!FlushAndWaitForDone(&dc))
    return "Couldn't flush repaint";
  if (!IsSquareInDC(dc, background_color, pp::Rect(0, 0, 1, 1),
                    subset_color))
    return "Subset paint failed.";

  return "";
}

std::string TestDeviceContext2D::TestReplace() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(pp::Size(w, h), false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Replacing with a different size image should fail.
  pp::ImageData weird_size(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                           pp::Size(w - 1, h), true);
  if (weird_size.is_null())
    return "Failure allocating the weird sized image";
  if (dc.ReplaceContents(&weird_size))
    return "Could replace a DC with a different size background";

  // Fill the background with blue but don't flush yet.
  const int32_t background_color = 0xFF0000FF;
  pp::ImageData background(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(w, h),
                           true);
  if (background.is_null())
    return "Failure to allocate background image";
  FillRectInImage(&background, pp::Rect(0, 0, w, h), background_color);
  if (!dc.PaintImageData(background, pp::Point(0, 0)))
    return "Couldn't paint the background.";

  // Replace with a green background but don't flush yet.
  const int32_t swapped_color = 0xFF0000FF;
  pp::ImageData swapped(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(w, h), true);
  if (swapped.is_null())
    return "Failure to allocate swapped image";
  FillRectInImage(&swapped, pp::Rect(0, 0, w, h), swapped_color);
  if (!dc.ReplaceContents(&swapped))
    return "Couldn't replace.";

  // The background should be unchanged since we didn't flush yet.
  if (!IsDCUniformColor(dc, 0))
    return "Image updated before flush (or failure in readback).";

  // Test the C++ wrapper. The size of the swapped image should be reset.
  if (swapped.pp_resource() || swapped.size().width() ||
      swapped.size().height() || swapped.data())
    return "Size of the swapped image should be reset.";

  // Painting with the swapped image should fail.
  if (dc.PaintImageData(swapped, pp::Point(0, 0)))
    return "Painting with the swapped image should fail.";

  // Flush and make sure the result is correct.
  if (!FlushAndWaitForDone(&dc))
    return "Couldn't flush";

  // The background should be green from the swapped image.
  if (!IsDCUniformColor(dc, swapped_color))
    return "Flushed color incorrect (or failure in readback).";

  return "";
}

std::string TestDeviceContext2D::TestFlush() {
  // Tests that synchronous flushes (NULL callback) fail on the main thread
  // (which is the current one).
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(pp::Size(w, h), false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Fill the background with blue but don't flush yet.
  pp::ImageData background(PP_IMAGEDATAFORMAT_BGRA_PREMUL, pp::Size(w, h),
                           true);
  if (background.is_null())
    return "Failure to allocate background image";
  if (!dc.PaintImageData(background, pp::Point(0, 0)))
    return "Couldn't paint the background.";

  int32_t rv = dc.Flush(pp::CompletionCallback::Block());
  if (rv == PP_OK || rv == PP_ERROR_WOULDBLOCK)
    return "Flush succeeded from the main thread with no callback.";

  // Test flushing with no operations still issues a callback.
  // (This may also hang if the browser never issues the callback).
  pp::DeviceContext2D dc_nopaints(pp::Size(w, h), false);
  if (dc.is_null())
    return "Failure creating the nopaint device";
  if (!FlushAndWaitForDone(&dc_nopaints))
    return "Couldn't flush the nopaint device";

  // Test that multiple flushes fail if we don't get a callback in between.
  rv = dc_nopaints.Flush(pp::CompletionCallback(&FlushCallbackNOP, NULL));
  if (rv != PP_OK && rv != PP_ERROR_WOULDBLOCK)
    return "Couldn't flush first time for multiple flush test.";

  if (rv != PP_OK) {
    // If the first flush would block, then a second should fail.
    rv = dc_nopaints.Flush(pp::CompletionCallback(&FlushCallbackNOP, NULL));
    if (rv == PP_OK || rv == PP_ERROR_WOULDBLOCK)
      return "Second flush succeeded before callback ran.";
  }

  return "";
}

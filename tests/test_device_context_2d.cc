// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_device_context_2d.h"

#include <string.h>

#include "ppapi/c/pp_rect.h"
#include "ppapi/c/ppb_testing.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/tests/test_instance.h"

namespace {

// A NOP flush callback for use in various tests.
void FlushCallbackNOP(PP_Resource context, void* data) {
}

}  // namespace

bool TestDeviceContext2D::Init() {
  device_context_interface_ = reinterpret_cast<PPB_DeviceContext2D const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_DEVICECONTEXT2D_INTERFACE));
  image_data_interface_ = reinterpret_cast<PPB_ImageData const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_IMAGEDATA_INTERFACE));
  testing_interface_ = reinterpret_cast<PPB_Testing const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_TESTING_INTERFACE));
  return device_context_interface_ && image_data_interface_ &&
         testing_interface_;
}

std::string TestDeviceContext2D::GetTestCaseName() const {
  return std::string("DeviceContext");
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
  instance_->LogTest("FlushSyncOnMainThread", TestFlushSyncOnMainThread());
}

bool TestDeviceContext2D::ReadImageData(const pp::DeviceContext2D& dc,
                                        pp::ImageData* image,
                                        int32_t x, int32_t y) const {
  return testing_interface_->ReadImageData(dc.pp_resource(),
                                           image->pp_resource(), x, y);
}

bool TestDeviceContext2D::IsDCUniformColor(const pp::DeviceContext2D& dc,
                                           uint32_t color) const {
  pp::ImageData readback(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                         dc.width(), dc.height(), false);
  if (readback.is_null())
    return false;
  if (!ReadImageData(dc, &readback, 0, 0))
    return false;
  return IsSquareInImage(readback, 0,
                         PP_MakeRectFromXYWH(0, 0, dc.width(), dc.height()),
                         color);
}

void TestDeviceContext2D::FillRectInImage(pp::ImageData* image,
                                          const PP_Rect& rect,
                                          uint32_t color) const {
  for (int y = rect.point.y; y < rect.point.y + rect.size.height; y++) {
    uint32_t* row = image->GetAddr32(rect.point.x, y);
    for (int pixel = 0; pixel < rect.size.width; pixel++)
      row[pixel] = color;
  }
}

bool TestDeviceContext2D::IsSquareInImage(const pp::ImageData& image_data,
                                          uint32_t background_color,
                                          const PP_Rect& square,
                                          uint32_t square_color) const {
  for (int y = 0; y < image_data.height(); y++) {
    for (int x = 0; x < image_data.width(); x++) {
      uint32_t pixel = *image_data.GetAddr32(x, y);
      uint32_t desired_color;
      if (x >= square.point.x && x < square.point.x + square.size.width &&
          y >= square.point.y && y < square.point.y + square.size.height)
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
                                       const PP_Rect& square,
                                       uint32_t square_color) const {
  pp::ImageData readback(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                         dc.width(), dc.height(), false);
  if (readback.is_null())
    return false;
  if (!ReadImageData(dc, &readback, 0, 0))
    return false;
  return IsSquareInImage(readback, background_color, square, square_color);
}

// Test all the functions with an invalid handle.
std::string TestDeviceContext2D::TestInvalidResource() {
  pp::DeviceContext2D null_context;
  pp::ImageData image(PP_IMAGEDATAFORMAT_BGRA_PREMUL, 16, 16, true);

  // Describe.
  int width, height;
  bool opaque;
  if (device_context_interface_->Describe(image.pp_resource(),
                                          &width, &height, &opaque))
    return "Describe succeeded with a different resource";
  if (device_context_interface_->Describe(null_context.pp_resource(),
                                          &width, &height, &opaque))
    return "Describe succeeded with a NULL resource";

  // PaintImageData.
  if (device_context_interface_->PaintImageData(image.pp_resource(),
                                                image.pp_resource(),
                                                0, 0, NULL))
    return "PaintImageData succeeded with a different resource";
  if (device_context_interface_->PaintImageData(null_context.pp_resource(),
                                                image.pp_resource(),
                                                0, 0, NULL))
    return "PaintImageData succeeded with a NULL resource";

  // Scroll.
  if (device_context_interface_->Scroll(image.pp_resource(), NULL, 0, 10))
    return "Scroll succeeded with a different resource";
  if (device_context_interface_->Scroll(null_context.pp_resource(),
                                        NULL, 0, 10))
    return "Scroll succeeded with a NULL resource";

  // ReplaceContents.
  if (device_context_interface_->ReplaceContents(image.pp_resource(),
                                                 image.pp_resource()))
    return "ReplaceContents succeeded with a different resource";
  if (device_context_interface_->ReplaceContents(null_context.pp_resource(),
                                                 image.pp_resource()))
    return "ReplaceContents succeeded with a NULL resource";

  // Flush.
  if (device_context_interface_->Flush(image.pp_resource(),
                                       &FlushCallbackNOP, NULL))
    return "Flush succeeded with a different resource";
  if (device_context_interface_->Flush(null_context.pp_resource(),
                                       &FlushCallbackNOP, NULL))
    return "Flush succeeded with a NULL resource";

  // ReadImageData.
  if (testing_interface_->ReadImageData(image.pp_resource(),
                                               image.pp_resource(), 0, 0))
    return "ReadImageData succeeded with a different resource";
  if (testing_interface_->ReadImageData(null_context.pp_resource(),
                                               image.pp_resource(), 0, 0))
    return "ReadImageData succeeded with a NULL resource";

  return "";
}

std::string TestDeviceContext2D::TestInvalidSize() {
  pp::DeviceContext2D a(16, 0, false);
  if (!a.is_null())
    return "0 height accepted";

  pp::DeviceContext2D b(0, 16, false);
  if (!b.is_null())
    return "0 width accepted";

  pp::DeviceContext2D c(16, -16, false);
  if (!c.is_null())
    return "negative height accepted";

  pp::DeviceContext2D d(-16, 16, false);
  if (!d.is_null())
    return "negative width accepted";

  return "";
}

std::string TestDeviceContext2D::TestHumongous() {
  pp::DeviceContext2D a(100000, 100000, false);
  if (!a.is_null())
    return "Humongous device created";
  return "";
}

std::string TestDeviceContext2D::TestInitToZero() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(w, h, false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Make an image with nonzero data in it (so we can test that zeros were
  // actually read versus ReadImageData being a NOP).
  pp::ImageData image(PP_IMAGEDATAFORMAT_BGRA_PREMUL, w, h, true);
  if (image.is_null())
    return "Failure to allocate an image";
  memset(image.data(), 0xFF, image.stride() * image.height() * 4);

  // Read out the initial data from the device & check.
  if (!ReadImageData(dc, &image, 0, 0))
    return "Couldn't read image data";
  if (!IsSquareInImage(image, 0, PP_MakeRectFromXYWH(0, 0, w, h), 0))
    return "Got a nonzero pixel";

  return "";
}

std::string TestDeviceContext2D::TestDescribe() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(w, h, false);
  if (dc.is_null())
    return "Failure creating a boring device";

  int out_w = -1, out_h = -1;
  bool is_always_opaque = true;
  if (!device_context_interface_->Describe(dc.pp_resource(), &out_w, &out_h,
                                           &is_always_opaque))
    return "Describe failed";
  if (out_w != w || out_h != h || is_always_opaque != false)
    return "Mismatch of data.";

  return "";
}

std::string TestDeviceContext2D::TestPaint() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(w, h, false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Make sure the device background is 0.
  if (!IsDCUniformColor(dc, 0))
    return "Bad initial color";

  // Fill the backing store with white.
  const uint32_t background_color = 0xFFFFFFFF;
  pp::ImageData background(PP_IMAGEDATAFORMAT_BGRA_PREMUL, w, h, false);
  FillRectInImage(&background, PP_MakeRectFromXYWH(0, 0, w, h),
                  background_color);
  if (!dc.PaintImageData(background, 0, 0, NULL))
    return "Couldn't fill background";
  if (!dc.Flush(&FlushCallbackNOP, NULL)) {
    // NOTE: Technically this should fail because Flush should prohibit
    // blocking on the main thread. When we implement that, this will have to
    // be refactored to run asynchronously.
    return "Couldn't flush (this will fail when we implement Flush properly)";
  }

  // Try painting where the dirty rect is outside of the bitmap bounds, this
  // should fail.
  const int fill_w = 2, fill_h = 3;
  pp::ImageData invalid_clip(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                             fill_w, fill_h, false);
  if (invalid_clip.is_null())
    return "Failure to allocate invalid_clip image";
  if (dc.PaintImageData(invalid_clip, 0, 0,
                        &PP_MakeRectFromXYWH(-1, 0, fill_w, fill_h)))
    return "Accepted a negative dirty rect";
  if (dc.PaintImageData(invalid_clip, 0, 0,
                        &PP_MakeRectFromXYWH(0, 0, fill_w, fill_h + 1)))
    return "Accepted a too-big dirty rect";

  // Make an image to paint with that's opaque white and enqueue a paint.
  pp::ImageData fill(PP_IMAGEDATAFORMAT_BGRA_PREMUL, fill_w, fill_h, true);
  if (fill.is_null())
    return "Failure to allocate fill image";
  FillRectInImage(&fill, PP_MakeRectFromXYWH(0, 0, fill_w, fill_h),
                  background_color);
  const int paint_x = 4, paint_y = 5;
  if (!dc.PaintImageData(fill, paint_x, paint_y, NULL))
    return "Couldn't paint the rect.";

  // Validate that nothing has been actually painted.
  if (!IsDCUniformColor(dc, background_color))
    return "Image updated before flush (or failure in readback).";

  // The paint hasn't been flushed so we can still change the bitmap. Fill with
  // 50% blue. This will also verify that the backing store is replaced
  // with the contents rather than blended.
  const uint32_t fill_color = 0x80000080;
  FillRectInImage(&fill, PP_MakeRectFromXYWH(0, 0, fill_w, fill_h),
                  fill_color);
  if (!dc.Flush(&FlushCallbackNOP, NULL)) {
    // See comment above for Flush().
    return "Couldn't flush (this will fail when we implement Flush properly)";
  }

  if (!IsSquareInDC(dc, background_color,
                    PP_MakeRectFromXYWH(paint_x, paint_y, fill_w, fill_h),
                    fill_color))
    return "Image not painted properly.";

  // Reset the DC to blank white & paint our image slightly off the buffer.
  // This should succeed. We also try painting the same thing where the
  // dirty rect falls outeside of the device, which should fail.
  if (!dc.PaintImageData(background, 0, 0, NULL))
    return "Couldn't fill background";
  const int second_paint_x = -1, second_paint_y = -2;
  if (dc.PaintImageData(fill, second_paint_x, second_paint_y, NULL))
    return "Trying to paint outside of the image.";
  if (!dc.PaintImageData(fill, second_paint_x, second_paint_y,
                         &PP_MakeRectFromXYWH(-second_paint_x, -second_paint_y,
                                              1, 1)))
    return "Painting failed.";
  if (!dc.Flush(&FlushCallbackNOP, NULL)) {
    // See comment above for Flush().
    return "Couldn't flush (this will fail when we implement Flush properly)";
  }

  // Now we should have a little bit of the image peeking out the top left.
  if (!IsSquareInDC(dc, background_color, PP_MakeRectFromXYWH(0, 0, 1, 1),
                    fill_color))
    return "Partially offscreen paint failed.";

  // Now repaint that top left pixel by doing a subset of the source image.
  pp::ImageData subset(PP_IMAGEDATAFORMAT_BGRA_PREMUL, w, h, false);
  uint32_t subset_color = 0x80808080;
  const int subset_x = 2, subset_y = 1;
  *subset.GetAddr32(subset_x, subset_y) = subset_color;
  if (!dc.PaintImageData(subset, -subset_x, -subset_y,
                         &PP_MakeRectFromXYWH(subset_x, subset_y, 1, 1)))
    return "Couldn't paint the subset.";
  if (!dc.Flush(&FlushCallbackNOP, NULL))
    return "Couldn't flush";
  if (!IsSquareInDC(dc, background_color, PP_MakeRectFromXYWH(0, 0, 1, 1),
                    subset_color))
    return "Subset paint failed.";

  return "";
}

std::string TestDeviceContext2D::TestReplace() {
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(w, h, false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Replacing with a different size image should fail.
  pp::ImageData weird_size(PP_IMAGEDATAFORMAT_BGRA_PREMUL, w - 1, h, true);
  if (weird_size.is_null())
    return "Failure allocating the weird sized image";
  if (dc.ReplaceContents(&weird_size))
    return "Could replace a DC with a different size background";

  // Fill the background with blue but don't flush yet.
  const int32_t background_color = 0xFF0000FF;
  pp::ImageData background(PP_IMAGEDATAFORMAT_BGRA_PREMUL, w, h, true);
  if (background.is_null())
    return "Failure to allocate background image";
  FillRectInImage(&background, PP_MakeRectFromXYWH(0, 0, w, h),
                  background_color);
  if (!dc.PaintImageData(background, 0, 0, NULL))
    return "Couldn't paint the background.";

  // Replace with a green background but don't flush yet.
  const int32_t swapped_color = 0xFF0000FF;
  pp::ImageData swapped(PP_IMAGEDATAFORMAT_BGRA_PREMUL, w, h, true);
  if (swapped.is_null())
    return "Failure to allocate swapped image";
  FillRectInImage(&swapped, PP_MakeRectFromXYWH(0, 0, w, h), swapped_color);
  if (!dc.ReplaceContents(&swapped))
    return "Couldn't replace.";

  // The background should be unchanged since we didn't flush yet.
  if (!IsDCUniformColor(dc, 0))
    return "Image updated before flush (or failure in readback).";

  // Test the C++ wrapper. The size of the swapped image should be reset.
  if (swapped.pp_resource() || swapped.width() || swapped.height() ||
      swapped.data())
    return "Size of the swapped image should be reset.";

  // Painting with the swapped image should fail.
  if (dc.PaintImageData(swapped, 0, 0, NULL))
    return "Painting with the swapped image should fail.";

  // Flush and make sure the result is correct.
  if (!dc.Flush(&FlushCallbackNOP, NULL)) {
    // NOTE: Technically this should fail because Flush should prohibit
    // blocking on the main thread. When we implement that, this will have to
    // be refactored to run asynchronously.
    return "Couldn't flush (this will fail when we implement Flush properly)";
  }

  // The background should be green from the swapped image.
  if (!IsDCUniformColor(dc, swapped_color))
    return "Flushed color incorrect (or failure in readback).";

  return "";
}

std::string TestDeviceContext2D::TestFlushSyncOnMainThread() {
  // Tests that synchronous flushes (NULL callback) fail on the main thread
  // (which is the current one).
  const int w = 15, h = 17;
  pp::DeviceContext2D dc(w, h, false);
  if (dc.is_null())
    return "Failure creating a boring device";

  // Fill the background with blue but don't flush yet.
  const int32_t background_color = 0xFF0000FF;
  pp::ImageData background(PP_IMAGEDATAFORMAT_BGRA_PREMUL, w, h, true);
  if (background.is_null())
    return "Failure to allocate background image";
  if (!dc.PaintImageData(background, 0, 0, NULL))
    return "Couldn't paint the background.";

  if (dc.Flush(NULL, NULL))
    return "Flush succeeded from the main thread with no callback.";
  return "";
}

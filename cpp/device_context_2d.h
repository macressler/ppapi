// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEVICE_CONTEXT_2D_H_
#define PPAPI_CPP_DEVICE_CONTEXT_2D_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_device_context_2d.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class ImageData;
class Rect;

class DeviceContext2D : public Resource {
 public:
  // Creates an is_null() ImageData object.
  DeviceContext2D();

  // The copied device will refer to the original (since this is just a wrapper
  // around a refcounted resource).
  DeviceContext2D(const DeviceContext2D& other);

  // Allocates a new 2D device context with the given size in the browser,
  // resulting object will be is_null() if the allocation failed.
  DeviceContext2D(int32_t width, int32_t height);

  virtual ~DeviceContext2D();

  DeviceContext2D& operator=(const DeviceContext2D& other);
  void swap(DeviceContext2D& other);

  void PaintImageData(const ImageData& image,
                      int32_t x, int32_t y,
                      const Rect& dirty,
                      PPB_DeviceContext2D_PaintCallback callback,
                      void* callback_data);
};

}  // namespace pp

#endif  // PPAPI_CPP_DEVICE_CONTEXT_2D_H_

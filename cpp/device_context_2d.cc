// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/device_context_2d.h"

#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/rect.h"

namespace pp {

static PPB_DeviceContext2D const* device_context_2d_funcs = NULL;

static bool EnsureFuncs() {
  if (!device_context_2d_funcs) {
    device_context_2d_funcs = reinterpret_cast<PPB_DeviceContext2D const*>(
        Module::Get()->GetBrowserInterface(PPB_DEVICECONTEXT2D_INTERFACE));
    if (!device_context_2d_funcs)
      return false;
  }
  return true;
}

DeviceContext2D::DeviceContext2D() : Resource() {
}

DeviceContext2D::DeviceContext2D(const DeviceContext2D& other)
    : Resource(other) {
}

DeviceContext2D::DeviceContext2D(int32_t width, int32_t height) : Resource() {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(device_context_2d_funcs->Create(
      Module::Get()->pp_module(), width, height));
}

DeviceContext2D::~DeviceContext2D() {
}

DeviceContext2D& DeviceContext2D::operator=(const DeviceContext2D& other) {
  Resource::operator=(other);
  return *this;
}

void DeviceContext2D::swap(DeviceContext2D& other) {
  Resource::swap(other);
}

void DeviceContext2D::PaintImageData(const ImageData& image,
                                     int32_t x, int32_t y,
                                     const Rect& dirty,
                                     PPB_DeviceContext2D_PaintCallback callback,
                                     void* callback_data) {
  if (!EnsureFuncs() || is_null())
    return;
  device_context_2d_funcs->PaintImageData(pp_resource(), image.pp_resource(),
                                          x, y, &dirty.pp_rect(), 1,
                                          callback, callback_data);
}

}  // namespace pp

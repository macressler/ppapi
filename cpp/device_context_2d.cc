// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/device_context_2d.h"

#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/module.h"

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

DeviceContext2D::DeviceContext2D(int32_t width, int32_t height,
                                 bool is_always_opaque) : Resource() {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(device_context_2d_funcs->Create(
      Module::Get()->pp_module(), width, height, is_always_opaque));
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

bool DeviceContext2D::PaintImageData(const ImageData& image,
                                     int32_t x, int32_t y,
                                     const PP_Rect* dirty) {
  if (!EnsureFuncs() || is_null())
    return false;
  return device_context_2d_funcs->PaintImageData(pp_resource(),
                                                 image.pp_resource(),
                                                 x, y, dirty);
}

bool DeviceContext2D::Scroll(const PP_Rect* clip, int32_t dx, int32_t dy) {
  if (!EnsureFuncs() || is_null())
    return false;
  return device_context_2d_funcs->Scroll(pp_resource(), clip, dx, dx);
}

bool DeviceContext2D::ReplaceContents(ImageData* image) {
  if (!EnsureFuncs() || is_null())
    return false;
  return device_context_2d_funcs->ReplaceContents(pp_resource(),
                                                  image->pp_resource());
}

bool DeviceContext2D::Flush(PPB_DeviceContext2D_FlushCallback callback,
                            void* callback_data) {
  if (!EnsureFuncs() || is_null())
    return false;
  return device_context_2d_funcs->Flush(pp_resource(), callback, callback_data);
}

}  // namespace pp

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/device_context_2d.h"

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_device_context_2d.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/point.h"
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
    : Resource(other),
      size_(other.size_) {
}

DeviceContext2D::DeviceContext2D(const Size& size,
                                 bool is_always_opaque)
    : Resource() {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(device_context_2d_funcs->Create(
      Module::Get()->pp_module(), &size.pp_size(), is_always_opaque));
  if (!is_null()) {
    // Only save the size if allocation succeeded.
    size_ = size;
  }
}

DeviceContext2D::~DeviceContext2D() {
}

DeviceContext2D& DeviceContext2D::operator=(const DeviceContext2D& other) {
  DeviceContext2D copy(other);
  swap(copy);
  return *this;
}

void DeviceContext2D::swap(DeviceContext2D& other) {
  Resource::swap(other);
  size_.swap(other.size_);
}

bool DeviceContext2D::PaintImageData(const ImageData& image,
                                     const Point& top_left) {
  if (!EnsureFuncs() || is_null())
    return false;
  return device_context_2d_funcs->PaintImageData(pp_resource(),
                                                 image.pp_resource(),
                                                 &top_left.pp_point(),
                                                 NULL);
}

bool DeviceContext2D::PaintImageData(const ImageData& image,
                                     const Point& top_left,
                                     const Rect& src_rect) {
  if (!EnsureFuncs() || is_null())
    return false;
  return device_context_2d_funcs->PaintImageData(pp_resource(),
                                                 image.pp_resource(),
                                                 &top_left.pp_point(),
                                                 &src_rect.pp_rect());
}

bool DeviceContext2D::Scroll(const Rect& clip, const Point& amount) {
  if (!EnsureFuncs() || is_null())
    return false;
  return device_context_2d_funcs->Scroll(pp_resource(), &clip.pp_rect(),
                                         &amount.pp_point());
}

bool DeviceContext2D::ReplaceContents(ImageData* image) {
  if (!EnsureFuncs() || is_null())
    return false;
  if (device_context_2d_funcs->ReplaceContents(pp_resource(),
                                               image->pp_resource())) {
    // On success, reset the image data. This is to help prevent people
    // from continuing to use the resource which will result in artifacts.
    *image = ImageData();
    return true;
  }
  return false;
}

int32_t DeviceContext2D::Flush(const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  if (is_null())
    return PP_ERROR_BADRESOURCE;
  return device_context_2d_funcs->Flush(pp_resource(),
                                        cc.pp_completion_callback());
}

}  // namespace pp

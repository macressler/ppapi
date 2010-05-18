// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/image_data.h"

#include <string.h>  // Needed for memset.

#include <algorithm>

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_ImageData const* image_data_funcs = NULL;

static bool EnsureFuncs() {
  if (!image_data_funcs) {
    image_data_funcs = reinterpret_cast<PPB_ImageData const*>(
        Module::Get()->GetBrowserInterface(PPB_IMAGEDATA_INTERFACE));
    if (!image_data_funcs)
      return false;
  }
  return true;
}

ImageData::ImageData() : data_(NULL) {
  memset(&desc_, 0, sizeof(PP_ImageDataDesc));
}

ImageData::ImageData(const ImageData& other)
    : Resource(other),
      desc_(other.desc_),
      data_(other.data_) {
}

ImageData::ImageData(PP_ImageDataFormat format,
                     int32_t width, int32_t height,
                     bool init_to_zero)
    : data_(NULL) {
  memset(&desc_, 0, sizeof(PP_ImageDataDesc));

  if (!EnsureFuncs())
    return;

  PassRefFromConstructor(image_data_funcs->Create(Module::Get()->pp_module(),
                                                  format, width, height,
                                                  init_to_zero));
  if (is_null())
    return;
  if (!image_data_funcs->Describe(pp_resource(), &desc_) ||
      !(data_ = image_data_funcs->Map(pp_resource())))
    *this = ImageData();
}

ImageData::~ImageData() {
}

ImageData& ImageData::operator=(const ImageData& other) {
  ImageData copy(other);
  swap(copy);
  return *this;
}

void ImageData::swap(ImageData& other) {
  Resource::swap(other);
  std::swap(desc_, other.desc_);
}

const uint32_t* ImageData::GetAddr32(int x, int y) const {
  // Prefer evil const casts rather than evil code duplication.
  return const_cast<ImageData*>(this)->GetAddr32(x, y);
}

uint32_t* ImageData::GetAddr32(int x, int y) {
  // If we add more image format types that aren't 32-bit, we'd want to check
  // here and fail.
  return reinterpret_cast<uint32_t*>(
      &static_cast<char*>(data())[y * stride() + x * 4]);
}

// static
PP_ImageDataFormat ImageData::GetNativeImageDataFormat() {
  if (!EnsureFuncs())
    return PP_IMAGEDATAFORMAT_BGRA_PREMUL;  // Default to something on failure.
  return image_data_funcs->GetNativeImageDataFormat();
}

}  // namespace pp

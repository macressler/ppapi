// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/image_data.h"

#include <string.h>  // Needed for memset.

#include <algorithm>

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_ImageData> image_data_f(PPB_IMAGEDATA_INTERFACE);

}  // namespace

namespace pp {

ImageData::ImageData() : data_(NULL) {
  memset(&desc_, 0, sizeof(PP_ImageDataDesc));
}

ImageData::ImageData(const ImageData& other)
    : Resource(other),
      desc_(other.desc_),
      data_(other.data_) {
}

ImageData::ImageData(PP_ImageDataFormat format,
                     const Size& size,
                     bool init_to_zero)
    : data_(NULL) {
  memset(&desc_, 0, sizeof(PP_ImageDataDesc));

  if (!image_data_f)
    return;

  PassRefFromConstructor(image_data_f->Create(Module::Get()->pp_module(),
                                              format, &size.pp_size(),
                                              init_to_zero));
  if (!image_data_f->Describe(pp_resource(), &desc_) ||
      !(data_ = image_data_f->Map(pp_resource())))
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
  std::swap(data_, other.data_);
}

const uint32_t* ImageData::GetAddr32(const Point& coord) const {
  // Prefer evil const casts rather than evil code duplication.
  return const_cast<ImageData*>(this)->GetAddr32(coord);
}

uint32_t* ImageData::GetAddr32(const Point& coord) {
  // If we add more image format types that aren't 32-bit, we'd want to check
  // here and fail.
  return reinterpret_cast<uint32_t*>(
      &static_cast<char*>(data())[coord.y() * stride() + coord.x() * 4]);
}

// static
PP_ImageDataFormat ImageData::GetNativeImageDataFormat() {
  if (!image_data_f)
    return PP_IMAGEDATAFORMAT_BGRA_PREMUL;  // Default to something on failure.
  return image_data_f->GetNativeImageDataFormat();
}

}  // namespace pp

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/buffer.h"

#include "ppapi/c/ppb_buffer.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_Buffer> buffer_f(PPB_BUFFER_INTERFACE);

}  // namespace

namespace pp {

Buffer::Buffer() : data_(NULL), size_(0) {
}

Buffer::Buffer(const Buffer& other)
    : Resource(other),
      data_(other.data_),
      size_(other.size_) {
}

Buffer::Buffer(int32_t size) : data_(NULL), size_(0) {
  if (!buffer_f)
    return;

  PassRefFromConstructor(buffer_f->Create(Module::Get()->pp_module(), size));
  if (!buffer_f->Describe(pp_resource(), &size_) ||
      !(data_ = buffer_f->Map(pp_resource())))
    *this = Buffer();
}

Buffer::~Buffer() {
}

Buffer& Buffer::operator=(const Buffer& other) {
  Buffer copy(other);
  swap(copy);
  return *this;
}

void Buffer::swap(Buffer& other) {
  Resource::swap(other);
  std::swap(size_, other.size_);
  std::swap(data_, other.data_);
}

}  // namespace pp


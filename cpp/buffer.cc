// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/buffer.h"

#include "ppapi/c/ppb_buffer.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_Buffer const* buffer_funcs = NULL;

static bool EnsureFuncs() {
  if (!buffer_funcs) {
    buffer_funcs = reinterpret_cast<PPB_Buffer const*>(
        Module::Get()->GetBrowserInterface(PPB_BUFFER_INTERFACE));
    if (!buffer_funcs)
      return false;
  }
  return true;
}

Buffer::Buffer() : data_(NULL), size_(0) {
}

Buffer::Buffer(const Buffer& other)
    : Resource(other),
      data_(other.data_),
      size_(other.size_) {
}

Buffer::Buffer(int32_t size) : data_(NULL), size_(0) {
  if (!EnsureFuncs())
    return;

  PassRefFromConstructor(buffer_funcs->Create(Module::Get()->pp_module(),
                         size));
  if (is_null())
    return;
  if (!buffer_funcs->Describe(pp_resource(), &size_) ||
      !(data_ = buffer_funcs->Map(pp_resource())))
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


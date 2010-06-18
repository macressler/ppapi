// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_BUFFER_H_
#define PPAPI_CPP_BUFFER_H_

#include "ppapi/cpp/resource.h"

namespace pp {

class Buffer : public Resource {
 public:
  // Creates an is_null() Buffer object.
  Buffer();

  Buffer(const Buffer& other);

  // Allocates a new Buffer in the browser with the given size. The
  // resulting object will be is_null() if the allocation failed.
  explicit Buffer(int32_t size);

  ~Buffer();

  Buffer& operator=(const Buffer& other);
  void swap(Buffer& other);

  int32_t size() const { return size_; }
  void* data() const { return data_; }

 private:
  int32_t size_;
  void* data_;
};

}  // namespace pp

#endif  // PPAPI_CPP_BUFFER_H_


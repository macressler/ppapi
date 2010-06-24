// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/directory_entry.h"

#include <string.h>

#include "ppapi/cpp/module.h"

namespace pp {

DirectoryEntry::DirectoryEntry() {
  memset(&data_, 0, sizeof(data_));
}

DirectoryEntry::DirectoryEntry(const DirectoryEntry& other) {
  data_.file_ref = other.data_.file_ref;
  data_.file_type = other.data_.file_type;
  if (data_.file_ref)
    Module::Get()->core().AddRefResource(data_.file_ref);
}

DirectoryEntry::~DirectoryEntry() {
  if (data_.file_ref)
    Module::Get()->core().ReleaseResource(data_.file_ref);
}

DirectoryEntry& DirectoryEntry::operator=(const DirectoryEntry& other) {
  DirectoryEntry copy(other);
  swap(copy);
  return *this;
}

void DirectoryEntry::swap(DirectoryEntry& other) {
  std::swap(data_.file_ref, other.data_.file_ref);
  std::swap(data_.file_type, other.data_.file_type);
}

}  // namespace pp

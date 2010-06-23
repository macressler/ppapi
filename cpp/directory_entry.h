// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DIRECTORY_ENTRY_H_
#define PPAPI_CPP_DIRECTORY_ENTRY_H_

#include "ppapi/c/ppb_directory_reader.h"
#include "ppapi/cpp/file_ref.h"

namespace pp {

class DirectoryEntry {
 public:
  DirectoryEntry();
  DirectoryEntry(const DirectoryEntry& other);
  ~DirectoryEntry();

  DirectoryEntry& operator=(const DirectoryEntry& other);
  void swap(DirectoryEntry& other);

  // Returns true if the DirectoryEntry is invalid or uninitialized.
  bool is_null() const { return !data_.file_ref; }

  // Returns the FileRef held by this DirectoryEntry.
  FileRef file_ref() const { return FileRef(data_.file_ref); }

  // Returns the type of the file referenced by this DirectoryEntry.
  PP_FileType file_type() const { return data_.file_type; }

 private:
  friend class DirectoryReader;
  PP_DirectoryEntry data_;
};

}  // namespace pp

#endif  // PPAPI_CPP_DIRECTORY_ENTRY_H_

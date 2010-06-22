// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DIRECTORY_READER_H_
#define PPAPI_CPP_DIRECTORY_READER_H_

#include <stdlib.h>

#include "ppapi/c/ppb_directory_reader.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class CompletionCallback;
class DirectoryEntry;
class FileRef;

class DirectoryReader : public Resource {
 public:
  // Creates a DirectoryReader for the given directory.
  DirectoryReader(const FileRef& directory_ref);

  DirectoryReader(const DirectoryReader& other);

  DirectoryReader& operator=(const DirectoryReader& other);
  void swap(DirectoryReader& other);

  // See PPB_DirectoryReader::GetNextEntry.
  int32_t GetNextEntry(DirectoryEntry* entry,
                       CompletionCallback* callback = NULL);
};

}  // namespace pp

#endif  // PPAPI_CPP_DIRECTORY_READER_H_

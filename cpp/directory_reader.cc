// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/directory_reader.h"

#include "ppapi/c/pp_errors.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/directory_entry.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_DirectoryReader const* directory_reader_funcs = NULL;

static bool EnsureFuncs() {
  if (!directory_reader_funcs) {
    directory_reader_funcs = reinterpret_cast<PPB_DirectoryReader const*>(
        Module::Get()->GetBrowserInterface(PPB_DIRECTORYREADER_INTERFACE));
    if (!directory_reader_funcs)
      return false;
  }
  return true;
}

DirectoryReader::DirectoryReader(const FileRef& directory_ref) {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(
      directory_reader_funcs->Create(directory_ref.pp_resource()));
}

DirectoryReader::DirectoryReader(const DirectoryReader& other)
    : Resource(other) {
}

DirectoryReader& DirectoryReader::operator=(const DirectoryReader& other) {
  DirectoryReader copy(other);
  swap(copy);
  return *this;
}

void DirectoryReader::swap(DirectoryReader& other) {
  Resource::swap(other);
}

int32_t DirectoryReader::GetNextEntry(DirectoryEntry* entry,
                                      const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return directory_reader_funcs->GetNextEntry(pp_resource(),
                                              &entry->data_,
                                              cc.pp_completion_callback());
}

}  // namespace pp

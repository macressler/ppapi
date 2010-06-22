// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_DIRECTORY_READER_H_
#define PPAPI_C_PPB_DIRECTORY_READER_H_

#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"

typedef struct _pp_CompletionCallback PP_CompletionCallback;
typedef struct _pp_DirectoryEntry PP_DirectoryEntry;

#define PPB_DIRECTORYREADER_INTERFACE "PPB_DirectoryReader;1"

typedef struct _ppb_DirectoryReader {
  // Creates a DirectoryReader for the given directory.  Upon success, the
  // corresponding directory is classified as "in use" by the resulting
  // DirectoryReader object until such time as the DirectoryReader object is
  // destroyed.
  PP_Resource (*Create)(PP_Resource directory_ref);

  // Returns true if the given resource is a DirectoryReader. Returns false if
  // the resource is invalid or some type other than a DirectoryReader.
  bool (*IsDirectoryReader)(PP_Resource resource);

  // Reads the next entry in the directory.  Return PP_OK and sets
  // entry->file_ref to 0 to indicate reaching the end of the directory.  If
  // entry->file_ref is non-zero when passed to GetNextEntry, it will be
  // released before the next file_ref is stored.
  //
  // EXAMPLE USAGE:
  //
  //   PP_Resource reader = reader_funcs->Create(dir_ref);
  //   PP_DirectoryEntry entry = {0};
  //   while (reader_funcs->GetNextEntry(reader, &entry,
  //                                     PP_BlockUntilComplete()) == PP_OK) {
  //     ProcessDirectoryEntry(entry);
  //   }
  //   core_funcs->ReleaseResource(reader);
  //
  int32_t (*GetNextEntry)(PP_Resource directory_reader,
                          PP_DirectoryEntry* entry,
                          PP_CompletionCallback callback);
} PPB_DirectoryReader;

#endif  // PPAPI_C_PPB_DIRECTORY_READER_H_

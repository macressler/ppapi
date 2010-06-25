// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FILE_SYSTEM_H_
#define PPAPI_CPP_FILE_SYSTEM_H_

#include <stdlib.h>

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

typedef struct _pp_FileInfo PP_FileInfo;

namespace pp {

class CompletionCallback;
class FileRef;

// Wraps methods from ppb_file_system.h
class FileSystem {
 public:
  static int32_t MakeDirectory(const FileRef& directory_ref,
                               const CompletionCallback& cc);

  static int32_t MakeDirectoryIncludingAncestors(
      const FileRef& directory_ref,
      const CompletionCallback& cc);

  static int32_t Query(const FileRef& file_ref,
                       PP_FileInfo* result_buf,
                       const CompletionCallback& cc);

  static int32_t Touch(const FileRef& file_ref,
                       PP_Time last_access_time,
                       PP_Time last_modified_time,
                       const CompletionCallback& cc);

  static int32_t Delete(const FileRef& file_ref,
                        const CompletionCallback& cc);

  static int32_t Rename(const FileRef& file_ref,
                        const FileRef& new_file_ref,
                        const CompletionCallback& cc);
};

}  // namespace pp

#endif  // PPAPI_CPP_FILE_SYSTEM_H_

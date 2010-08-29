// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_FILE_SYSTEM_DEV_H_
#define PPAPI_CPP_DEV_FILE_SYSTEM_DEV_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

struct PP_FileInfo_Dev;

namespace pp {

class CompletionCallback;
class FileRef_Dev;

// Wraps methods from ppb_file_system.h
class FileSystem {
 public:
  static int32_t MakeDirectory(const FileRef_Dev& directory_ref,
                               const CompletionCallback& cc);

  static int32_t MakeDirectoryIncludingAncestors(
      const FileRef_Dev& directory_ref,
      const CompletionCallback& cc);

  static int32_t Query(const FileRef_Dev& file_ref,
                       PP_FileInfo_Dev* result_buf,
                       const CompletionCallback& cc);

  static int32_t Touch(const FileRef_Dev& file_ref,
                       PP_Time last_access_time,
                       PP_Time last_modified_time,
                       const CompletionCallback& cc);

  static int32_t Delete(const FileRef_Dev& file_ref,
                        const CompletionCallback& cc);

  static int32_t Rename(const FileRef_Dev& file_ref,
                        const FileRef_Dev& new_file_ref,
                        const CompletionCallback& cc);
};

}  // namespace pp

#endif  // PPAPI_CPP_DEV_FILE_SYSTEM_DEV_H_

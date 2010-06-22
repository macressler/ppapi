// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FILE_IO_H_
#define PPAPI_CPP_FILE_IO_H_

#include <stdlib.h>

#include "ppapi/c/pp_time.h"
#include "ppapi/cpp/resource.h"

typedef struct _pp_FileInfo PP_FileInfo;

namespace pp {

class CompletionCallback;
class FileRef;

class FileIO : public Resource {
 public:
  FileIO();
  FileIO(const FileIO& other);

  FileIO& operator=(const FileIO& other);
  void swap(FileIO& other);

  // PPB_FileIO methods:
  int32_t Open(const FileRef& file_ref,
               int32_t open_flags,
               CompletionCallback* callback = NULL);
  int32_t Query(PP_FileInfo* result_buf,
                CompletionCallback* callback = NULL);
  int32_t Touch(PP_Time last_access_time,
                PP_Time last_modified_time,
                CompletionCallback* callback = NULL);
  int32_t Read(int64_t offset,
               char* buffer,
               int32_t bytes_to_read,
               CompletionCallback* callback = NULL);
  int32_t Write(int64_t offset,
                const char* buffer,
                int32_t bytes_to_write,
                CompletionCallback* callback = NULL);
  int32_t SetLength(int64_t length,
                CompletionCallback* callback = NULL);
  int32_t Flush(CompletionCallback* callback = NULL);
  void Close();

  // PPB_FileIOTrusted methods:
  // NOTE: These are only available to trusted plugins and will return
  // PP_Error_NoInterface if called from an untrusted plugin.
  int32_t GetOSFileDescriptor();
  int32_t WillWrite(int64_t offset,
                    int32_t bytes_to_write,
                    CompletionCallback* callback = NULL);
  int32_t WillSetLength(int64_t length,
                        CompletionCallback* callback = NULL);
};

}  // namespace pp

#endif  // PPAPI_CPP_FILE_IO_H_

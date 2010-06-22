// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_FILE_IO_TRUSTED_H_
#define PPAPI_C_PPB_FILE_IO_TRUSTED_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_CompletionCallback PP_CompletionCallback;

#define PPB_FILEIOTRUSTED_INTERFACE "PPB_FileIOTrusted;1"

// Available only to trusted implementations.
typedef struct _ppb_FileIOTrusted {
  // Returns a POSIX file descriptor corresponding to the given FileIO object.
  // The FileIO object must have been opened with a successful call to
  // FileIO::Open.  The file descriptor will be closed automatically when the
  // FileIO object is closed or destroyed.
  int32_t (*GetOSFileDescriptor)(PP_Resource file_io);
 
  // Notifies the browser that underlying file will be modified.  This gives
  // the browser the opportunity to apply quota restrictions and possibly
  // return an error to indicate that the write is not allowed.
  int32_t (*WillWrite)(PP_Resource file_io,
                       int64_t offset,
                       int32_t bytes_to_write,
                       PP_CompletionCallback callback);
 
  // Notifies the browser that underlying file will be modified.  This gives
  // the browser the opportunity to apply quota restrictions and possibly
  // return an error to indicate that the write is not allowed.
  int32_t (*WillSetLength)(PP_Resource file_io,
                           int64_t length,
                           PP_CompletionCallback callback);
 
  // TODO(darin): Maybe unify the above into a single WillChangeFileSize
  // method?  The above methods have the advantage of mapping to PPB_FileIO
  // Write and SetLength calls.  WillChangeFileSize would require the caller to
  // compute the file size resulting from a Write call, which may be
  // undesirable.
} PPB_FileIOTrusted;

#endif  // PPAPI_C_PPB_FILE_IO_TRUSTED_H_

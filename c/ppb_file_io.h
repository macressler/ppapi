// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_FILE_IO_H_
#define PPAPI_C_PPB_FILE_IO_H_

#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

typedef struct _pp_CompletionCallback PP_CompletionCallback;
typedef struct _pp_FileInfo PP_FileInfo;

#define PPB_FILEIO_INTERFACE "PPB_FileIO;1"

// Use this interface to operate on a regular file (PP_FileType_Regular).
typedef struct _ppb_FileIO {
  // Creates a new FileIO object.  Returns 0 if the module is invalid.
  PP_Resource (*Create)(PP_Module module);

  // Returns true if the given resource is a FileIO. Returns false if the
  // resource is invalid or some type other than a FileIO.
  bool (*IsFileIO)(PP_Resource resource);
 
  // Open the specified regular file for I/O according to the given open flags,
  // which is a bit-mask of the PP_FileOpenFlags values.  Upon success, the
  // corresponding file is classified as "in use" by this FileIO object until
  // such time as the FileIO object is closed or destroyed.
  int32_t (*Open)(PP_Resource file_io,
                  PP_Resource file_ref,
                  int32_t open_flags,
                  PP_CompletionCallback callback);

  // Queries info about the file opened by this FileIO object.  Fails if the
  // FileIO object has not been opened.
  int32_t (*Query)(PP_Resource file_io,
                   PP_FileInfo* info,
                   PP_CompletionCallback callback);

  // Updates timestamps for the file opened by this FileIO object.  Fails if
  // the FileIO object has not been opened.
  int32_t (*Touch)(PP_Resource file_io,
                   PP_Time last_access_time,
                   PP_Time last_modified_time,
                   PP_CompletionCallback callback);
 
  // Read from an offset in the file.  The size of the buffer must be large
  // enough to hold the specified number of bytes to read.  May perform a
  // partial read.  Returns the number of bytes read or an error code.  If the
  // return value is 0, then it indicates that end-of-file was reached.  It is
  // valid to call Read multiple times with a completion callback to queue up
  // parallel reads from the file at different offsets.
  int32_t (*Read)(PP_Resource file_io,
                  int64_t offset,
                  char* buffer,
                  int32_t bytes_to_read,
                  PP_CompletionCallback callback);
 
  // Write to an offset in the file.  May perform a partial write.  Returns the
  // number of bytes written or an error code.  It is valid to call Write
  // multiple times with a completion callback to queue up parallel writes to
  // the file at different offsets.  The FileIO object must have been opened
  // with write access.
  int32_t (*Write)(PP_Resource file_io,
                   int64_t offset,
                   const char* buffer,
                   int32_t bytes_to_write,
                   PP_CompletionCallback callback);
 
  // Sets the length of the file.  If the file size is extended, then the
  // extended area of the file is zero-filled.  The FileIO object must have
  // been opened with write access.
  int32_t (*SetLength)(PP_Resource file_io,
                       int64_t length,
                       PP_CompletionCallback callback);
 
  // Flush changes to disk.  This call can be very expensive!
  int32_t (*Flush)(PP_Resource file_io,
                   PP_CompletionCallback callback);
 
  // Cancels any IO that may be pending, and closes the FileIO object.  Any
  // pending callbacks will still run, reporting PP_Error_Aborted if pending IO
  // was interrupted.  It is NOT valid to call Open again after a call to this
  // method.  Note: If the FileIO object is destroyed, and it is still open,
  // then it will be implicitly closed, so you are not required to call the
  // Close method.
  void (*Close)(PP_Resource file_io);
} PPB_FileIO;

#endif  // PPAPI_C_PPB_FILE_IO_H_

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_FILE_OPEN_FLAGS_H_
#define PPAPI_C_PP_FILE_OPEN_FLAGS_H_

typedef enum _pp_FileOpenFlags {
  // Requests read access to a file.
  PP_FileOpenFlag_Read      = 1 << 0,
  
  // Requests write access to a file.  May be combined with
  // PP_FileOpenFlag_Read to request read and write access.
  PP_FileOpenFlag_Write     = 1 << 1,

  // Requests that the file be created if it does not exist.  If the file
  // already exists, then this flag is ignored unless PP_FileOpenFlag_Exclusive
  // was also specified, in which case FileIO::Open will fail.
  PP_FileOpenFlag_Create    = 1 << 2,

  // Requests that the file be truncated to length 0 if it exists and is a
  // regular file.  PP_FileOpenFlag_Write must also be specified.
  PP_FileOpenFlag_Truncate  = 1 << 3,

  // Requests that the file is created when this flag is combined with
  // PP_FileOpenFlag_Create.  If this flag is specified, and the file already
  // exists, then the FileIO::Open call will fail.
  PP_FileOpenFlag_Exclusive = 1 << 4
} PP_FileOpenFlags;

#endif  // PPAPI_C_PP_FILE_OPEN_FLAGS_H_

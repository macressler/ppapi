// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_FILE_INFO_H_
#define PPAPI_C_PP_FILE_INFO_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_file_system_type.h"
#include "ppapi/c/pp_file_type.h"
#include "ppapi/c/pp_time.h"

typedef struct _pp_FileInfo {
  int64_t size;  // Measured in bytes
  PP_FileType type;
  PP_FileSystemType system_type;
  PP_Time creation_time;
  PP_Time last_access_time;
  PP_Time last_modified_time;
} PP_FileInfo;

#endif  // PPAPI_C_PP_FILE_INFO_H_

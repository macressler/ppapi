// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_DIRECTORY_ENTRY_H_
#define PPAPI_C_PP_DIRECTORY_ENTRY_H_

#include "ppapi/c/pp_file_type.h"
#include "ppapi/c/pp_resource.h"

typedef struct _pp_DirectoryEntry {
  PP_Resource file_ref;
  PP_FileType file_type;
} PP_DirectoryEntry;

#endif  // PPAPI_C_PP_DIRECTORY_ENTRY_H_

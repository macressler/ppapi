// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_FILE_TYPE_H_
#define PPAPI_C_PP_FILE_TYPE_H_

typedef enum _pp_FileType {
  PP_FileType_Regular,
  PP_FileType_Directory,
  PP_FileType_Other  // A catch-all for unidentified types.
} PP_FileType;

#endif  // PPAPI_C_PP_FILE_TYPE_H_

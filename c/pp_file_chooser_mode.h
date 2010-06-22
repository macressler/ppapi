// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_FILE_CHOOSER_MODE_H_
#define PPAPI_C_PP_FILE_CHOOSER_MODE_H_

typedef enum _pp_FileChooserMode {
  PP_FileChooserMode_Open,
  PP_FileChooserMode_OpenMultiple,
  PP_FileChooserMode_Save
  // TODO(darin): Should there be a way to choose a directory?  We may need to
  // kill the save mode since that may get dropped from the web platform specs.
} PP_FileChooserMode;

#endif  // PPAPI_C_PP_FILE_CHOOSER_MODE_H_

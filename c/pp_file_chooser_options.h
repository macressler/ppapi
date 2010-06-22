// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_FILE_CHOOSER_OPTIONS_H_
#define PPAPI_C_PP_FILE_CHOOSER_OPTIONS_H_

#include "ppapi/c/pp_file_chooser_mode.h"

typedef struct _pp_FileChooserOptions {
  PP_FileChooserMode mode;
 
  // A comma-separated list of MIME types such as audio/*,text/plain.  The
  // dialog may restrict selectable files to the specified MIME types.
  // TODO(darin): What if the mime type is unknown to the system?  The plugin
  // may wish to describe the mime type and provide a matching file extension.
  // It is more webby to use mime types here instead of file extensions.
  const char* accept_mime_types;
} PP_FileChooserOptions;

#endif  // PPAPI_C_PP_FILE_CHOOSER_OPTIONS_H_

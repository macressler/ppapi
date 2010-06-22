// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_FILE_CHOOSER_H_
#define PPAPI_C_PPB_FILE_CHOOSER_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"

typedef struct _pp_CompletionCallback PP_CompletionCallback;
typedef struct _pp_FileChooserOptions PP_FileChooserOptions;

#define PPB_FILECHOOSER_INTERFACE "PPB_FileChooser;1"

typedef struct _ppb_FileChooser {
  // Creates a file chooser dialog with the specified options.  The chooser is
  // associated with a particular instance, so that it may be positioned on the
  // screen relative to the tab containing the instance.  Returns 0 if passed
  // an invalid instance.
  PP_Resource (*Create)(PP_Instance instance,
                        const PP_FileChooserOptions* options);

  // Prompts the user to choose a file or files.
  int32_t (*Show)(PP_Resource chooser, PP_CompletionCallback callback);

  // After a successful call to Show, this method may be used to query the
  // chosen files.  It should be called in a loop until it returns 0.
  // Depending on the PP_ChooseFileMode requested when the FileChooser was
  // created, the file refs will either be readable or writable.  Their file
  // system type will be PP_FileSystemType_External.  If the user chose no
  // files or cancelled the dialog, then this method will simply return 0
  // the first time it is called.
  PP_Resource (*GetNextChosenFile)(PP_Resource chooser);
} PPB_FileChooser;

#endif  // PPAPI_C_PPB_FILE_CHOOSER_H_

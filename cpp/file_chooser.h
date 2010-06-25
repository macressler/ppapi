// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FILE_CHOOSER_H_
#define PPAPI_CPP_FILE_CHOOSER_H_

#include <stdlib.h>

#include "ppapi/cpp/resource.h"

typedef struct _pp_FileChooserOptions PP_FileChooserOptions;

namespace pp {

class CompletionCallback;
class FileRef;
class Instance;

class FileChooser : public Resource {
 public:
  // Creates an is_null() FileChooser object.
  FileChooser() {}

  FileChooser(const Instance& instance, const PP_FileChooserOptions* options);

  FileChooser(const FileChooser& other);

  FileChooser& operator=(const FileChooser& other);
  void swap(FileChooser& other);

  // PPB_FileChooser methods:
  int32_t Show(const CompletionCallback& cc);
  FileRef GetNextChosenFile() const;
};

}  // namespace pp

#endif  // PPAPI_CPP_FILE_CHOOSER_H_

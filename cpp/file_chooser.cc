// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/file_chooser.h"

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_file_chooser.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_FileChooser const* file_chooser_funcs = NULL;

static bool EnsureFuncs() {
  if (!file_chooser_funcs) {
    file_chooser_funcs = reinterpret_cast<PPB_FileChooser const*>(
        Module::Get()->GetBrowserInterface(PPB_FILECHOOSER_INTERFACE));
    if (!file_chooser_funcs)
      return false;
  }
  return true;
}

FileChooser::FileChooser(const Instance& instance,
                         const PP_FileChooserOptions* options) {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(file_chooser_funcs->Create(instance.pp_instance(),
                                                    options));
}

FileChooser::FileChooser(const FileChooser& other)
    : Resource(other) {
}

FileChooser& FileChooser::operator=(const FileChooser& other) {
  FileChooser copy(other);
  swap(copy);
  return *this;
}

void FileChooser::swap(FileChooser& other) {
  Resource::swap(other);
}

int32_t FileChooser::Show(CompletionCallback* callback) {
  if (!EnsureFuncs())
    return PP_Error_NoInterface;
  return file_chooser_funcs->Show(pp_resource(),
                                  CompletionCallback::ToPP(callback));
}

FileRef FileChooser::GetNextChosenFile() const {
  if (!EnsureFuncs())
    return FileRef();
  return FileRef(FileRef::PassRef(),
                 file_chooser_funcs->GetNextChosenFile(pp_resource()));
}

}  // namespace pp

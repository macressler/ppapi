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
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_FileChooser> file_chooser_f(PPB_FILECHOOSER_INTERFACE);

}  // namespace

namespace pp {

FileChooser::FileChooser(const Instance& instance,
                         const PP_FileChooserOptions* options) {
  if (!file_chooser_f)
    return;
  PassRefFromConstructor(file_chooser_f->Create(instance.pp_instance(),
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

int32_t FileChooser::Show(const CompletionCallback& cc) {
  if (!file_chooser_f)
    return PP_ERROR_NOINTERFACE;
  return file_chooser_f->Show(pp_resource(), cc.pp_completion_callback());
}

FileRef FileChooser::GetNextChosenFile() const {
  if (!file_chooser_f)
    return FileRef();
  return FileRef(FileRef::PassRef(),
                 file_chooser_f->GetNextChosenFile(pp_resource()));
}

}  // namespace pp

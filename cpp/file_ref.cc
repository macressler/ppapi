// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/file_ref.h"

#include "ppapi/cpp/module.h"

namespace pp {

static PPB_FileRef const* file_ref_funcs = NULL;

static bool EnsureFuncs() {
  if (!file_ref_funcs) {
    file_ref_funcs = reinterpret_cast<PPB_FileRef const*>(
        Module::Get()->GetBrowserInterface(PPB_FILEREF_INTERFACE));
    if (!file_ref_funcs)
      return false;
  }
  return true;
}

FileRef::FileRef(PP_Resource resource) : Resource(resource) {
}

FileRef::FileRef(PassRef, PP_Resource resource) {
  PassRefFromConstructor(resource);
}

FileRef::FileRef(InPersistentFS, const char* path) {
  if (!EnsureFuncs())
    return;
  PP_Module module = Module::Get()->pp_module();
  PassRefFromConstructor(file_ref_funcs->CreatePersistentFileRef(module, path));
}

FileRef::FileRef(InTemporaryFS, const char* path) {
  if (!EnsureFuncs())
    return;
  PP_Module module = Module::Get()->pp_module();
  PassRefFromConstructor(file_ref_funcs->CreateTemporaryFileRef(module, path));
}

FileRef::FileRef(const FileRef& other)
    : Resource(other) {
}

FileRef& FileRef::operator=(const FileRef& other) {
  FileRef copy(other);
  swap(copy);
  return *this;
}

void FileRef::swap(FileRef& other) {
  Resource::swap(other);
}

PP_FileSystemType FileRef::GetFileSystemType() const {
  if (!EnsureFuncs())
    return PP_FILESYSTEMTYPE_EXTERNAL;
  return file_ref_funcs->GetFileSystemType(pp_resource()); 
}

Var FileRef::GetName() const {
  if (!EnsureFuncs())
    return Var();
  return Var(Var::PassRef(), file_ref_funcs->GetName(pp_resource()));
}

Var FileRef::GetPath() const {
  if (!EnsureFuncs())
    return Var();
  return Var(Var::PassRef(), file_ref_funcs->GetPath(pp_resource()));
}

FileRef FileRef::GetParent() const {
  if (!EnsureFuncs())
    return FileRef();
  return FileRef(PassRef(), file_ref_funcs->GetParent(pp_resource()));
}

}  // namespace pp

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/dev/file_ref_dev.h"

#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_FileRef_Dev> file_ref_f(PPB_FILEREF_DEV_INTERFACE);

}  // namespace

namespace pp {

FileRef_Dev::FileRef_Dev(PP_Resource resource) : Resource(resource) {
}

FileRef_Dev::FileRef_Dev(PassRef, PP_Resource resource) {
  PassRefFromConstructor(resource);
}

FileRef_Dev::FileRef_Dev(InPersistentFS, const char* path) {
  if (!file_ref_f)
    return;
  PP_Module module = Module::Get()->pp_module();
  PassRefFromConstructor(file_ref_f->CreatePersistentFileRef(module, path));
}

FileRef_Dev::FileRef_Dev(InTemporaryFS, const char* path) {
  if (!file_ref_f)
    return;
  PP_Module module = Module::Get()->pp_module();
  PassRefFromConstructor(file_ref_f->CreateTemporaryFileRef(module, path));
}

FileRef_Dev::FileRef_Dev(const FileRef_Dev& other)
    : Resource(other) {
}

FileRef_Dev& FileRef_Dev::operator=(const FileRef_Dev& other) {
  FileRef_Dev copy(other);
  swap(copy);
  return *this;
}

void FileRef_Dev::swap(FileRef_Dev& other) {
  Resource::swap(other);
}

PP_FileSystemType_Dev FileRef_Dev::GetFileSystemType() const {
  if (!file_ref_f)
    return PP_FILESYSTEMTYPE_EXTERNAL;
  return file_ref_f->GetFileSystemType(pp_resource());
}

Var FileRef_Dev::GetName() const {
  if (!file_ref_f)
    return Var();
  return Var(Var::PassRef(), file_ref_f->GetName(pp_resource()));
}

Var FileRef_Dev::GetPath() const {
  if (!file_ref_f)
    return Var();
  return Var(Var::PassRef(), file_ref_f->GetPath(pp_resource()));
}

FileRef_Dev FileRef_Dev::GetParent() const {
  if (!file_ref_f)
    return FileRef_Dev();
  return FileRef_Dev(PassRef(), file_ref_f->GetParent(pp_resource()));
}

}  // namespace pp

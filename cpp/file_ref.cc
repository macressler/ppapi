// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/file_ref.h"

#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_FileRef> file_ref_f(PPB_FILEREF_INTERFACE);

}  // namespace

namespace pp {

FileRef::FileRef(PP_Resource resource) : Resource(resource) {
}

FileRef::FileRef(PassRef, PP_Resource resource) {
  PassRefFromConstructor(resource);
}

FileRef::FileRef(InPersistentFS, const char* path) {
  if (!file_ref_f)
    return;
  PP_Module module = Module::Get()->pp_module();
  PassRefFromConstructor(file_ref_f->CreatePersistentFileRef(module, path));
}

FileRef::FileRef(InTemporaryFS, const char* path) {
  if (!file_ref_f)
    return;
  PP_Module module = Module::Get()->pp_module();
  PassRefFromConstructor(file_ref_f->CreateTemporaryFileRef(module, path));
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
  if (!file_ref_f)
    return PP_FILESYSTEMTYPE_EXTERNAL;
  return file_ref_f->GetFileSystemType(pp_resource());
}

Var FileRef::GetName() const {
  if (!file_ref_f)
    return Var();
  return Var(Var::PassRef(), file_ref_f->GetName(pp_resource()));
}

Var FileRef::GetPath() const {
  if (!file_ref_f)
    return Var();
  return Var(Var::PassRef(), file_ref_f->GetPath(pp_resource()));
}

FileRef FileRef::GetParent() const {
  if (!file_ref_f)
    return FileRef();
  return FileRef(PassRef(), file_ref_f->GetParent(pp_resource()));
}

}  // namespace pp

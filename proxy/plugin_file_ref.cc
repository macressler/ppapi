// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_file_ref.h"

#include "native_client/src/include/portability.h"
#include "ppapi/proxy/plugin_globals.h"

namespace ppapi_proxy {

namespace {
PP_Resource Create(PP_Resource file_system, const char* path) {
  UNREFERENCED_PARAMETER(file_system);
  UNREFERENCED_PARAMETER(path);
  return kInvalidResourceId;
}

bool IsFileRef(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

PP_FileSystemType_Dev GetFileSystemType(PP_Resource file_ref) {
  UNREFERENCED_PARAMETER(file_ref);
  return PP_FILESYSTEMTYPE_EXTERNAL;
}

PP_Var GetName(PP_Resource file_ref) {
  UNREFERENCED_PARAMETER(file_ref);
  return PP_MakeUndefined();
}

PP_Var GetPath(PP_Resource file_ref) {
  UNREFERENCED_PARAMETER(file_ref);
  return PP_MakeUndefined();
}

PP_Resource GetParent(PP_Resource file_ref) {
  UNREFERENCED_PARAMETER(file_ref);
  return kInvalidResourceId;
}
}  // namespace

const PPB_FileRef_Dev* PluginFileRef::GetInterface() {
  static const PPB_FileRef_Dev intf = {
    Create,
    IsFileRef,
    GetFileSystemType,
    GetName,
    GetPath,
    GetParent
  };
  return &intf;
}
}  // namespace ppapi_proxy

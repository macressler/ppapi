// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/file_system.h"

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_file_system.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_FileSystem const* file_sys_funcs = NULL;

static bool EnsureFuncs() {
  if (!file_sys_funcs) {
    file_sys_funcs = reinterpret_cast<PPB_FileSystem const*>(
        Module::Get()->GetBrowserInterface(PPB_FILESYSTEM_INTERFACE));
    if (!file_sys_funcs)
      return false;
  }
  return true;
}

// static
int32_t FileSystem::MakeDirectory(const FileRef& directory_ref,
                                  const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_sys_funcs->MakeDirectory(directory_ref.pp_resource(),
                                       false,  // make_ancestors
                                       cc.pp_completion_callback());
}

// static
int32_t FileSystem::MakeDirectoryIncludingAncestors(
    const FileRef& directory_ref,
    const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_sys_funcs->MakeDirectory(directory_ref.pp_resource(),
                                       true,  // make_ancestors
                                       cc.pp_completion_callback());
}

// static
int32_t FileSystem::Query(const FileRef& file_ref,
                          PP_FileInfo* result_buf,
                          const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_sys_funcs->Query(file_ref.pp_resource(),
                               result_buf,
                               cc.pp_completion_callback());
}

// static
int32_t FileSystem::Touch(const FileRef& file_ref,
                          PP_Time last_access_time,
                          PP_Time last_modified_time,
                          const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_sys_funcs->Touch(file_ref.pp_resource(),
                               last_access_time,
                               last_modified_time,
                               cc.pp_completion_callback());
}

// static
int32_t FileSystem::Delete(const FileRef& file_ref,
                           const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_sys_funcs->Delete(file_ref.pp_resource(),
                                cc.pp_completion_callback());
}

// static
int32_t FileSystem::Rename(const FileRef& file_ref,
                           const FileRef& new_file_ref,
                           const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_sys_funcs->Rename(file_ref.pp_resource(),
                                new_file_ref.pp_resource(),
                                cc.pp_completion_callback());
}

}  // namespace pp

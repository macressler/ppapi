// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_FILE_REF_H_
#define PPAPI_C_PPB_FILE_REF_H_

#include "ppapi/c/pp_file_system_type.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"

#define PPB_FILEREF_INTERFACE "PPB_FileRef;1"

// A FileRef is a "weak pointer" to a file in a file system.  It contains a
// PP_FileSystemType identifier and a file path string.  FileRef methods are
// cheap and do not operate on the file system.  To operate on the file system
// use PPB_FileSystem, PPB_FileIO, etc.
typedef struct _ppb_FileRef {
  // Creates a weak pointer to a file in the application's local persistent
  // filesystem.  File paths are POSIX style.  Returns 0 if the path is
  // malformed.
  PP_Resource (*CreatePersistentFileRef)(PP_Module module, const char* path);
 
  // Creates a weak pointer to a file in the application's local temporary
  // filesystem.  File paths are POSIX style.  Returns 0 if the path is
  // malformed.
  PP_Resource (*CreateTemporaryFileRef)(PP_Module module, const char* path);
 
  // Returns the file system identifier of this file.
  PP_FileSystemType (*GetFileSystemType)(PP_Resource file_ref);
 
  // Returns the name of the file.
  PP_Var (*GetName)(PP_Resource file_ref);
 
  // Returns the absolute path of the file.  This method fails if the file
  // system type is PP_FileSystemType_External.
  PP_Var (*GetPath)(PP_Resource file_ref);

  // Returns the parent directory of this file.  If file_ref points to the root
  // of the filesystem, then the root is returned.  This method fails if the
  // file system type is PP_FileSystemType_External.
  PP_Resource (*GetParent)(PP_Resource file_ref);

  // TODO(darin): Add these conversion routines.
#if 0
  // Convert a DOM File object to a FileRef object.
  PP_Resource (*FromFileObject)(PP_Var file_object);

  // Convert a FileRef object to a DOM File object.
  PP_Var (*ToFileObject)(PP_Resource file_ref);
#endif
} PPB_FileRef;

#endif  // PPAPI_C_PPB_FILE_REF_H_

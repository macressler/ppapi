// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FILE_REF_H_
#define PPAPI_CPP_FILE_REF_H_

#include "ppapi/c/ppb_file_ref.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"

namespace pp {

class FileRef : public Resource {
 public:
  // Creates an is_null() FileRef object.
  FileRef() {}

  // This constructor is used when we've gotten a PP_Resource as a return value
  // that we need to addref.
  explicit FileRef(PP_Resource resource);

  // This constructor is used when we've gotten a PP_Resource as a return value
  // that has already been addref'ed for us.
  struct PassRef {};
  FileRef(PassRef, PP_Resource resource);

  // Creates a FileRef pointing to a path in the persistent filesystem.
  struct InPersistentFS {};
  FileRef(InPersistentFS, const char* path);

  // Creates a FileRef pointing to a path in the temporary filesystem.
  struct InTemporaryFS {};
  FileRef(InTemporaryFS, const char* path);

  FileRef(const FileRef& other);

  FileRef& operator=(const FileRef& other);
  void swap(FileRef& other);

  // Returns the file system type.
  PP_FileSystemType GetFileSystemType() const;

  // Returns the name of the file.
  Var GetName() const;

  // Returns the absolute path of the file.  See PPB_FileRef::GetPath for more
  // details.
  Var GetPath() const;

  // Returns the parent directory of this file.  See PPB_FileRef::GetParent for
  // more details.
  FileRef GetParent() const;
};

}  // namespace pp

#endif  // PPAPI_CPP_FILE_REF_H_

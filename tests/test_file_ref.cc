// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_file_ref.h"

#include <stdio.h>

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/dev/ppb_testing_dev.h"
#include "ppapi/cpp/dev/file_ref_dev.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/tests/test_instance.h"

REGISTER_TEST_CASE(FileRef);

namespace {

const char* kPersFileName = "persistent";
const char* kTempFileName = "temporary";
const char* kParentPath = "/foo/bar";
const char* kPersFilePath = "/foo/bar/persistent";
const char* kTempFilePath = "/foo/bar/temporary";

std::string ReportMismatch(const std::string& method_name,
                           const std::string& returned_result,
                           const std::string& expected_result) {
  return method_name + " returned '" + returned_result + "'; '" +
      expected_result + "' expected.";
}

}  // namespace

bool TestFileRef::Init() {
  return true;
}

void TestFileRef::RunTest() {
  RUN_TEST(GetFileSystemType);
  RUN_TEST(GetName);
  RUN_TEST(GetPath);
  RUN_TEST(GetParent);
}

std::string TestFileRef::TestGetFileSystemType() {
  pp::FileRef_Dev file_ref_pers(pp::FileRef_Dev::InPersistentFS(),
                                kPersFilePath);
  if (file_ref_pers.GetFileSystemType() != PP_FILESYSTEMTYPE_LOCALPERSISTENT)
    return "FileRef::CreatePersistentFileRef did not create a persistent file.";

  pp::FileRef_Dev file_ref_temp(pp::FileRef_Dev::InTemporaryFS(),
                                kTempFilePath);
  if (file_ref_temp.GetFileSystemType() != PP_FILESYSTEMTYPE_LOCALTEMPORARY)
    return "FileRef::CreateTemporaryFileRef did not create a persistent file.";

  // TODO(dumi): add a test for an external file once we have access to one

  return "";
}

std::string TestFileRef::TestGetName() {
  pp::FileRef_Dev file_ref_pers(pp::FileRef_Dev::InPersistentFS(),
                                kPersFilePath);
  std::string name = file_ref_pers.GetName().AsString();
  if (name != kPersFileName)
    return ReportMismatch("FileRef::GetName", name, kPersFileName);

  pp::FileRef_Dev file_ref_temp(pp::FileRef_Dev::InTemporaryFS(),
                                kTempFilePath);
  name = file_ref_temp.GetName().AsString();
  if (name != kTempFileName)
    return ReportMismatch("FileRef::GetName", name, kTempFileName);

  // Test the "/" case.
  pp::FileRef_Dev file_ref_slash(pp::FileRef_Dev::InTemporaryFS(), "/");
  name = file_ref_slash.GetName().AsString();
  if (name != "/")
    return ReportMismatch("FileRef::GetName", name, "/");

  // TODO(dumi): add a test for an external file once we have access to one

  return "";
}

std::string TestFileRef::TestGetPath() {
  pp::FileRef_Dev file_ref_pers(pp::FileRef_Dev::InPersistentFS(),
                                kPersFilePath);
  std::string path = file_ref_pers.GetPath().AsString();
  if (path != kPersFilePath)
    return ReportMismatch("FileRef::GetPath", path, kPersFilePath);

  pp::FileRef_Dev file_ref_temp(pp::FileRef_Dev::InTemporaryFS(),
                                kTempFilePath);
  path = file_ref_temp.GetPath().AsString();
  if (path != kTempFilePath)
    return ReportMismatch("FileRef::GetPath", path, kTempFilePath);

  // TODO(dumi): add a test for an external file once we have access to one

  return "";
}

std::string TestFileRef::TestGetParent() {
  pp::FileRef_Dev file_ref_pers(pp::FileRef_Dev::InPersistentFS(),
                                kPersFilePath);
  std::string parent_path = file_ref_pers.GetParent().GetPath().AsString();
  if (parent_path != kParentPath)
    return ReportMismatch("FileRef::GetParent", parent_path, kParentPath);

  pp::FileRef_Dev file_ref_temp(pp::FileRef_Dev::InTemporaryFS(),
                                kTempFilePath);
  parent_path = file_ref_temp.GetParent().GetPath().AsString();
  if (parent_path != kParentPath)
    return ReportMismatch("FileRef::GetParent", parent_path, kParentPath);

  // Test the "/" case.
  pp::FileRef_Dev file_ref_slash(pp::FileRef_Dev::InTemporaryFS(), "/");
  parent_path = file_ref_slash.GetParent().GetPath().AsString();
  if (parent_path != "/")
    return ReportMismatch("FileRef::GetParent", parent_path, "/");

  // Test the "/foo" case (the parent is "/").
  pp::FileRef_Dev file_ref_with_root_parent(pp::FileRef_Dev::InTemporaryFS(),
                                            "/foo");
  parent_path = file_ref_with_root_parent.GetParent().GetPath().AsString();
  if (parent_path != "/")
    return ReportMismatch("FileRef::GetParent", parent_path, "/");

  // TODO(dumi): add a test for an external file once we have access to one

  return "";
}

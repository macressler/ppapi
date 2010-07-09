// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/file_io.h"

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_file_io.h"
#include "ppapi/c/ppb_file_io_trusted.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_FileIO const* file_io_funcs = NULL;
static PPB_FileIOTrusted const* file_io_trusted_funcs = NULL;

static bool EnsureFuncs() {
  if (!file_io_funcs) {
    file_io_funcs = reinterpret_cast<PPB_FileIO const*>(
        Module::Get()->GetBrowserInterface(PPB_FILEIO_INTERFACE));
    if (!file_io_funcs)
      return false;
  }
  return true;
}

static bool EnsureTrustedFuncs() {
  if (!file_io_trusted_funcs) {
    file_io_trusted_funcs = reinterpret_cast<PPB_FileIOTrusted const*>(
        Module::Get()->GetBrowserInterface(PPB_FILEIOTRUSTED_INTERFACE));
    if (!file_io_trusted_funcs)
      return false;
  }
  return true;
}

FileIO::FileIO() {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(file_io_funcs->Create(Module::Get()->pp_module()));
}

FileIO::FileIO(const FileIO& other)
    : Resource(other) {
}

FileIO& FileIO::operator=(const FileIO& other) {
  FileIO copy(other);
  swap(copy);
  return *this;
}

void FileIO::swap(FileIO& other) {
  Resource::swap(other);
}

int32_t FileIO::Open(const FileRef& file_ref,
                     int32_t open_flags,
                     const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_funcs->Open(pp_resource(),
                             file_ref.pp_resource(),
                             open_flags,
                             cc.pp_completion_callback());
}

int32_t FileIO::Query(PP_FileInfo* result_buf,
                      const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_funcs->Query(pp_resource(),
                              result_buf,
                              cc.pp_completion_callback());
}

int32_t FileIO::Touch(PP_Time last_access_time,
                      PP_Time last_modified_time,
                      const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_funcs->Touch(pp_resource(),
                              last_access_time,
                              last_modified_time,
                              cc.pp_completion_callback());
}

int32_t FileIO::Read(int64_t offset,
                     char* buffer,
                     int32_t bytes_to_read,
                     const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_funcs->Read(pp_resource(),
                             offset,
                             buffer,
                             bytes_to_read,
                             cc.pp_completion_callback());
}

int32_t FileIO::Write(int64_t offset,
                      const char* buffer,
                      int32_t bytes_to_write,
                      const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_funcs->Write(pp_resource(),
                              offset,
                              buffer,
                              bytes_to_write,
                              cc.pp_completion_callback());
}

int32_t FileIO::SetLength(int64_t length,
                          const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_funcs->SetLength(pp_resource(),
                                  length,
                                  cc.pp_completion_callback());
}

int32_t FileIO::Flush(const CompletionCallback& cc) {
  if (!EnsureFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_funcs->Flush(pp_resource(), cc.pp_completion_callback());
}

void FileIO::Close() {
  if (!EnsureFuncs())
    return;
  file_io_funcs->Close(pp_resource());
}

int32_t FileIO::GetOSFileDescriptor() {
  if (!EnsureTrustedFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_trusted_funcs->GetOSFileDescriptor(pp_resource());
}

int32_t FileIO::WillWrite(int64_t offset,
                          int32_t bytes_to_write,
                          const CompletionCallback& cc) {
  if (!EnsureTrustedFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_trusted_funcs->WillWrite(pp_resource(),
                                          offset,
                                          bytes_to_write,
                                          cc.pp_completion_callback());
}

int32_t FileIO::WillSetLength(int64_t length,
                              const CompletionCallback& cc) {
  if (!EnsureTrustedFuncs())
    return PP_ERROR_NOINTERFACE;
  return file_io_trusted_funcs->WillSetLength(pp_resource(),
                                              length,
                                              cc.pp_completion_callback());
}

}  // namespace pp

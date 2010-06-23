// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/url_loader.h"

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/ppb_url_loader.h"
#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/url_request_info.h"
#include "ppapi/cpp/url_response_info.h"

namespace pp {

static PPB_URLLoader const* url_loader_funcs = NULL;

static bool EnsureFuncs() {
  if (!url_loader_funcs) {
    url_loader_funcs = reinterpret_cast<PPB_URLLoader const*>(
        Module::Get()->GetBrowserInterface(PPB_URLLOADER_INTERFACE));
    if (!url_loader_funcs)
      return false;
  }
  return true;
}

URLLoader::URLLoader(const Instance& instance) {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(url_loader_funcs->Create(instance.pp_instance()));
}

URLLoader::URLLoader(const URLLoader& other)
    : Resource(other) {
}

URLLoader& URLLoader::operator=(const URLLoader& other) {
  URLLoader copy(other);
  swap(copy);
  return *this;
}

void URLLoader::swap(URLLoader& other) {
  Resource::swap(other);
}

int32_t URLLoader::Open(const URLRequestInfo& request_info,
                        CompletionCallback* callback) {
  if (!EnsureFuncs())
    return PP_Error_NoInterface;
  return url_loader_funcs->Open(pp_resource(),
                                request_info.pp_resource(),
                                CompletionCallback::ToPP(callback));
}

int32_t URLLoader::FollowRedirect(CompletionCallback* callback) {
  if (!EnsureFuncs())
    return PP_Error_NoInterface;
  return url_loader_funcs->FollowRedirect(pp_resource(),
                                          CompletionCallback::ToPP(callback));
}

bool URLLoader::GetUploadProgress(int64_t* bytes_sent,
                                  int64_t* total_bytes_to_be_sent) const {
  if (!EnsureFuncs())
    return false;
  return url_loader_funcs->GetUploadProgress(
      pp_resource(),
      bytes_sent,
      total_bytes_to_be_sent);
}

bool URLLoader::GetDownloadProgress(int64_t* bytes_received,
                                    int64_t* total_bytes_to_be_received) const {
  if (!EnsureFuncs())
    return false;
  return url_loader_funcs->GetDownloadProgress(
      pp_resource(),
      bytes_received,
      total_bytes_to_be_received);
}

URLResponseInfo URLLoader::GetResponseInfo() const {
  if (!EnsureFuncs())
    return URLResponseInfo();
  return URLResponseInfo(URLResponseInfo::PassRef(),
                         url_loader_funcs->GetResponseInfo(pp_resource()));
}

int32_t URLLoader::ReadResponseBody(char* buffer,
                                    int32_t bytes_to_read,
                                    CompletionCallback* callback) {
  if (!EnsureFuncs())
    return PP_Error_NoInterface;
  return url_loader_funcs->ReadResponseBody(pp_resource(),
                                            buffer,
                                            bytes_to_read,
                                            CompletionCallback::ToPP(callback));
}

void URLLoader::Close() {
  if (!EnsureFuncs())
    return;
  url_loader_funcs->Close(pp_resource());
}

}  // namespace pp

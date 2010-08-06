// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/url_response_info.h"

#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_URLResponseInfo> url_response_info_f(
    PPB_URLRESPONSEINFO_INTERFACE);

}  // namespace

namespace pp {

URLResponseInfo::URLResponseInfo(const URLResponseInfo& other)
    : Resource(other) {
}

URLResponseInfo::URLResponseInfo(PassRef, PP_Resource resource) {
  PassRefFromConstructor(resource);
}

URLResponseInfo& URLResponseInfo::operator=(const URLResponseInfo& other) {
  URLResponseInfo copy(other);
  swap(copy);
  return *this;
}

void URLResponseInfo::swap(URLResponseInfo& other) {
  Resource::swap(other);
}

Var URLResponseInfo::GetProperty(PP_URLResponseProperty property) const {
  if (!url_response_info_f)
    return Var();
  return Var(Var::PassRef(),
             url_response_info_f->GetProperty(pp_resource(), property));
}

FileRef URLResponseInfo::GetBody() const {
  if (!url_response_info_f)
    return FileRef();
  return FileRef(FileRef::PassRef(),
                 url_response_info_f->GetBody(pp_resource()));
}

}  // namespace pp

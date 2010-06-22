// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/url_response_info.h"

#include "ppapi/cpp/file_ref.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_URLResponseInfo const* url_response_info_funcs = NULL;

static bool EnsureFuncs() {
  if (!url_response_info_funcs) {
    url_response_info_funcs = reinterpret_cast<PPB_URLResponseInfo const*>(
        Module::Get()->GetBrowserInterface(PPB_URLRESPONSEINFO_INTERFACE));
    if (!url_response_info_funcs)
      return false;
  }
  return true;
}

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
  if (!EnsureFuncs())
    return Var();
  return Var(Var::PassRef(),
             url_response_info_funcs->GetProperty(pp_resource(), property));
}

FileRef URLResponseInfo::GetBody() const {
  if (!EnsureFuncs())
    return FileRef();
  return FileRef(); //FileRef::PassRef(), 
}

}  // namespace pp

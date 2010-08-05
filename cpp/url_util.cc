// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/url_util.h"

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"

namespace pp {

// static
UrlUtil* UrlUtil::Get() {
  static bool tried_to_init = false;
  static UrlUtil util;

  if (!tried_to_init) {
    tried_to_init = true;
    util.interface_ = static_cast<const PPB_UrlUtil*>(
        Module::Get()->GetBrowserInterface(PPB_URLUTIL_INTERFACE));
  }

  if (!util.interface_)
    return NULL;
  return &util;
}

Var UrlUtil::Canonicalize(const Var& url, PP_UrlComponents* components) {
  return Var(Var::PassRef(),
             interface_->Canonicalize(url.pp_var(), components));
}

Var UrlUtil::ResolveRelativeToUrl(const Var& base_url,
                                  const Var& relative_string,
                                  PP_UrlComponents* components) {
  return Var(Var::PassRef(),
             interface_->ResolveRelativeToUrl(base_url.pp_var(),
                                              relative_string.pp_var(),
                                              components));
}

Var UrlUtil::ResoveRelativeToDocument(const Instance& instance,
                                      const Var& relative_string,
                                      PP_UrlComponents* components) {
  return Var(Var::PassRef(),
             interface_->ResolveRelativeToDocument(instance.pp_instance(),
                                                   relative_string.pp_var(),
                                                   components));
}

bool UrlUtil::IsSameSecurityOrigin(const Var& url_a, const Var& url_b) {
  return interface_->IsSameSecurityOrigin(url_a.pp_var(), url_b.pp_var());
}

bool UrlUtil::DocumentCanRequest(const Instance& instance, const Var& url) {
  return interface_->DocumentCanRequest(instance.pp_instance(), url.pp_var());
}

bool UrlUtil::DocumentCanAccessDocument(const Instance& active,
                                        const Instance& target) {
  return interface_->DocumentCanAccessDocument(active.pp_instance(),
                                               target.pp_instance());
}

}  // namespace pp


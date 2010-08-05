// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_URL_UTIL_H_
#define PPAPI_CPP_URL_UTIL_H_

#include "ppapi/c/ppb_url_util.h"
#include "ppapi/cpp/var.h"

namespace pp {

class Instance;
class Module;

// Simple wrapper around the PPB_UrlUtil interface.
class UrlUtil {
 public:
  // This class is just a collection of random functions that aren't
  // particularly attached to anything. So this getter returns a cached
  // instance of this interface. This may return NULL if the browser doesn't
  // support the UrlUtil inteface. Since this is a singleton, don't delete the
  // pointer.
  static UrlUtil* Get();

  Var Canonicalize(const Var& url, PP_UrlComponents* components = NULL);

  Var ResolveRelativeToUrl(const Var& base_url,
                           const Var& relative_string,
                           PP_UrlComponents* components = NULL);
  Var ResoveRelativeToDocument(const Instance& instance,
                               const Var& relative_string,
                               PP_UrlComponents* components = NULL);

  bool IsSameSecurityOrigin(const Var& url_a, const Var& url_b);
  bool DocumentCanRequest(const Instance& instance, const Var& url);
  bool DocumentCanAccessDocument(const Instance& active,
                                 const Instance& target);

 private:
  UrlUtil() : interface_(NULL) {}

  // Copy and assignment are disallowed.
  UrlUtil(const UrlUtil& other);
  UrlUtil& operator=(const UrlUtil& other);

  const PPB_UrlUtil* interface_;
};

}  // namespace pp

#endif  // PPAPI_CPP_URL_UTIL_H_


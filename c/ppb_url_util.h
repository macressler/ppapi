// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_URL_UTIL_H_
#define PPAPI_C_PPB_URL_UTIL_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_URLUTIL_INTERFACE "PPB_UrlUtil;1"

// A component specifies the range of the part of the URL. The begin specifies
// the index into the string of the first character of that component. The len
// specifies the length of that component.
//
// This range does not include any special delimiter for that component, so
// the scheme doesn't include the trailing colon, the username and password
// don't include the @ and :, the port doesn't include the colon, the query
// doesn't include the ?, and the ref doesn't include the #.
//
// The exception is that the path *does* include the first /, since that's an
// integral part of the path.
//
// If the component is not present at all, begin will be 0 and len will be -1.
// If the component is present but empty, the length will be 0 instead. Example:
//   http://foo/search    -> query = (0, -1)
//   http://foo/search?   -> query = (18, 0)
typedef struct _pp_UrlComponent {
  int32_t begin;
  int32_t len;
} PP_UrlComponent;

typedef struct _pp_UrlComponents {
  PP_UrlComponent scheme;
  PP_UrlComponent username;
  PP_UrlComponent password;
  PP_UrlComponent host;
  PP_UrlComponent port;
  PP_UrlComponent path;
  PP_UrlComponent query;
  PP_UrlComponent ref;
} PP_UrlComponents;

// URL encoding: URLs are supplied to this interface as NULL-terminated 8-bit
// strings. You can pass non-ASCII characters which will be interpreted as
// UTF-8. Canonicalized URL strings returned by these functions will be ASCII
// except for the reference fragment (stuff after the '#') which will be
// encoded as UTF-8.
typedef struct _ppb_UrlUtil {
  // Canonicalizes the given URL string according to the rules of the host
  // browser. If the URL is invalid or the var is not a string, this will
  // return a Null var and the components structure will be unchanged.
  //
  // The components pointer, if non-NULL and the canonicalized URL is valid,
  // will identify the components of the resulting URL. Components may be NULL
  // to specify that no component information is necessary.
  PP_Var (*Canonicalize)(PP_Var url, PP_UrlComponents* components);

  // Resolves the given URL relative to the given base URL. The resulting URL
  // is returned as a string. If the resolution is invalid or either of the
  // inputs are not strings, a Null var will be returned. The resulting URL
  // will also be canonicalized according to the rules of the browser.
  //
  // Note that the "relative" URL bay in fact be absolute, in which case it
  // will be returned. This function is identical to resolving the full URL
  // for an <a href="..."> on a web page. Attempting to resolve a relative URL
  // on a base URL that doesn't support this (e.g. "data") will fail and will
  // return a Null var, unless the relative URL is itself absolute.
  //
  // The components pointer, if non-NULL and the canonicalized URL is valid,
  // will identify the components of the resulting URL. Components may be NULL
  // to specify that no component information is necessary.
  PP_Var (*ResolveRelativeToUrl)(PP_Var base_url,
                                 PP_Var relative_string,
                                 PP_UrlComponents* components);

  // Identical to ResolveRelativeToUrl except that the base URL is the base
  // URL of the document containing the given plugin instance.
  //
  // Danger: This will be identical to resolving a relative URL on the page,
  // and might be overridden by the page to something different than its actual
  // URL via the <base> tag. Therefore, resolving a relative URL of "" won't
  // necessarily give you the URL of the page!
  PP_Var (*ResolveRelativeToDocument)(PP_Instance instance,
                                      PP_Var relative_string,
                                      PP_UrlComponents* components);

  // Checks whether the given two URLs are in the same security origin. Returns
  // false if either of the URLs are invalid.
  bool (*IsSameSecurityOrigin)(PP_Var url_a, PP_Var url_b);

  // Checks whether the document hosting the given plugin instance can access
  // the given URL according to the same origin policy of the browser. Returns
  // false if the instance or the URL is invalid.
  bool (*DocumentCanRequest)(PP_Instance instance, PP_Var url);

  // Checks whether the document containing the |active| plugin instance can
  // access the document containing the |target| plugin instance according to
  // the security policy of the browser. This includes the same origin policy
  // and any cross-origin capabilities enabled by the document. If either of
  // the plugin instances are invalid, returns false.
  bool (*DocumentCanAccessDocument)(PP_Instance active, PP_Instance target);
} PPB_UrlUtil;

#endif  // PPAPI_C_PPB_URL_UTIL_H_


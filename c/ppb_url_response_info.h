// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_URL_RESPONSE_INFO_H_
#define PPAPI_C_PPB_URL_RESPONSE_INFO_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"

enum PP_URLResponseProperty {
  PP_URLResponseProperty_URL,             // string
  PP_URLResponseProperty_RedirectURL,     // string
  PP_URLResponseProperty_RedirectMethod,  // string
  PP_URLResponseProperty_StatusCode,      // int32
  PP_URLResponseProperty_StatusLine,      // string
  PP_URLResponseProperty_Headers          // string, \n-delim
};

#define PPB_URLRESPONSEINFO_INTERFACE "PPB_URLResponseInfo;1"

typedef struct _ppb_URLResponseInfo {
  // Returns true if the given resource is an URLResponseInfo. Returns false if
  // the resource is invalid or some type other than an URLResponseInfo.
  bool (*IsURLResponseInfo)(PP_Resource resource);

  // Gets a response property.  Return PP_VarType_Void if an input parameter is
  // invalid.
  PP_Var (*GetProperty)(PP_Resource response,
                        PP_URLResponseProperty property);

  // Returns the FileRef pointing to the file containing the response body.
  // This is only valid if StreamToFile was specified in the URLRequestInfo
  // used to produce this response.  This file remains valid for the lifetime
  // of this URLResponseInfo object.  Returns 0 if StreamToFile was not
  // requested.
  PP_Resource (*GetBody)(PP_Resource response);
} PPB_URLResponseInfo;

#endif  // PPAPI_C_PPB_URL_RESPONSE_INFO_H_

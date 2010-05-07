// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_RESOURCE_H_
#define PPAPI_C_PP_RESOURCE_H_

#include "ppapi/c/pp_stdint.h"

typedef struct _pp_Resource {
  // Internal ID for strings and objects. This ID will be 0 for an undefined
  // resource. Otherwise, the value is a browser-assigned opaque handle.
  int64_t id;
} PP_Resource;

#endif  // PPAPI_C_PP_RESOURCE_H_

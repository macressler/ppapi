// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_MODULE_H_
#define PPAPI_C_PP_MODULE_H_

#include "ppapi/c/pp_stdint.h"

// A module uniquely identifies one plugin library.
typedef struct _pp_Module {
  int64_t id;
} PP_Module;

#endif  // PPAPI_C_PP_MODULE_H_

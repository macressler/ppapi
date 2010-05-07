// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_INSTANCE_H_
#define PPAPI_C_PP_INSTANCE_H_

#include "ppapi/c/pp_stdint.h"

// A PP_Instance uniquely identifies one plugin instance, which is one time
// that a page says <embed>. There can be multiple instances of the same plugin
// type on a page that will all be in the same module.
typedef struct _pp_Instance {
  int64_t id;
} PP_Instance;

#endif  // PPAPI_C_PP_INSTANCE_H_

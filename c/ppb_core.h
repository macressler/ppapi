// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_CORE_H_
#define PPAPI_C_PPB_CORE_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

typedef struct _pp_Resource PP_Resource;

#define PPB_CORE_INTERFACE "PPB_Core;1"

typedef struct _ppb_Core {
  // Same as AddRefVar for Resources.
  void (*AddRefResource)(PP_Resource resource);

  // Same as ReleaseVar for Resources.
  void (*ReleaseResource)(PP_Resource resource);

  // TODO(brettw) some thread/timer stuff should go here, as well as
  // GetCurrentTime.
} PPB_Core;

#endif  // PPAPI_C_PPB_CORE_H_

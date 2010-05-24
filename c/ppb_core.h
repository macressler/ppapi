// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_CORE_H_
#define PPAPI_C_PPB_CORE_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

#define PPB_CORE_INTERFACE "PPB_Core;1"

typedef struct _ppb_Core {
  // Same as AddRefVar for Resources.
  void (*AddRefResource)(PP_Resource resource);

  // Same as ReleaseVar for Resources.
  void (*ReleaseResource)(PP_Resource resource);

  // Memory management.  MemAlloc will return NULL if the allocation fails, and
  // it is safe to pass NULL to MemFree.
  void* (*MemAlloc)(size_t num_bytes);
  void (*MemFree)(void* ptr);

  // Returns the wall clock time according to the browser, which is defined as
  // the number of seconds since the Epoch (00:00:00 UTC, January 1, 1970).
  // This clock is used by the browser when passing time values to the plugin
  // (e.g., via the PP_Event::time_stamp_seconds field).
  double (*GetTime)();

  // Schedules work to be executed on the main plugin thread after the
  // specified delay.  The context parameter is a user defined argument that
  // will be passed to the callback function when it runs.  NOTE: If the
  // browser is shutting down or if the plugin has no instances, then the
  // function may not be called.
  void (*CallOnMainThread)(int delay_in_milliseconds,
                           void (*func)(void* context),
                           void* context);
} PPB_Core;

#endif  // PPAPI_C_PPB_CORE_H_

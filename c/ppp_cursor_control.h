// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_CURSOR_CONTROL_H_
#define PPAPI_C_PPP_CURSOR_CONTROL_H_

#include "ppapi/c/pp_instance.h"

#define PPP_CURSOR_CONTROL_INTERFACE "PPP_CursorControl;1"

typedef struct _ppp_CursorControl {
  // Called when the instance looses the cursor lock, e.g. because the user
  // pressed the ESC key.
  void (*CursorLockLost)(PP_Instance instance);
} PPP_CursorControl;

#endif  // PPAPI_C_PPP_CURSOR_CONTROL_H_


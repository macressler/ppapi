// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_SCROLLBAR_H_
#define PPAPI_C_PPP_SCROLLBAR_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

// Interface for the plugin to implement when using a scrollbar widget.
#define PPP_SCROLLBAR_INTERFACE "PPP_Scrollbar;1"

typedef struct _ppp_Scrollbar {
  // Informs the instance that the scrollbar's value has changed.
  void (*ValueChanged)(PP_Instance instance,
                       PP_Resource scrollbar,
                       uint32_t value);
} PPP_Scrollbar;

#endif  // PPAPI_C_PPP_SCROLLBAR_H_

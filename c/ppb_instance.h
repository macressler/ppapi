// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_INSTANCE_H_
#define PPAPI_C_PPB_INSTANCE_H_

#include "ppapi/c/pp_rect.h"

typedef struct _pp_Instance PP_Instance;
typedef struct _pp_Resource PP_Resource;

#define PPB_INSTANCE_INTERFACE "PPB_Instance;1"

typedef struct _ppb_Instance {
  // Binds the given graphics device as the current drawing surface. The
  // contents of this device is what will be displayed in the plugin's area
  // on the web page. The device must be a 2D or a 3D device.
  //
  // Any previously-bound device will be Release()d.
  //
  // Returns true if the bind was successful. False means the device was not
  // the correct type. On success, a reference to the device will be held by
  // the plugin instance, so the caller can release its reference if it
  // chooses.
  //
  // Binding will automatically schedule a repaint the plugin on the page. This
  // repaint will happen asynchronously.
  bool (*BindGraphicsDeviceContext)(PP_Instance instance,
                                    PP_Resource device);
} PPB_Instance;

#endif  // PPAPI_C_PPB_INSTANCE_H_

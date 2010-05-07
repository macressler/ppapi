// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_DEVICE_CONTEXT_2D_H_
#define PPAPI_C_PPB_DEVICE_CONTEXT_2D_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_Module PP_Module;
typedef struct _pp_Rect PP_Rect;

// Callback function type for PaintImageData.
typedef void (*PPB_DeviceContext2D_PaintCallback)(PP_Resource context,
                                                  void* data);

#define PPB_DEVICECONTEXT2D_INTERFACE "PPB_DeviceContext2D;1"

typedef struct _ppb_DeviceContext2D {
  // The returned device context will not be bound to any plugin instance on
  // creation (call BindGraphicsDeviceContext on the plugin instance to do
  // that. The device context have a lifetime that can exceed that of the given
  // plugin instance.
  PP_Resource (*Create)(PP_Module module, int32_t width, int32_t height);

  // Paints the given image at the given position in the device.
  //
  // A 0-sized dirty rect means to paint the entire ImageData (do not use a
  // NULL pointer). The dirty rect is in the coordinate space of the image, not
  // of the plugin.
  //
  // TODO(brettw) this needs error handling.
  void (*PaintImageData)(PP_Resource device_context,
                         PP_Resource image,
                         int32_t x, int32_t y,
                         const PP_Rect* dirty,
                         uint32_t dirty_rect_count,
                         PPB_DeviceContext2D_PaintCallback callback,
                         void* callback_data);
} PPB_DeviceContext2D;

#endif  // PPAPI_C_PPB_DEVICE_CONTEXT_2D_H_

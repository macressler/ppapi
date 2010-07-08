// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_INSTANCE_H_
#define PPAPI_C_PPB_INSTANCE_H_

#include "ppapi/c/pp_cursor_type.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_var.h"

#define PPB_INSTANCE_INTERFACE "PPB_Instance;1"

typedef struct _ppb_Instance {
  // Returns a reference to the DOM window containing this instance.
  PP_Var (*GetWindowObject)(PP_Instance instance);

  // Returns a reference to the DOM element containing this instance.
  PP_Var (*GetOwnerElementObject)(PP_Instance instance);

  // Binds the given graphics device as the current drawing surface. The
  // contents of this device is what will be displayed in the plugin's area
  // on the web page. The device must be a 2D or a 3D device.
  //
  // You can pass a NULL resource as the device parameter to unbind all
  // devices from the given instance. The instance will then appear
  // transparent. Re-binding the same device will return true and will do
  // nothing. Unbinding a device will drop any pending flush callbacks.
  //
  // Any previously-bound device will be Release()d. It is an error to bind
  // a device when it is already bound to another plugin instance. If you want
  // to move a device between instances, first unbind it from the old one, and
  // then rebind it to the new one.
  //
  // Returns true if the bind was successful. False means the device was not
  // the correct type. On success, a reference to the device will be held by
  // the plugin instance, so the caller can release its reference if it
  // chooses.
  //
  // Binding a device will invalidate that portion of the web page to flush the
  // contents of the new device to the screen.
  bool (*BindGraphicsDeviceContext)(PP_Instance instance,
                                    PP_Resource device);

  // Returns true if the instance is full-frame.  Such a plugin represents the
  // entire document in a frame rather than an embedded resource. This can
  // happen if the user does a top level navigation or the page specifies an
  // iframe to a resource with a MIME type registered by the plugin.
  bool (*IsFullFrame)(PP_Instance instance);

  // Set a cursor.  If "type" is PP_CURSOR_TYPE_CUSTOM, then "custom_image"
  // must be an ImageData resource containing the cursor and "hot_spot" must
  // contain the offset within that image that refers to the cursor's position.
  bool (*SetCursor)(PP_Instance instance,
                    PP_CursorType type,
                    PP_Resource custom_image,
                    const PP_Point* hot_spot);
                    
} PPB_Instance;

#endif  // PPAPI_C_PPB_INSTANCE_H_

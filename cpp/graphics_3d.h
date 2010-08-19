// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_GRAPHICS_3D_H_
#define PPAPI_CPP_GRAPHICS_3D_H_

#include "ppapi/c/ppb_graphics_3d.h"
#include "ppapi/c/ppb_opengles.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class Graphics3D : public Resource {
 public:
  static bool ResetCurrent();
  static Graphics3D GetCurrentContext();
  static uint32_t GetError();
  static const PPB_OpenGLES* GetImplementation();

  // Creates an is_null() Graphics3D object.
  Graphics3D() {}

  Graphics3D(const Instance& instance, int32_t config, int32_t share_context,
             const int32_t* attrib_list);

  bool MakeCurrent() const;
  bool SwapBuffers() const;


 protected:
  explicit Graphics3D(PP_Resource resource_id) : Resource(resource_id) {}
  static Graphics3D FromResource(PP_Resource resource_id);
};

}  // namespace pp

#endif  // PPAPI_CPP_GRAPHICS_3D_H_


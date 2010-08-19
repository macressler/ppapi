// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/graphics_3d.h"

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

const PPB_OpenGLES* pepper_opengl_interface;

namespace {

DeviceFuncs<PPB_Graphics3D> graphics_3d_f(PPB_GRAPHICS_3D_INTERFACE);
DeviceFuncs<PPB_OpenGLES> opengles_f(PPB_OPENGLES_INTERFACE);

inline void InitializeOpenGLCInterface() {
  if (!pepper_opengl_interface) {
    pepper_opengl_interface = &(*opengles_f);
  }
}

}  // namespace

namespace pp {

Graphics3D Graphics3D::FromResource(PP_Resource resource_id) {
  if (graphics_3d_f && graphics_3d_f->IsGraphics3D(resource_id))
    return Graphics3D(resource_id);
  else
    return Graphics3D();
}

bool Graphics3D::ResetCurrent() {
  return graphics_3d_f && graphics_3d_f->MakeCurent(0);
}

Graphics3D Graphics3D::GetCurrentContext() {
  if (graphics_3d_f)
    return FromResource(graphics_3d_f->GetCurrentContext());
  else
    return Graphics3D();
}

uint32_t Graphics3D::GetError() {
  if (graphics_3d_f)
    return graphics_3d_f->GetError();
  else
    return PP_GRAPHICS_3D_ERROR_NOT_INITIALIZED;
}

const PPB_OpenGLES* Graphics3D::GetImplementation() {
  return &(*opengles_f);
}

Graphics3D::Graphics3D(const Instance& instance, int32_t config,
                       int32_t share_context, const int32_t* attrib_list) {
  if (graphics_3d_f && opengles_f) {
    InitializeOpenGLCInterface();
    PassRefFromConstructor(graphics_3d_f->CreateContext(instance.pp_instance(),
                                                        config, share_context,
                                                        attrib_list));
  }
}

bool Graphics3D::MakeCurrent() const {
  InitializeOpenGLCInterface();
  return graphics_3d_f && graphics_3d_f->MakeCurent(pp_resource());
}

bool Graphics3D::SwapBuffers() const {
  return graphics_3d_f && graphics_3d_f->SwapBuffers(pp_resource());
}

}  // namespace pp


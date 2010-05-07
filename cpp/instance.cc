// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/instance.h"

#include "ppapi/c/ppb_instance.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"

namespace pp {

namespace {

const PPB_Instance* ppb_instance_funcs = NULL;

bool EnsureFuncs() {
  if (!ppb_instance_funcs) {
    ppb_instance_funcs = reinterpret_cast<PPB_Instance const*>(
        Module::Get()->GetBrowserInterface(PPB_INSTANCE_INTERFACE));
    if (!ppb_instance_funcs)
      return false;
  }
  return true;
}

}  // namespace

Instance::Instance(PP_Instance instance) : pp_instance_(instance) {
}

Instance::~Instance() {
}

bool Instance::Init(uint32_t argc, const char* argn[], const char* argv[]) {
  return true;
}

Var Instance::GetInstanceObject() {
  return Var();
}

bool Instance::BindGraphicsDeviceContext(const DeviceContext2D& context) {
  if (!EnsureFuncs())
    return false;
  return ppb_instance_funcs->BindGraphicsDeviceContext(pp_instance(),
                                                       context.pp_resource());
}

}  // namespace pp

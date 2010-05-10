// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/module.h"

#include <string.h>

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppp_instance.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/var.h"

namespace pp {

static Module* module_singleton = NULL;

// PPP_Instance implementation -------------------------------------------------

bool Instance_New(PP_Instance instance) {
  if (!module_singleton)
    return false;
  Instance* obj = module_singleton->CreateInstance(instance);
  if (obj) {
    module_singleton->current_instances_[instance] = obj;
    return true;
  }
  return false;
}

void Instance_Delete(PP_Instance instance) {
  if (!module_singleton)
    return;
  Module::InstanceMap::iterator found =
      module_singleton->current_instances_.find(instance);
  if (found == module_singleton->current_instances_.end())
    return;

  // Remove it from the map before deleting to try to catch reentrancy.
  Instance* obj = found->second;
  module_singleton->current_instances_.erase(found);
  delete obj;
}

bool Instance_Initialize(PP_Instance pp_instance,
                         uint32_t argc,
                         const char* argn[],
                         const char* argv[]) {
  if (!module_singleton)
    return false;
  Instance* instance = module_singleton->InstanceForPPInstance(pp_instance);
  if (!instance)
    return false;
  return instance->Init(argc, argn, argv);
}

bool Instance_HandleEvent(PP_Instance pp_instance,
                          const PP_Event* event) {
  if (!module_singleton)
    return false;
  Instance* instance = module_singleton->InstanceForPPInstance(pp_instance);
  if (!instance)
    return false;
  return instance->HandleEvent(*event);
}

PP_Var Instance_GetInstanceObject(PP_Instance pp_instance) {
  if (!module_singleton)
    return Var().Detach();
  Instance* instance = module_singleton->InstanceForPPInstance(pp_instance);
  if (!instance)
    return Var().Detach();
  return instance->GetInstanceObject().Detach();
}

void Instance_ViewChanged(PP_Instance pp_instance,
                          const PP_Rect* position,
                          const PP_Rect* clip) {
  if (!module_singleton)
    return;
  Instance* instance = module_singleton->InstanceForPPInstance(pp_instance);
  if (!instance)
    return;
  instance->ViewChanged(*position, *clip);
}

static PPP_Instance instance_interface = {
  &Instance_New,
  &Instance_Delete,
  &Instance_Initialize,
  &Instance_HandleEvent,
  &Instance_GetInstanceObject,
  &Instance_ViewChanged,
};

// Module ----------------------------------------------------------------------

Module::Module() : get_browser_interface_(NULL), core_(NULL) {
  pp_module_.id = 0;
  module_singleton = this;
}

Module::~Module() {
  module_singleton = NULL;
}

// static
Module* Module::Get() {
  return module_singleton;
}

const void* Module::GetInstanceInterface(const char* interface_name) {
  if (strcmp(interface_name, PPP_INSTANCE_INTERFACE) == 0)
    return &instance_interface;
  return NULL;
}

const void* Module::GetBrowserInterface(const char* interface_name) {
  return get_browser_interface_(interface_name);
}

Instance* Module::InstanceForPPInstance(PP_Instance instance) {
  InstanceMap::iterator found = current_instances_.find(instance);
  if (found == current_instances_.end())
    return NULL;
  return found->second;
}

bool Module::InternalInit(PP_Module mod,
                          PPB_GetInterface get_browser_interface) {
  pp_module_ = mod;
  get_browser_interface_ = get_browser_interface;
  core_ = reinterpret_cast<const PPB_Core*>(GetBrowserInterface(
      PPB_CORE_INTERFACE));
  if (!core_)
    return false;  // Can't run without the core interface.

  return Init();
}

}  // namespace pp

// Global PPP functions --------------------------------------------------------

PP_EXPORT int PPP_InitializeModule(PP_Module module_id,
                                   PPB_GetInterface get_browser_interface) {
  pp::Module* module = pp::CreateModule();
  if (!module)
    return 1;

  if (!module->InternalInit(module_id, get_browser_interface)) {
    delete module;
    return 2;  // Return a different error code for init failure so we can
               // differentiate these two cases when debugging.
  }
  return 0;
}

PP_EXPORT void PPP_ShutdownModule() {
  delete pp::module_singleton;
}

PP_EXPORT const void* PPP_GetInterface(const char* interface_name) {
  if (!pp::module_singleton)
    return NULL;
  return pp::module_singleton->GetInstanceInterface(interface_name);
}

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_MODULE_H_
#define PPAPI_CPP_MODULE_H_

#include <map>

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_core.h"
#include "ppapi/c/ppb.h"
#include "ppapi/c/ppp.h"

namespace pp {

class Instance;

class Module {
 public:
  // You may not call any other PP functions from the constructor, put them
  // in Init instead. Various things will not be set up until the constructor
  // completes.
  Module();
  virtual ~Module();

  // Returns the global instance of this module object, or NULL if the module
  // is not initialized yet.
  static Module* Get();

  // This function will be automatically called after the object is created.
  // This is where you can put functions that rely on other parts of the API,
  // now that the module has been created.
  virtual bool Init() { return true; }

  PP_Module pp_module() const { return pp_module_; }
  const PPB_Core& core() const { return *core_; }

  // Implements GetInterface for the browser to get plugin interfaces. Override
  // if you need to implement your own interface types that this wrapper
  // doesn't support.
  virtual const void* GetInstanceInterface(const char* interface_name);

  // Returns an interface in the browser.
  const void* GetBrowserInterface(const char* interface_name);

  // Returns the object associated with this PP_Instance, or NULL if one is
  // not found.
  Instance* InstanceForPPInstance(PP_Instance instance) const;

  // Sets the browser interface and calls the regular init function that
  // can be overridden by the base classes.
  //
  // TODO(brettw) make this private when I can figure out how to make the
  // initialize function a friend.
  bool InternalInit(PP_Module mod,
                    PPB_GetInterface get_browser_interface);

 protected:
  // Override to create your own plugin type.
  virtual Instance* CreateInstance(PP_Instance instance) = 0;

 private:
  friend bool Instance_New(PP_Instance instance);
  friend void Instance_Delete(PP_Instance instance);

  // Unimplemented (disallow copy and assign).
  Module(const Module&);
  Module& operator=(const Module&);

  // Instance tracking.
  class CompareInstance {
   public:
    bool operator()(PP_Instance a, PP_Instance b) {
      return a.id < b.id;
    }
  };
  typedef std::map<PP_Instance, Instance*, CompareInstance> InstanceMap;
  InstanceMap current_instances_;

  PP_Module pp_module_;
  PPB_GetInterface get_browser_interface_;
  PPB_Core const* core_;
};

// Implemented by the embedder.
//
// Creates the pp::Module object associated with this plugin. Returns the
// module if it was successfully created, or NULL on failure. Upon failure,
// the plugin will be unloaded.
Module* CreateModule();

}  // namespace pp

#endif  // PPAPI_CPP_MODULE_H_

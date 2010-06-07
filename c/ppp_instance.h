// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_INSTANCE_H_
#define PPAPI_C_PPP_INSTANCE_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"

typedef struct _pp_Event PP_Event;
typedef struct _pp_Var PP_Var;

#define PPP_INSTANCE_INTERFACE "PPP_Instance;1"

typedef struct _ppp_Instance {
  // You may not call other parts of PPAPI from this function (for example, do
  // not execute JS or modify the DOM. Put such code in InitializeInstance.
  //
  // If this returns false, the plugin will be deleted and the DeleteInstance
  // will NOT be called (since we assume it was not properly initialized).
  bool (*New)(PP_Instance instance);

  // Called when the plugin instance is destroyed.
  void (*Delete)(PP_Instance instance);

  // Called immediately after New. Plugins can do instance-specific
  // initialization that requires other parts of PPAPI.
  //
  // If the plugin reports failure from this function, the plugin to be deleted
  // and DeleteInstance will be called.
  bool (*Initialize)(PP_Instance instance,
                     uint32_t argc,
                     const char* argn[],
                     const char* argv[]);

  // General handler for all event types. Returns true if the event was handled
  // or false if it was not.
  //
  // If the event was handled, it will not be forwarded to the web page or
  // browser. If it was not handled, it will bubble according to the normal
  // rules. So it is important that a plugin respond accurately with whether
  // event propogation should continue.
  bool (*HandleEvent)(PP_Instance instance, const PP_Event* event);

  // Returns a Var representing the instance object to the web page. Normally
  // this will be a PPP_Class object that exposes certain methods the page
  // may want to call.
  //
  // On Failure, the returned var should be a "void" variant.
  //
  // The returned PP_Var should have a reference added for the caller, which
  // will be responsible for Release()ing that reference.
  PP_Var (*GetInstanceObject)(PP_Instance instance);

  // Called when the position, the size, or the clip rect has changed.
  //
  // The |position| is the location on the page of this plugin instance. This is
  // relative to the top left corner of the viewport, which changes as the page
  // is scrolled.
  //
  // The |clip| indicates the visible region of the plugin instance. This is
  // relative to the top left of the plugin's coordinate system (not the page).
  // If the plugin is invisible, the clip rect will be (0, 0, 0, 0).
  void (*ViewChanged)(PP_Instance instance,
                      const PP_Rect* position,
                      const PP_Rect* clip);
} PPP_Instance;

#endif  // PPAPI_C_PPP_INSTANCE_H_

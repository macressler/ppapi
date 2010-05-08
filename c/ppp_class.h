// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_CLASS_H_
#define PPAPI_C_PPP_CLASS_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_var.h"

typedef struct _pp_Var PP_Var;

// Interface for the plugin to implement JavaScript-accessible objects.
//
// This interface has no interface name. Instead, the plugin passes a pointer
// to this interface to PPB_Var.CreateObject that corresponds to the object
// being implemented.
typedef struct _ppp_Class {
  bool (*HasProperty)(void* object,
                      PP_Var name);

  PP_Var (*GetProperty)(void* object,
                        PP_Var name,
                        PP_Var* exception);

  void (*GetAllProperties)(void* object,
                           uint32_t* property_count,
                           PP_Var** properties);

  void (*SetProperty)(void* object,
                      PP_Var name,
                      PP_Var value,
                      PP_Var* exception);

  void (*RemoveProperty)(void* object,
                         PP_Var name,
                         PP_Var* exception);

  // TODO(brettw) need native array access here.

  // method_name is guaranteed to be either a string or an integer.
  PP_Var (*Call)(void* object,
                 PP_Var method_name,
                 uint32_t argc,
                 PP_Var* argv,
                 PP_Var* exception);

  PP_Var (*Construct)(void* object,
                      uint32_t argc,
                      PP_Var* argv,
                      PP_Var* exception);

  // Called when the reference count of the object reaches 0. Normally, plugins
  // would free their internal data pointed to by the |object| pointer.
  void (*Deallocate)(void* object);
} PPP_Class;

#endif  // PPAPI_C_PPP_CLASS_H_

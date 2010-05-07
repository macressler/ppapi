// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_VAR_H_
#define PPAPI_C_PP_VAR_H_

#include "ppapi/c/pp_stdint.h"

enum PP_VarType {
  PP_VarType_Void,
  PP_VarType_Null,
  PP_VarType_Bool,
  PP_VarType_Int32,
  PP_VarType_Double,
  PP_VarType_String,
  PP_VarType_Object
};

typedef struct _pp_Var {
  PP_VarType type;
  union {
    bool as_bool;
    int32_t as_int;
    double as_double;

    // Internal ID for strings and objects.
    int64_t as_id;
  } value;
} PP_Var;

#endif  // PPAPI_C_PP_VAR_H_


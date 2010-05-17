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

    // Numeric values. JavaScript has a "number" type for holding a number, and
    // does not differentiate between floating point and integer numbers. The
    // JavaScript library will try to optimize operations by using integers
    // when possible, but could end up with doubles depending on how the number
    // was arrived at.
    //
    // Therefore, you should not rely on having a predictable and reproducible
    // int/double differentiation. The best bet is to have a wrapper for this
    // which always gets out the type you expect, converting as necessary.
    int32_t as_int;
    double as_double;

    // Internal ID for strings and objects. The identifier is an opaque handle
    // assigned by the browser to the plugin. It is guaranteed never to be 0,
    // so a plugin can initialize this ID to 0 to indicate a "NULL handle."
    int64_t as_id;
  } value;
} PP_Var;

inline PP_Var PP_MakeVoid() {
  PP_Var result = { PP_VarType_Void, {0} };
  return result;
}

inline PP_Var PP_MakeNull() {
  PP_Var result = { PP_VarType_Null, {0} };
  return result;
}

inline PP_Var PP_MakeBool(bool value) {
  PP_Var result = { PP_VarType_Bool, {0} };
  result.value.as_bool = value;
  return result;
}

inline PP_Var PP_MakeInt32(int32_t value) {
  PP_Var result = { PP_VarType_Int32, {0} };
  result.value.as_int = value;
  return result;
}

inline PP_Var PP_MakeDouble(double value) {
  PP_Var result = { PP_VarType_Double, {0} };
  result.value.as_double = value;
  return result;
}

#endif  // PPAPI_C_PP_VAR_H_

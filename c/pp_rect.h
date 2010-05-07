// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_RECT_H_
#define PPAPI_C_PP_RECT_H_

#include "ppapi/c/pp_stdint.h"

typedef struct _pp_Rect {
  int32_t left;
  int32_t top;
  int32_t right;
  int32_t bottom;
} PP_Rect;

#endif  // PPAPI_C_PP_RECT_H_

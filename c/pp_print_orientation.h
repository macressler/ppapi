// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_PRINT_ORIENTATION_H_
#define PPAPI_C_PP_PRINT_ORIENTATION_H_

typedef enum _pp_PrintOrientation {
  PP_PrintOrientation_Normal         = 0,
  PP_PrintOrientation_Rotated_90_CW  = 1,
  PP_PrintOrientation_Rotated_180    = 2,
  PP_PrintOrientation_Rotated_90_CCW = 3,
} PP_PrintOrientation;

#endif  // PPAPI_C_PP_PRINT_ORIENTATION_H_


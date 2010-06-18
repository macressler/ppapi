// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_PRINT_SETTINGS_H_
#define PPAPI_C_PP_PRINT_SETTINGS_H_

#include "ppapi/c/pp_print_orientation.h"
#include "ppapi/c/pp_print_output_format.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_PrintSettings {
  // This is the size of the printable area in points (1/72 of an inch)
  PP_Rect printable_area;
  int32_t dpi;
  PP_PrintOrientation orientation;
  bool grayscale;
  PP_PrintOutputFormat format;
} PP_PrintSettings;

#endif  // PPAPI_C_PP_PRINT_SETTINGS_H_


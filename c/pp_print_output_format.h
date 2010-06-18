// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_PRINT_OUTPUT_FORMAT_H_
#define PPAPI_C_PP_PRINT_OUTPUT_FORMAT_H_

typedef enum _pp_PrintOutputFormat {
  PP_PrintOutputFormat_Raster     = 0,
  PP_PrintOutputFormat_PDF        = 1,
  PP_PrintOutputFormat_PostScript = 2,
} PP_PrintOutputFormat;

#endif  // PPAPI_C_PP_PRINT_OUTPUT_FORMAT_H_


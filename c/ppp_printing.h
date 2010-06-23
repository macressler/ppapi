// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_PRINTING_H_
#define PPAPI_C_PPP_PRINTING_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

typedef enum _pp_PrintOrientation {
  PP_PrintOrientation_Normal         = 0,
  PP_PrintOrientation_Rotated_90_CW  = 1,
  PP_PrintOrientation_Rotated_180    = 2,
  PP_PrintOrientation_Rotated_90_CCW = 3
} PP_PrintOrientation;

typedef enum _pp_PrintOutputFormat {
  PP_PrintOutputFormat_Raster     = 0,
  PP_PrintOutputFormat_PDF        = 1,
  PP_PrintOutputFormat_PostScript = 2
} PP_PrintOutputFormat;

typedef struct _pp_PrintSettings {
  // This is the size of the printable area in points (1/72 of an inch)
  PP_Rect printable_area;
  int32_t dpi;
  PP_PrintOrientation orientation;
  bool grayscale;
  PP_PrintOutputFormat format;
} PP_PrintSettings;

// Interface for the plugin to implement printing.
#define PPP_PRINTING_INTERFACE "PPP_Printing;1"

typedef struct _ppp_Printing {
  // Returns array of supported print output formats. The array is expected to
  // be a static array so the caller does not have to free it.
  // Sets |*format_count| to 0 returns NULL if printing is not supported at all.
  const PP_PrintOutputFormat* (*QuerySupportedFormats)(uint32_t* format_count);

  // Begins a print session with the given print settings. Calls to PrintPage
  // can only be made after a successful call to Begin. Returns the number of
  // pages required for the print output at the given page size (0 indicates
  // a failure).
  int32_t (*Begin)(PP_Instance instance,
                   const PP_PrintSettings* print_settings);

  // Prints the specified page using the specified format. |page_number| is a
  // zero-based index. Returns a resource that represents the printed output.
  // This is a PPB_ImageData resource if the output format is
  // PP_PrintOutputFormat_Raster and a PPB_Blob otherwise. Returns 0 on failure.
  PP_Resource (*PrintPage)(PP_Instance instance, int32_t page_number);

  // Ends the print session. Further calls to PrintPage will fail.
  void (*End)(PP_Instance instance);
} PPP_Printing;

#endif  // PPAPI_C_PPP_PRINTING_H_


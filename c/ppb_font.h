// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_FONT_H_
#define PPAPI_C_PPB_FONT_H_

#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FONT_INTERFACE "PPB_Font;1"

typedef enum _pp_FontCharset {
  PP_FONTCHARSET_ANSI = 0,
  PP_FONTCHARSET_DEFAULT = 1,
  PP_FONTCHARSET_SYMBOL = 2,
  PP_FONTCHARSET_MAC = 77,
  PP_FONTCHARSET_SHIFTJIS = 128,
  PP_FONTCHARSET_HANGUL = 129,
  PP_FONTCHARSET_JOHAB = 130,
  PP_FONTCHARSET_GB2312 =134,
  PP_FONTCHARSET_CHINESEBIG5 = 136,
  PP_FONTCHARSET_GREEK = 161,
  PP_FONTCHARSET_TURKISH = 162,
  PP_FONTCHARSET_VIETNAMESE = 163,
  PP_FONTCHARSET_HEBREW = 177,
  PP_FONTCHARSET_ARABIC = 178,
  PP_FONTCHARSET_BALTIC = 186,
  PP_FONTCHARSET_RUSSIAN = 204,
  PP_FONTCHARSET_THAI = 222,
  PP_FONTCHARSET_EASTEUROPE = 238,
  PP_FONTCHARSET_OEM = 255
} PP_FontCharset;

typedef enum _pp_FontPitch {
  PP_FONTPITCH_DEFAULT = 0,
  PP_FONTPITCH_FIXED = 1
} PP_FontPitch;

typedef enum _pp_FontFamily {
  PP_FONTFAMILY_DEFAULT = 0,
  PP_FONTFAMILY_ROMAN = 1,
  PP_FONTFAMILY_SCRIPT = 2
} PP_FontFamily;

typedef struct _pp_FontDescription {
  const char* face;
  uint32_t weight;
  bool italic;
  PP_FontPitch pitch;
  PP_FontFamily family;
  PP_FontCharset charset;
} PP_FontDescription;

typedef struct _ppb_Font {
  // Returns a font which best matches the given description.  The return value
  // will have a non-zero ID on success, or zero on failure.
  PP_Resource (*MatchFontWithFallback)(PP_Module module,
                                       const PP_FontDescription* description);

  // Returns true if the given resource is a Font. Returns false if the
  // resource is invalid or some type other than a Font.
  bool (*IsFont)(PP_Resource resource);

  // Loads a specified font table for the given font.
  //   table: the table in *big-endian* format, or 0 for the whole font file.
  //   output: a buffer of size output_length that gets the data.  can be 0, in
  //     which case output_length will be set to the required size in bytes.
  //   output_length: size of output, if it's not 0.
  // Returns true on success, false on failure.
  bool (*GetFontTable)(PP_Resource font,
                       uint32_t table,
                       void* output,
                       uint32_t* output_length);
} PPB_Font;

#endif  // PPAPI_C_PPB_FONT_H_

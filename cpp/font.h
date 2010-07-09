// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FONT_H_
#define PPAPI_CPP_FONT_H_

#include "ppapi/c/ppb_font.h"
#include "ppapi/cpp/resource.h"

typedef struct _pp_FontDescription PP_FontDescription;

namespace pp {

class Instance;

// Provides access to system fonts.
class Font : public Resource {
 public:
  // Creates an is_null() Font object.
  Font() {}

  explicit Font(PP_Resource resource);
  // Constructs a font from a description.  If a font couldn't be found, is_null
  // will be true.
  explicit Font(const PP_FontDescription* description);
  Font(const Font& other);

  Font& operator=(const Font& other);
  void swap(Font& other);

  // PPB_Font methods:
  bool GetFontTable(uint32_t table,
                    void* output,
                    uint32_t* output_length);
};

}  // namespace pp

#endif  // PPAPI_CPP_FONT_H_

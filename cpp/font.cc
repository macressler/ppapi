// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/font.h"

#include "ppapi/cpp/module.h"

namespace pp {

static PPB_Font const* font_funcs = NULL;

static bool EnsureFuncs() {
  if (!font_funcs) {
    font_funcs = reinterpret_cast<PPB_Font const*>(
        Module::Get()->GetBrowserInterface(PPB_FONT_INTERFACE));
    if (!font_funcs)
      return false;
  }
  return true;
}

Font::Font(PP_Resource resource) : Resource(resource) {
}

Font::Font(const PP_FontDescription* description) {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(font_funcs->MatchFontWithFallback(
      Module::Get()->pp_module(), description));
}

Font::Font(const Font& other)
    : Resource(other) {
}

Font& Font::operator=(const Font& other) {
  Font copy(other);
  swap(copy);
  return *this;
}

void Font::swap(Font& other) {
  Resource::swap(other);
}

bool Font::GetFontTable(uint32_t table,
                        void* output,
                        uint32_t* output_length) {
  if (!EnsureFuncs())
    return false;
  return font_funcs->GetFontTable(pp_resource(), table, output, output_length);
}

}  // namespace pp

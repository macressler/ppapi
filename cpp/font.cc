// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/font.h"

#include <algorithm>

#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/point.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_Font> font_f(PPB_FONT_INTERFACE);

}  // namespace

namespace pp {

// FontDescription -------------------------------------------------------------

FontDescription::FontDescription() {
  pp_font_description_.face = face_.pp_var();
  set_family(PP_FONTFAMILY_DEFAULT);
  set_size(0);
  set_weight(PP_FONTWEIGHT_NORMAL);
  set_italic(false);
  set_small_caps(false);
  set_letter_spacing(0);
  set_word_spacing(0);
}

FontDescription::FontDescription(const FontDescription& other) {
  set_face(other.face());
  set_family(other.family());
  set_size(other.size());
  set_weight(other.weight());
  set_italic(other.italic());
  set_small_caps(other.small_caps());
  set_letter_spacing(other.letter_spacing());
  set_word_spacing(other.word_spacing());
}

FontDescription::~FontDescription() {
}

FontDescription& FontDescription::operator=(const FontDescription& other) {
  FontDescription copy(other);
  swap(copy);
  return *this;
}

void FontDescription::swap(FontDescription& other) {
  // Need to fix up both the face and the pp_font_description_.face which the
  // setter does for us.
  Var temp = face();
  set_face(other.face());
  other.set_face(temp);

  std::swap(pp_font_description_.family, other.pp_font_description_.family);
  std::swap(pp_font_description_.size, other.pp_font_description_.size);
  std::swap(pp_font_description_.weight, other.pp_font_description_.weight);
  std::swap(pp_font_description_.italic, other.pp_font_description_.italic);
  std::swap(pp_font_description_.small_caps,
            other.pp_font_description_.small_caps);
  std::swap(pp_font_description_.letter_spacing,
            other.pp_font_description_.letter_spacing);
  std::swap(pp_font_description_.word_spacing,
            other.pp_font_description_.word_spacing);
}

// TextRun ---------------------------------------------------------------------

TextRun::TextRun() {
  pp_text_run_.text = text_.pp_var();
  pp_text_run_.rtl = false;
  pp_text_run_.override_direction = false;
}

TextRun::TextRun(const std::string& text,
                 bool rtl,
                 bool override_direction)
    : text_(text) {
  pp_text_run_.text = text_.pp_var();
  pp_text_run_.rtl = rtl;
  pp_text_run_.override_direction = override_direction;
}

TextRun::TextRun(const TextRun& other)
    : text_(other.text_) {
  pp_text_run_.text = text_.pp_var();
  pp_text_run_.rtl = other.pp_text_run_.rtl;
  pp_text_run_.override_direction = other.pp_text_run_.override_direction;
}

TextRun::~TextRun() {
}

TextRun& TextRun::operator=(const TextRun& other) {
  TextRun copy(other);
  swap(copy);
  return *this;
}

void TextRun::swap(TextRun& other) {
  text_.swap(other.text_);

  // Fix up both object's pp_text_run.text to point to their text_ member.
  pp_text_run_.text = text_.pp_var();
  other.pp_text_run_.text = other.text_.pp_var();

  std::swap(pp_text_run_.rtl, other.pp_text_run_.rtl);
  std::swap(pp_text_run_.override_direction,
            other.pp_text_run_.override_direction);
}

// Font ------------------------------------------------------------------------

Font::Font(PP_Resource resource) : Resource(resource) {
}

Font::Font(const FontDescription& description) {
  if (!font_f)
    return;
  PassRefFromConstructor(font_f->Create(
      Module::Get()->pp_module(), &description.pp_font_description()));
}

Font::Font(const Font& other) : Resource(other) {
}

Font& Font::operator=(const Font& other) {
  Font copy(other);
  swap(copy);
  return *this;
}

void Font::swap(Font& other) {
  Resource::swap(other);
}

bool Font::Describe(FontDescription* description,
                    PP_FontMetrics* metrics) const {
  if (!font_f)
    return false;

  // Be careful with ownership of the |face| string. It will come back with
  // a ref of 1, which we want to assign to the |face_| member of the C++ class.
  if (!font_f->Describe(pp_resource(), &description->pp_font_description_,
                        metrics))
    return false;
  description->face_ = Var(Var::PassRef(),
                           description->pp_font_description_.face);

  return true;
}

bool Font::DrawTextAt(ImageData* dest,
                      const TextRun& text,
                      const Point& position,
                      uint32_t color,
                      const Rect& clip,
                      bool image_data_is_opaque) const {
  if (!font_f)
    return false;
  return font_f->DrawTextAt(pp_resource(), dest->pp_resource(),
                            &text.pp_text_run(), &position.pp_point(),
                            color, &clip.pp_rect(), image_data_is_opaque);
}

int32_t Font::MeasureText(const TextRun& text) const {
  if (!font_f)
    return -1;
  return font_f->MeasureText(pp_resource(), &text.pp_text_run());
}

uint32_t Font::CharacterOffsetForPixel(const TextRun& text,
                                       int32_t pixel_position) const {
  if (!font_f)
    return 0;
  return font_f->CharacterOffsetForPixel(pp_resource(), &text.pp_text_run(),
                                         pixel_position);

}

int32_t Font::PixelOffsetForCharacter(const TextRun& text,
                                      uint32_t char_offset) const {
  if (!font_f)
    return 0;
  return font_f->PixelOffsetForCharacter(pp_resource(), &text.pp_text_run(),
                                         char_offset);
}

bool Font::DrawSimpleText(ImageData* dest,
                          const std::string& text,
                          const Point& position,
                          uint32_t color,
                          bool image_data_is_opaque) const {
  return DrawTextAt(dest, TextRun(text), position, color,
                    Rect(dest->size()), image_data_is_opaque);
}

int32_t Font::MeasureSimpleText(const std::string& text) const {
  return MeasureText(TextRun(text));
}

}  // namespace pp

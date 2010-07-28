// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_FONT_H_
#define PPAPI_CPP_FONT_H_

#include <string>

#include "ppapi/c/ppb_font.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"

typedef struct _pp_FontDescription PP_FontDescription;

namespace pp {

class Font;
class ImageData;
class Instance;
class Point;
class Rect;

// FontDescription -------------------------------------------------------------

class FontDescription {
 public:
  FontDescription();
  FontDescription(const FontDescription& other);
  ~FontDescription();

  const PP_FontDescription& pp_font_description() const {
    return pp_font_description_;
  }

  FontDescription& operator=(const FontDescription& other);
  void swap(FontDescription& other);

  Var face() const { return face_; }
  void set_face(const Var& face) {
    face_ = face;
    pp_font_description_.face = face_.pp_var();
  }

  PP_FontFamily family() const { return pp_font_description_.family; }
  void set_family(PP_FontFamily f) { pp_font_description_.family = f; }

  float size() const { return pp_font_description_.size; }
  void set_size(float s) { pp_font_description_.size = s; }

  PP_FontWeight weight() const { return pp_font_description_.weight; }
  void set_weight(PP_FontWeight w) { pp_font_description_.weight = w; }

  bool italic() const { return pp_font_description_.italic; }
  void set_italic(bool i) { pp_font_description_.italic = i; }

  bool small_caps() const { return pp_font_description_.small_caps; }
  void set_small_caps(bool s) { pp_font_description_.small_caps = s; }

  int letter_spacing() const { return pp_font_description_.letter_spacing; }
  void set_letter_spacing(int s) { pp_font_description_.letter_spacing = s; }

  int word_spacing() const { return pp_font_description_.word_spacing; }
  void set_word_spacing(int w) { pp_font_description_.word_spacing = w; }

 private:
  friend class Font;

  Var face_;  // Manages memory for pp_font_description_.face
  PP_FontDescription pp_font_description_;
};

// TextRun ---------------------------------------------------------------------

class TextRun {
 public:
  TextRun();
  TextRun(const std::string& text,
          bool rtl = false,
          bool override_direction = false);
  TextRun(const TextRun& other);
  ~TextRun();

  TextRun& operator=(const TextRun& other);
  void swap(TextRun& other);

  const PP_TextRun& pp_text_run() const {
    return pp_text_run_;
  }

 private:
  Var text_;  // Manages memory for the reference in pp_text_run_.
  PP_TextRun pp_text_run_;
};

// Font ------------------------------------------------------------------------

// Provides access to system fonts.
class Font : public Resource {
 public:
  // Creates an is_null() Font object.
  Font() {}

  explicit Font(PP_Resource resource);
  explicit Font(const FontDescription& description);
  Font(const Font& other);

  Font& operator=(const Font& other);
  void swap(Font& other);

  // PPB_Font methods:
  bool Describe(FontDescription* description,
                PP_FontMetrics* metrics);
  bool DrawTextAt(ImageData* dest,
                  const TextRun& text,
                  const Point& position,
                  uint32_t color,
                  const Rect& clip,
                  bool image_data_is_opaque);
  int32_t MeasureText(const TextRun& text);
  uint32_t CharacterOffsetForPixel(const TextRun& text,
                                   int32_t pixel_position);
  int32_t PixelOffsetForCharacter(const TextRun& text,
                                  uint32_t char_offset);

  // Convenience function that assumes a left-to-right string with no clipping.
  bool DrawSimpleText(ImageData* dest,
                      const std::string& text,
                      const Point& position,
                      uint32_t color,
                      bool image_data_is_opaque = false);

  // Convenience function that assumes a left-to-right string.
  int32_t MeasureSimpleText(const std::string& text);
};

}  // namespace pp

#endif  // PPAPI_CPP_FONT_H_

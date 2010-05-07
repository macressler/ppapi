// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_pp_rect_H_
#define PPAPI_CPP_pp_rect_H_

#include "ppapi/c/pp_rect.h"

namespace pp {

class Rect {
 public:
  Rect() {
    pp_rect_.left = 0;
    pp_rect_.top = 0;
    pp_rect_.right = 0;
    pp_rect_.bottom = 0;
  }
  Rect(const PP_Rect& r) : pp_rect_(r) {
  }

  int left() const { return pp_rect_.left; }
  int top() const { return pp_rect_.top; }
  int right() const { return pp_rect_.right; }
  int bottom() const { return pp_rect_.bottom; }

  void set_left(int val) { pp_rect_.left = val; }
  void set_top(int val) { pp_rect_.top = val; }
  void set_right(int val) { pp_rect_.right = val; }
  void set_bottom(int val) { pp_rect_.bottom = val; }

  int width() const { return pp_rect_.right - pp_rect_.left; }
  int height() const { return pp_rect_.bottom - pp_rect_.top; }

  const PP_Rect& pp_rect() const { return pp_rect_; }

 private:
  PP_Rect pp_rect_;
};

}  // namespace pp

#endif  // PPAPI_CPP_pp_rect_H_

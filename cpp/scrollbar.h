// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_SCROLLBAR_H_
#define PPAPI_CPP_SCROLLBAR_H_

#include "ppapi/c/ppb_scrollbar.h"
#include "ppapi/cpp/widget.h"

namespace pp {

class Instance;

// This class allows a plugin to use the browser's scrollbar widget.
class Scrollbar : public Widget {
 public:
  // Creates an is_null() Scrollbar object.
  Scrollbar() {}

  explicit Scrollbar(PP_Resource resource);
  Scrollbar(const Instance& instance, bool vertical);
  Scrollbar(const Scrollbar& other);

  Scrollbar& operator=(const Scrollbar& other);
  void swap(Scrollbar& other);

  // PPB_Scrollbar methods:
  static uint32_t GetThickness();
  uint32_t GetValue();
  void SetValue(uint32_t value);
  void SetDocumentSize(uint32_t size);
  void SetTickMarks(const Rect* tick_marks, uint32_t count);
  void ScrollBy(PP_ScrollBy unit, int32_t multiplier);
};

}  // namespace pp

#endif  // PPAPI_CPP_SCROLLBAR_H_

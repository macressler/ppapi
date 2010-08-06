// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <vector>

#include "ppapi/cpp/scrollbar.h"

#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"
#include "ppapi/cpp/rect.h"

namespace {

DeviceFuncs<PPB_Scrollbar> scrollbar_f(PPB_SCROLLBAR_INTERFACE);

}  // namespace

namespace pp {

Scrollbar::Scrollbar(PP_Resource resource) : Widget(resource) {
}

Scrollbar::Scrollbar(const Instance& instance, bool vertical) {
  if (!scrollbar_f)
    return;
  PassRefFromConstructor(scrollbar_f->Create(instance.pp_instance(), vertical));
}

Scrollbar::Scrollbar(const Scrollbar& other)
    : Widget(other) {
}

Scrollbar& Scrollbar::operator=(const Scrollbar& other) {
  Scrollbar copy(other);
  swap(copy);
  return *this;
}

void Scrollbar::swap(Scrollbar& other) {
  Resource::swap(other);
}

uint32_t Scrollbar::GetThickness() {
  if (!scrollbar_f)
    return 0;
  return scrollbar_f->GetThickness();
}

uint32_t Scrollbar::GetValue() {
  if (!scrollbar_f)
    return 0;
  return scrollbar_f->GetValue(pp_resource());
}

void Scrollbar::SetValue(uint32_t value) {
  if (scrollbar_f)
    scrollbar_f->SetValue(pp_resource(), value);
}

void Scrollbar::SetDocumentSize(uint32_t size) {
  if (scrollbar_f)
    scrollbar_f->SetDocumentSize(pp_resource(), size);
}

void Scrollbar::SetTickMarks(const Rect* tick_marks, uint32_t count) {
  if (!scrollbar_f)
    return;

  std::vector<PP_Rect> temp;
  temp.resize(count);
  for (uint32_t i = 0; i < count; ++i)
    temp[i] = tick_marks[i];

  scrollbar_f->SetTickMarks(pp_resource(), count ? &temp[0] : NULL, count);
}

void Scrollbar::ScrollBy(PP_ScrollBy unit, int32_t multiplier) {
  if (scrollbar_f)
    scrollbar_f->ScrollBy(pp_resource(), unit, multiplier);
}

}  // namespace pp

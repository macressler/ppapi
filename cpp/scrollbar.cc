// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/scrollbar.h"

#include "ppapi/cpp/module.h"

namespace pp {

static PPB_Scrollbar const* scrollbar_funcs = NULL;

static bool EnsureFuncs() {
  if (!scrollbar_funcs) {
    scrollbar_funcs = reinterpret_cast<PPB_Scrollbar const*>(
        Module::Get()->GetBrowserInterface(PPB_SCROLLBAR_INTERFACE));
    if (!scrollbar_funcs)
      return false;
  }
  return true;
}

Scrollbar::Scrollbar(PP_Instance instance, bool vertical) {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(scrollbar_funcs->Create(instance, vertical));
}

uint32_t Scrollbar::GetThickness() {
  if (!EnsureFuncs())
    return 0;
  return scrollbar_funcs->GetThickness();
}

uint32_t Scrollbar::GetValue() {
  if (!EnsureFuncs())
    return 0;
  return scrollbar_funcs->GetValue(pp_resource());
}

void Scrollbar::SetValue(uint32_t value) {
  if (EnsureFuncs())
    scrollbar_funcs->SetValue(pp_resource(), value);
}

void Scrollbar::SetDocumentSize(uint32_t size) {
  if (EnsureFuncs())
    scrollbar_funcs->SetDocumentSize(pp_resource(), size);
}

void Scrollbar::SetTickMarks(const PP_Rect* tick_marks, uint32_t count) {
  if (EnsureFuncs())
    scrollbar_funcs->SetTickMarks(pp_resource(), tick_marks, count);
}

void Scrollbar::ScrollBy(PP_ScrollBy unit, int32_t multiplier) {
  if (EnsureFuncs())
    scrollbar_funcs->ScrollBy(pp_resource(), unit, multiplier);
}

}  // namespace pp

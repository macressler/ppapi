// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/widget.h"

#include "ppapi/c/ppb_widget.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_Widget const* widget_funcs = NULL;

static bool EnsureFuncs() {
  if (!widget_funcs) {
    widget_funcs = reinterpret_cast<PPB_Widget const*>(
        Module::Get()->GetBrowserInterface(PPB_WIDGET_INTERFACE));
    if (!widget_funcs)
      return false;
  }
  return true;
}

Widget::Widget() {
}

bool Widget::Paint(const PP_Rect& rect, ImageData* image) {
  if (!EnsureFuncs())
    return false;

  return widget_funcs->Paint(pp_resource(), &rect, image->pp_resource());
}

bool Widget::HandleEvent(const PP_Event& event) {
  if (!EnsureFuncs())
    return false;

  return widget_funcs->HandleEvent(pp_resource(), &event);
}

bool Widget::GetLocation(PP_Rect* location) {
  if (!EnsureFuncs())
    return false;

  return widget_funcs->GetLocation(pp_resource(), location);
}

void Widget::SetLocation(const PP_Rect& location) {
  if (EnsureFuncs())
    widget_funcs->SetLocation(pp_resource(), &location);
}

}  // namespace pp

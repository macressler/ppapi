// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/widget.h"

#include "ppapi/c/ppb_widget.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_Widget> widget_f(PPB_WIDGET_INTERFACE);

}  // namespace

namespace pp {

Widget::Widget(PP_Resource resource) : Resource(resource) {
}

Widget::Widget(const Widget& other)
    : Resource(other) {
}

Widget& Widget::operator=(const Widget& other) {
  Widget copy(other);
  swap(copy);
  return *this;
}

void Widget::swap(Widget& other) {
  Resource::swap(other);
}

bool Widget::Paint(const Rect& rect, ImageData* image) {
  if (!widget_f)
    return false;

  return widget_f->Paint(
      pp_resource(), &rect.pp_rect(), image->pp_resource());
}

bool Widget::HandleEvent(const PP_Event& event) {
  if (!widget_f)
    return false;

  return widget_f->HandleEvent(pp_resource(), &event);
}

bool Widget::GetLocation(Rect* location) {
  if (!widget_f)
    return false;

  return widget_f->GetLocation(pp_resource(), &location->pp_rect());
}

void Widget::SetLocation(const Rect& location) {
  if (widget_f)
    widget_f->SetLocation(pp_resource(), &location.pp_rect());
}

}  // namespace pp

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/instance.h"

#include "ppapi/c/ppb_instance.h"
#include "ppapi/c/ppp_printing.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/point.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/scrollbar.h"
#include "ppapi/cpp/var.h"
#include "ppapi/cpp/widget.h"

namespace pp {

namespace {

const PPB_Instance* ppb_instance_funcs = NULL;

bool EnsureFuncs() {
  if (!ppb_instance_funcs) {
    ppb_instance_funcs = reinterpret_cast<PPB_Instance const*>(
        Module::Get()->GetBrowserInterface(PPB_INSTANCE_INTERFACE));
    if (!ppb_instance_funcs)
      return false;
  }
  return true;
}

}  // namespace

Instance::Instance(PP_Instance instance) : pp_instance_(instance) {
}

Instance::~Instance() {
}

bool Instance::Init(uint32_t /*argc*/, const char* /*argn*/[],
                    const char* /*argv*/[]) {
  return true;
}

bool Instance::HandleDocumentLoad(const URLLoader& /*url_loader*/) {
  return false;
}

bool Instance::HandleEvent(const PP_Event& /*event*/) {
  return false;
}

Var Instance::GetInstanceObject() {
  return Var();
}

void Instance::ViewChanged(const pp::Rect& /*position*/,
                           const pp::Rect& /*clip*/) {
}

Var Instance::GetSelectedText(bool /* html */) {
  return Var();
}

int32_t Instance::PrintBegin(const PP_PrintSettings&) {
  return 0;
}

Resource Instance::PrintPages(const PP_PrintPageNumberRange* /*page_ranges*/,
                              uint32_t /*page_range_count*/) {
  return Resource();
}

void Instance::PrintEnd() {
}

void Instance::InvalidateWidget(Widget /* widget */,
                                const Rect& /* dirty_rect */) {
}

void Instance::ScrollbarValueChanged(Scrollbar /* scrollbar */,
                                     uint32_t /* value */) {
}

Var Instance::GetWindowObject() {
  if (!EnsureFuncs())
    return Var();
  return Var(Var::PassRef(),
             ppb_instance_funcs->GetWindowObject(pp_instance()));
}

Var Instance::GetOwnerElementObject() {
  if (!EnsureFuncs())
    return Var();
  return Var(Var::PassRef(),
             ppb_instance_funcs->GetOwnerElementObject(pp_instance()));
}

bool Instance::BindGraphicsDeviceContext(const DeviceContext2D& context) {
  if (!EnsureFuncs())
    return false;
  return ppb_instance_funcs->BindGraphicsDeviceContext(pp_instance(),
                                                       context.pp_resource());
}

bool Instance::IsFullFrame() {
  if (!EnsureFuncs())
    return false;
  return ppb_instance_funcs->IsFullFrame(pp_instance());
}

bool Instance::SetCursor(PP_CursorType type,
                         const ImageData& custom_image,
                         const Point& hot_spot) {
  if (!EnsureFuncs())
    return false;
  return ppb_instance_funcs->SetCursor(
      pp_instance(), type, custom_image.pp_resource(), &hot_spot.pp_point());
}

}  // namespace pp

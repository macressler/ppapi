// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/instance.h"

#include "ppapi/c/ppb_find.h"
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

bool EnsureInstanceFuncs() {
  if (!ppb_instance_funcs) {
    ppb_instance_funcs = reinterpret_cast<PPB_Instance const*>(
        Module::Get()->GetBrowserInterface(PPB_INSTANCE_INTERFACE));
    if (!ppb_instance_funcs)
      return false;
  }
  return true;
}

const PPB_Find* ppb_find_funcs = NULL;

bool EnsureFindFuncs() {
  if (!ppb_find_funcs) {
    ppb_find_funcs = reinterpret_cast<PPB_Find const*>(
        Module::Get()->GetBrowserInterface(PPB_FIND_INTERFACE));
    if (!ppb_find_funcs)
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

PP_PrintOutputFormat* Instance::QuerySupportedPrintOutputFormats(
    uint32_t* format_count) {
  *format_count = 0;
  return NULL;
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

void Instance::Zoom(float /* scale */, bool /* text_only */) {
}

bool Instance::StartFind(const char* /* text */, bool /* case_sensitive */) {
  return false;
}

void Instance::SelectFindResult(bool /* forward */) {
}

void Instance::StopFind() {
}

Var Instance::GetWindowObject() {
  if (!EnsureInstanceFuncs())
    return Var();
  return Var(Var::PassRef(),
             ppb_instance_funcs->GetWindowObject(pp_instance()));
}

Var Instance::GetOwnerElementObject() {
  if (!EnsureInstanceFuncs())
    return Var();
  return Var(Var::PassRef(),
             ppb_instance_funcs->GetOwnerElementObject(pp_instance()));
}

bool Instance::BindGraphicsDeviceContext(const DeviceContext2D& context) {
  if (!EnsureInstanceFuncs())
    return false;
  return ppb_instance_funcs->BindGraphicsDeviceContext(pp_instance(),
                                                       context.pp_resource());
}

bool Instance::IsFullFrame() {
  if (!EnsureInstanceFuncs())
    return false;
  return ppb_instance_funcs->IsFullFrame(pp_instance());
}

bool Instance::SetCursor(PP_CursorType type,
                         const ImageData& custom_image,
                         const Point& hot_spot) {
  if (!EnsureInstanceFuncs())
    return false;
  return ppb_instance_funcs->SetCursor(
      pp_instance(), type, custom_image.pp_resource(), &hot_spot.pp_point());
}

void Instance::NumberOfFindResultsChanged(int32_t total, bool final_result) {
  if (!EnsureFindFuncs())
    return;
  ppb_find_funcs->NumberOfFindResultsChanged(
      pp_instance(), total, final_result);
}

void Instance::SelectedFindResultChanged(int32_t index) {
  if (EnsureFindFuncs())
    ppb_find_funcs->SelectedFindResultChanged(pp_instance(), index);
}

}  // namespace pp

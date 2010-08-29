// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/instance.h"

#include "ppapi/c/dev/ppb_find_dev.h"
#include "ppapi/c/dev/ppp_printing_dev.h"
#include "ppapi/c/ppb_instance.h"
#include "ppapi/cpp/dev/scrollbar_dev.h"
#include "ppapi/cpp/dev/widget_dev.h"
#include "ppapi/cpp/graphics_2d.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"
#include "ppapi/cpp/point.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"

namespace {

DeviceFuncs<PPB_Instance> ppb_instance_f(PPB_INSTANCE_INTERFACE);
DeviceFuncs<PPB_Find_Dev> ppb_find_f(PPB_FIND_DEV_INTERFACE);

}  // namespace

namespace pp {

Instance::Instance(PP_Instance instance) : pp_instance_(instance) {
}

Instance::~Instance() {
}

bool Instance::Init(uint32_t /*argc*/, const char* /*argn*/[],
                    const char* /*argv*/[]) {
  return true;
}

bool Instance::HandleDocumentLoad(const URLLoader_Dev& /*url_loader*/) {
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

PP_PrintOutputFormat_Dev* Instance::QuerySupportedPrintOutputFormats(
    uint32_t* format_count) {
  *format_count = 0;
  return NULL;
}

int32_t Instance::PrintBegin(const PP_PrintSettings_Dev&) {
  return 0;
}

Resource Instance::PrintPages(
    const PP_PrintPageNumberRange_Dev* /*page_ranges*/,
    uint32_t /*page_range_count*/) {
  return Resource();
}

void Instance::PrintEnd() {
}

void Instance::InvalidateWidget(Widget_Dev /* widget */,
                                const Rect& /* dirty_rect */) {
}

void Instance::ScrollbarValueChanged(Scrollbar_Dev /* scrollbar */,
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

void Instance::Graphics3DContextLost() {
}

Var Instance::GetWindowObject() {
  if (!ppb_instance_f)
    return Var();
  return Var(Var::PassRef(), ppb_instance_f->GetWindowObject(pp_instance()));
}

Var Instance::GetOwnerElementObject() {
  if (!ppb_instance_f)
    return Var();
  return Var(Var::PassRef(),
             ppb_instance_f->GetOwnerElementObject(pp_instance()));
}

bool Instance::BindGraphics(const Graphics2D& graphics) {
  if (!ppb_instance_f)
    return false;
  return ppb_instance_f->BindGraphics(pp_instance(), graphics.pp_resource());
}

bool Instance::IsFullFrame() {
  if (!ppb_instance_f)
    return false;
  return ppb_instance_f->IsFullFrame(pp_instance());
}


void Instance::NumberOfFindResultsChanged(int32_t total, bool final_result) {
  if (!ppb_find_f)
    return;
  ppb_find_f->NumberOfFindResultsChanged(pp_instance(), total, final_result);
}

void Instance::SelectedFindResultChanged(int32_t index) {
  if (ppb_find_f)
    ppb_find_f->SelectedFindResultChanged(pp_instance(), index);
}

}  // namespace pp

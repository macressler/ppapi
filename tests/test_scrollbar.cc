// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_scrollbar.h"

#include "ppapi/c/pp_event.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/scrollbar.h"
#include "ppapi/tests/test_instance.h"

REGISTER_TEST_CASE(Scrollbar);

TestScrollbar::TestScrollbar(TestInstance* instance)
    : TestCase(instance),
      scrollbar_id_(0),
      scrollbar_value_changed_(false) {
}

void TestScrollbar::RunTest() {
  instance_->LogTest("HandleEvent", TestHandleEvent());
}

void TestScrollbar::ScrollbarValueChanged(pp::Scrollbar* scrollbar,
                                          uint32_t value) {
  if (scrollbar->pp_resource() == scrollbar_id_)
    scrollbar_value_changed_ = true;
}

std::string TestScrollbar::TestHandleEvent() {
  pp::Scrollbar scrollbar(*instance_, true);
  scrollbar_id_ = scrollbar.pp_resource();

  PP_Rect location;
  location.size.width = location.size.height  = 1000;
  scrollbar.SetLocation(location);

  scrollbar.SetDocumentSize(10000);

  PP_Event event;
  event.size = sizeof(event);
  event.type = PP_Event_Type_KeyDown;
  event.u.key.normalizedKeyCode = 0x28; // VKEY_DOWN
  scrollbar.HandleEvent(event);

  return scrollbar_value_changed_ ?
      "" : "Didn't get callback for scrollbar value change";
}

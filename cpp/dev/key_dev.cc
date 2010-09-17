// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/dev/key_dev.h"

#include "ppapi/c/dev/ppb_key_dev.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

namespace pp {

namespace {

DeviceFuncs<PPB_Key_Dev> ppb_key_f(PPB_KEY_DEV_INTERFACE);

}  // namespace

uint8_t Key_Dev::GetKeyState(int32_t key_code) {
  if (ppb_key_f) {
	  return ppb_key_f->GetKeyState(associated_instance_->pp_instance(),
                                    key_code);
  }
  return 0;
}

}  // namespace pp

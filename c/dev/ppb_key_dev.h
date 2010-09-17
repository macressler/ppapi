// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_DEV_PPB_KEY_DEV_H_
#define PPAPI_C_DEV_PPB_KEY_DEV_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_KEY_DEV_INTERFACE "PPB_Key(Dev);1"

struct PPB_Key_Dev {
  // Returns the status of the specified key.
  uint8_t (*GetKeyState)(PP_Instance instance, int32_t key_code);
};

#endif  //  PPAPI_C_DEV_PPB_KEY_DEV_H_

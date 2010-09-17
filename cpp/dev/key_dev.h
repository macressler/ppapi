// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_KEY_DEV_H_
#define PPAPI_CPP_DEV_KEY_DEV_H_

#include <string>

#include "ppapi/c/dev/ppb_key_dev.h"

namespace pp {

class Instance;

// This class allows you to associate the PPB_Key_Dev C-based
// interfaces with an object. It associates itself with the given instance.
//
// You would typically use this either via inheritance on your instance:
//   class MyInstance : public pp::Instance, public pp::Key_Dev {
//     class MyInstance() : pp::Key_Dev(this) {
//     }
//     ...
//   };
//
// or by composition:
//   class MyKey : public pp::Key {
//     ...
//   };
//
//   class MyInstance : public pp::Instance {
//     MyInstance() : key_(this) {
//     }
//
//     MyKey key_;
//   };
class Key_Dev {
 public:
  Key_Dev(Instance* instance) : associated_instance_(instance) {}
  virtual ~Key_Dev() {}

  // PPB_Key_Dev functions for you to call to report key state.
  uint8_t GetKeyState(int32_t key_code);

 private:
  Instance* associated_instance_;
};

}  // namespace pp

#endif  // PPAPI_CPP_DEV_KEY_DEV_H_

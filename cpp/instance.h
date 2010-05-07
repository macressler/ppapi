// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_INSTANCE_H_
#define PPAPI_CPP_INSTANCE_H_

#include "ppapi/c/pp_instance.h"

typedef struct _pp_Event PP_Event;

namespace pp {

class DeviceContext2D;
class Rect;
class Var;

class Instance {
 public:
  explicit Instance(PP_Instance instance);
  virtual ~Instance();

  PP_Instance pp_instance() const { return pp_instance_; }

  // Initializes this plugin with the given arguments. The argument count is in
  // |argc|, the argument names are in |argn|, and the argument values are in
  // |argv|.
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

  virtual bool HandleEvent(const PP_Event& event) {}
  virtual Var GetInstanceObject();
  virtual void ViewChanged(const Rect& position, const Rect& clip) {}

  bool BindGraphicsDeviceContext(const DeviceContext2D& context);
  //bool BindGraphicsDeviceContext(const DeviceContext3D& context);

 private:
  PP_Instance pp_instance_;
};

}  // namespace pp

#endif  // PPAPI_CPP_INSTANCE_H_

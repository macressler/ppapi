// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_INSTANCE_H_
#define PPAPI_CPP_INSTANCE_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_Event PP_Event;
typedef struct _pp_PrintPageNumberRange PP_PrintPageNumberRange;
typedef struct _pp_PrintSettings PP_PrintSettings;
typedef struct _pp_Rect PP_Rect;

namespace pp {

class DeviceContext2D;
class Var;
class Resource;
class URLLoader;

class Instance {
 public:
  explicit Instance(PP_Instance instance);
  virtual ~Instance();

  PP_Instance pp_instance() const { return pp_instance_; }

  // Initializes this plugin with the given arguments. The argument count is in
  // |argc|, the argument names are in |argn|, and the argument values are in
  // |argv|.
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

  virtual bool HandleDocumentLoad(const URLLoader&) {
    return false;
  }
  virtual bool HandleEvent(const PP_Event&) {
    return false;
  }
  virtual Var GetInstanceObject();
  virtual void ViewChanged(const PP_Rect& position, const PP_Rect& clip) {
  }

  // Print interfaces.
  virtual int32_t PrintBegin(const PP_PrintSettings&) {
    return 0;
  }
  virtual Resource PrintPages(const PP_PrintPageNumberRange* page_ranges,
                              uint32_t page_range_count);
  virtual void PrintEnd() {}

  Var GetWindowObject();
  Var GetOwnerElementObject();
  bool BindGraphicsDeviceContext(const DeviceContext2D& context);
  //bool BindGraphicsDeviceContext(const DeviceContext3D& context);
  bool IsFullFrame();

 private:
  PP_Instance pp_instance_;
};

}  // namespace pp

#endif  // PPAPI_CPP_INSTANCE_H_

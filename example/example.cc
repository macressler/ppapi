// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>  // FIXME(brettw) eraseme.

#include "ppapi/c/pp_event.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/rect.h"

class MyInstance : public pp::Instance {
 public:
  MyInstance(PP_Instance instance)
      : pp::Instance(instance),
        width_(0),
        height_(0) {}
  virtual ~MyInstance() {}

  virtual bool Init(size_t argc, const char* argn[], const char* argv[]) {
    return true;
  }

  virtual bool HandleEvent(const PP_Event& event) {
    switch (event.type) {
      case PP_Event_Type_MouseDown:
        printf("Mouse down\n");
        return true;
      case PP_Event_Type_MouseMove:
        printf("Mouse move\n");
        return true;
      case PP_Event_Type_KeyDown:
        printf("Key down\n");
        return true;
      default:
        return false;
    }
  }

  virtual void ViewChanged(const pp::Rect& position, const pp::Rect& clip) {
    printf("ViewChanged %d,%d,%d,%d\n", position.left(), position.top(),
           position.right(), position.bottom());
    if (position.width() == width_ || position.height() == height_)
      return;  // We don't care about the position, only the size.

    width_ = position.width();
    height_ = position.height();

    device_context_ = pp::DeviceContext2D(width_, height_);
    if (!BindGraphicsDeviceContext(device_context_)) {
      printf("Couldn't bind the device context\n");
      return;
    }

    pp::ImageData image(PP_IMAGEDATAFORMAT_BGRA_PREMUL, width_, height_);
    if (image.is_null()) {
      printf("Couldn't allocate the image data\n");
      return;
    }

    // Fill with semitransparent blue so we know it worked.
    for (int y = 0; y < image.height(); y++) {
      char* row = &static_cast<char*>(image.data())[y * image.stride()];
      for (int x = 0; x < image.width(); x++) {
        row[x * 4 + 0] = y;
        row[x * 4 + 1] = y;
        row[x * 4 + 2] = y;
        row[x * 4 + 3] = y;
      }
    }

    device_context_.PaintImageData(image, 0, 0, pp::Rect(), NULL, NULL);
  }

 private:
  pp::DeviceContext2D device_context_;

  int width_;
  int height_;
};

class MyModule : public pp::Module {
 public:
  MyModule() : pp::Module() {}
  virtual ~MyModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new MyInstance(instance);
  }
};

namespace pp {

Module* CreateModule() {
  return new MyModule();
}

}  // namespace pp

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/completion_callback.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/font.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/rect.h"
#include "ppapi/cpp/size.h"

static void DummyCompletionCallback(void* /*user_data*/, int32_t /*result*/) {
}

class MyInstance : public pp::Instance {
 public:
  MyInstance(PP_Instance instance)
      : pp::Instance(instance) {
  }

  virtual ~MyInstance() {
  }

  virtual bool Init(size_t argc, const char* argn[], const char* argv[]) {
    return true;
  }

  virtual void ViewChanged(const pp::Rect& position, const pp::Rect& clip) {
    if (position.size() == last_size_)
      return;
    last_size_ = position.size();

    pp::ImageData image(PP_IMAGEDATAFORMAT_BGRA_PREMUL, last_size_, true);
    pp::DeviceContext2D device(last_size_, false);
    BindGraphicsDeviceContext(device);

    pp::FontDescription desc;
    desc.set_family(PP_FONTFAMILY_SANSSERIF);
    desc.set_size(30);
    pp::Font font(desc);

    pp::Rect text_clip(position.size());  // Use entire bounds for clip.
    font.DrawTextAt(&image,
        pp::TextRun("\xD9\x85\xD8\xB1\xD8\xAD\xD8\xA8\xD8\xA7\xE2\x80\x8E",
                    true, true),
        pp::Point(10, 40), 0xFF008000, clip, false);
    font.DrawTextAt(&image, pp::TextRun("Hello"),
        pp::Point(10, 80), 0xFF000080, text_clip, false);

    device.PaintImageData(image, pp::Point(0, 0));
    device.Flush(pp::CompletionCallback(&DummyCompletionCallback, NULL));
  }

 private:
  pp::Size last_size_;
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

// Factory function for your specialization of the Module object.
Module* CreateModule() {
  return new MyModule();
}

}  // namespace pp

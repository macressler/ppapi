// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <stdio.h>  // FIXME(brettw) eraseme.

#include "ppapi/c/pp_event.h"
#include "ppapi/c/pp_rect.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/scriptable_object.h"
#include "ppapi/cpp/var.h"

class MyScriptableObject : public pp::ScriptableObject {
 public:
  virtual bool HasMethod(const pp::Var& method, pp::Var* exception) {
    return method.AsString() == "toString";
  }
  
  virtual bool HasProperty(const pp::Var& name, pp::Var* exception) {
    if (name.is_string() && name.AsString() == "blah")
      return true;
    return false;
  }

  virtual pp::Var GetProperty(const pp::Var& name, pp::Var* exception) {
    if (name.is_string() && name.AsString() == "blah")
      return new MyScriptableObject();
    return pp::Var();
  }
  
  virtual void GetAllPropertyNames(std::vector<pp::Var>* names,
                                   pp::Var* exception) {
    names->push_back("blah");
  }
 
  virtual pp::Var Call(const pp::Var& method,
                       const std::vector<pp::Var>& args,
                       pp::Var* exception) {
    if (method.AsString() == "toString")
      return pp::Var("hello world");
    return pp::Var();
  }
};

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
        SayHello();
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
  
  virtual pp::Var GetInstanceObject() {
    return new MyScriptableObject();
  }

  virtual void ViewChanged(const PP_Rect& position, const PP_Rect& clip) {
    printf("ViewChanged %d,%d,%d,%d\n", position.point.x, position.point.y,
           position.size.width, position.size.height);
    if (position.size.width == width_ || position.size.height == height_)
      return;  // We don't care about the position, only the size.

    width_ = position.size.width;
    height_ = position.size.height;

    device_context_ = pp::DeviceContext2D(width_, height_, false);
    if (!BindGraphicsDeviceContext(device_context_)) {
      printf("Couldn't bind the device context\n");
      return;
    }

    pp::ImageData image(PP_IMAGEDATAFORMAT_BGRA_PREMUL, width_, height_, false);
    if (image.is_null()) {
      printf("Couldn't allocate the image data\n");
      return;
    }

    // Fill with semitransparent gradient.
    for (int y = 0; y < image.height(); y++) {
      char* row = &static_cast<char*>(image.data())[y * image.stride()];
      for (int x = 0; x < image.width(); x++) {
        row[x * 4 + 0] = y;
        row[x * 4 + 1] = y;
        row[x * 4 + 2] = 0;
        row[x * 4 + 3] = y;
      }
    }

    // Either of these calls is OK in this context, Swap is slightly more
    // efficient since it avoids the copy.
    device_context_.ReplaceContents(&image);
    //device_context_.PaintImageData(image, 0, 0, NULL);
    device_context_.Flush(NULL, NULL);
  }

 private:
  void Log(const pp::Var& var) {
    pp::Var doc = GetWindowObject().GetProperty("document");
    if (console_.is_void()) {
      pp::Var body = doc.GetProperty("body");
      console_ = doc.Call("createElement", "pre");
      console_.GetProperty("style").SetProperty("backgroundColor", "lightgray");
      body.Call("appendChild", console_);
    }
    console_.Call("appendChild", doc.Call("createTextNode", var));
    console_.Call("appendChild", doc.Call("createTextNode", "\n"));    
  }
 
  void SayHello() {
    pp::Var window = GetWindowObject();
    pp::Var doc = window.GetProperty("document");
    pp::Var body = doc.GetProperty("body");

    pp::Var obj(new MyScriptableObject());

    // Our object should have its toString method called.
    Log("Testing MyScriptableObject::toString():");
    Log(obj);

    // body.appendChild(body) should throw an exception
    Log("\nCalling body.appendChild(body):");
    pp::Var exception;
    body.Call("appendChild", body, &exception);
    Log(exception);

    Log("\nEnumeration of window properties:");
    std::vector<pp::Var> props;
    window.GetAllPropertyNames(&props);
    for (size_t i = 0; i < props.size(); ++i)
      Log(props[i]);
  }

  pp::Var console_; 
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

// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_INSTANCE_H_
#define PPAPI_CPP_INSTANCE_H_

#include <map>
#include <string>

#include "ppapi/c/dev/ppp_printing_dev.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

struct PP_Event;

namespace pp {

class Graphics2D;
class ImageData;
class Point;
class Rect;
class Rect;
class Resource;
class Scrollbar_Dev;
class URLLoader_Dev;
class Var;
class Widget_Dev;

class Instance {
 public:
  explicit Instance(PP_Instance instance);
  virtual ~Instance();

  PP_Instance pp_instance() const { return pp_instance_; }

  // Initializes this plugin with the given arguments. The argument count is in
  // |argc|, the argument names are in |argn|, and the argument values are in
  // |argv|. Returns true on success. Returning false will cause the plugin
  // instance to be deleted and no other functions will be called.
  virtual bool Init(uint32_t argc, const char* argn[], const char* argv[]);

  // PPP_Instance methods for the plugin to override.
  // See ppp_instance.h for details.
  virtual bool HandleDocumentLoad(const URLLoader_Dev& url_loader);
  virtual bool HandleEvent(const PP_Event& event);
  virtual Var GetInstanceObject();
  virtual void ViewChanged(const Rect& position, const Rect& clip);
  virtual Var GetSelectedText(bool html);

  // PPP_Printing methods for the plugin to override if it supports printing.
  // See ppp_printing.h for details.
  // TODO(brettw) http://crbug.com/53718 don't expose this here.
  virtual PP_PrintOutputFormat_Dev* QuerySupportedPrintOutputFormats(
      uint32_t* format_count);
  virtual int32_t PrintBegin(const PP_PrintSettings_Dev& print_settings);
  virtual Resource PrintPages(const PP_PrintPageNumberRange_Dev* page_ranges,
                              uint32_t page_range_count);
  virtual void PrintEnd();

  // Widget interface.
  // TODO(brettw) http://crbug.com/53718 don't expose this here.
  virtual void InvalidateWidget(Widget_Dev widget, const Rect& dirty_rect);

  // Scrollbar interface.
  // TODO(brettw) http://crbug.com/53718 don't expose this here.
  virtual void ScrollbarValueChanged(Scrollbar_Dev scrollbar, uint32_t value);

  // Zoom interface.
  // TODO(brettw) http://crbug.com/53718 don't expose this here.
  virtual void Zoom(float scale, bool text_only);

  // Find interface.
  // TODO(brettw) http://crbug.com/53718 don't expose this here.
  virtual bool StartFind(const char* text, bool case_sensitive);
  virtual void SelectFindResult(bool forward);
  virtual void StopFind();

  // Graphics3D interface.
  // TODO(brettw) http://crbug.com/53718 don't expose this here.
  virtual void Graphics3DContextLost();

  // PPB_Instance methods for querying the browser.
  // See ppb_instance.h for details.
  Var GetWindowObject();
  Var GetOwnerElementObject();
  bool BindGraphics(const Graphics2D& graphics);
  bool IsFullFrame();

  // Many optional interfaces are associated with a plugin instance. For
  // example, the find in PPP_Find interface receives updates on a per-instance
  // basis. This "per-instance" tracking allows such objects to associate
  // themselves with an instance as "the" handler for that interface name.
  //
  // In the case of the find example, the find object registers with its
  // associated instance in its constructor and unregisters in its destructor.
  // Then whenever it gets updates with a PP_Instance parameter, it can
  // map back to the find object corresponding to that given PP_Instance by
  // calling GetPerInstanceObject.
  //
  // This lookup is done on a per-interface-name basis. This means you can
  // only have one object of a given interface name associated with an
  // instance.
  //
  // If you are adding a handler for an additional interface, be sure to
  // register with the module (AddPluginInterface) for your interface name to
  // get the C calls in the first place.
  void AddPerInstanceObject(const std::string& interface_name, void* object);
  void RemovePerInstanceObject(const std::string& interface_name, void* object);

  // See comments for Add/RemovePerInstanceObject. This function is used to
  // lookup an object previously associated with an instance. Returns NULL
  // if the instance is invalid or there is no object for the given interface
  // name on the instance.
  static void* GetPerInstanceObject(PP_Instance instance,
                                    const std::string& interface_name);

 private:
  PP_Instance pp_instance_;

  typedef std::map<std::string, void*> InterfaceNameToObjectMap;
  InterfaceNameToObjectMap interface_name_to_objects_;
};

}  // namespace pp

#endif  // PPAPI_CPP_INSTANCE_H_

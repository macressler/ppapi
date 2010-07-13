// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_INSTANCE_H_
#define PPAPI_CPP_INSTANCE_H_

#include "ppapi/c/pp_cursor_type.h"
#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_Event PP_Event;
typedef struct _pp_PrintPageNumberRange PP_PrintPageNumberRange;
typedef struct _pp_PrintSettings PP_PrintSettings;

namespace pp {

class DeviceContext2D;
class ImageData;
class Point;
class Rect;
class Rect;
class Resource;
class Scrollbar;
class URLLoader;
class Var;
class Widget;

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
  virtual bool HandleDocumentLoad(const URLLoader& url_loader);
  virtual bool HandleEvent(const PP_Event& event);
  virtual Var GetInstanceObject();
  virtual void ViewChanged(const Rect& position, const Rect& clip);
  virtual Var GetSelectedText(bool html);

  // PPP_Printing methods for the plugin to override if it supports printing.
  // See ppp_printing.h for details.
  virtual int32_t PrintBegin(const PP_PrintSettings& print_settings);
  virtual Resource PrintPages(const PP_PrintPageNumberRange* page_ranges,
                              uint32_t page_range_count);
  virtual void PrintEnd();

  // Widget interface.
  virtual void InvalidateWidget(Widget widget, const Rect& dirty_rect);

  // Scrollbar interface.
  virtual void ScrollbarValueChanged(Scrollbar scrollbar, uint32_t value);

  // Zoom interface.
  virtual void Zoom(float scale, bool text_only);

  // Find interface.
  virtual bool StartFind(const char* text, bool case_sensitive);
  virtual void SelectFindResult(bool forward);
  virtual void StopFind();

  // PPB_Instance methods for querying the browser.
  // See ppb_instance.h for details.
  Var GetWindowObject();
  Var GetOwnerElementObject();
  bool BindGraphicsDeviceContext(const DeviceContext2D& context);
  bool IsFullFrame();
  bool SetCursor(PP_CursorType type,
                 const ImageData& custom_image,
                 const Point& hot_spot);

  // PPB_Find methods
  void NumberOfFindResultsChanged(int32_t total, bool final_result);
  void SelectedFindResultChanged(int32_t index);

 private:
  PP_Instance pp_instance_;
};

}  // namespace pp

#endif  // PPAPI_CPP_INSTANCE_H_

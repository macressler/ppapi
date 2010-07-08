// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_WIDGET_H_
#define PPAPI_C_PPB_WIDGET_H_

#include "ppapi/c/pp_resource.h"

typedef struct _pp_Rect PP_Rect;
typedef struct _pp_Event PP_Event;

#define PPB_WIDGET_INTERFACE "PPB_Widget;1"

// The interface for reusing browser widgets.
typedef struct _ppb_Widget {
  // Returns true if the given resource is a Widget. Returns false if the
  // resource is invalid or some type other than an Widget.
  bool (*IsWidget)(PP_Resource resource);

  // Paint the given rectangle of the widget into the given image.
  // Returns true on success, false on failure
  bool (*Paint)(PP_Resource widget,
                const PP_Rect* rect,
                PP_Resource image);

  // Pass in an event to a widget.  It'll return true if the event was consumed.
  bool (*HandleEvent)(PP_Resource widget,
                      const PP_Event* event);

  // Get/set the location of the widget.
  bool (*GetLocation)(PP_Resource widget,
                      PP_Rect* location);

  void (*SetLocation)(PP_Resource widget,
                      const PP_Rect* location);
} PPB_Widget;

#endif  // PPAPI_C_PPB_WIDGET_H_

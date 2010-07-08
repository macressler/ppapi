// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_SCROLLBAR_H_
#define PPAPI_C_PPB_SCROLLBAR_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"

typedef struct _pp_Rect PP_Rect;

enum PP_ScrollBy {
  PP_WIDGET_SCROLL_BY_PIXEL = 0,
  PP_WIDGET_SCROLL_BY_LINE = 1,
  PP_WIDGET_SCROLL_BY_PAGE = 2,
  PP_WIDGET_SCROLL_BY_DOCUMENT = 3
};

#define PPB_SCROLLBAR_INTERFACE "PPB_Scrollbar;1"

// The interface for a scrollbar.  A scrollbar is a widget, so the functions
// in PPB_Widget can also be used with scrollbar objects.
typedef struct _ppb_Scrollbar {
  // Create a new scrollbar.  Returns 0 if the instance is invalid.
  PP_Resource (*Create)(PP_Instance instance,
                        bool vertical);

  // Returns true if the given resource is a Scrollbar. Returns false if the
  // resource is invalid or some type other than a scrollbar.
  bool (*IsScrollbar)(PP_Resource resource);

  // Gets the thickness of a scrollbar.
  uint32_t (*GetThickness)();

  // Get/set the value of the scrollbar.
  uint32_t (*GetValue)(PP_Resource resource);

  void (*SetValue)(
      PP_Resource resource,
      uint32_t value);

  // Set the document size (i.e. total length of the region that's being
  // scrolled).
  void (*SetDocumentSize)(
      PP_Resource resource,
      uint32_t size);

  // Updates the tickmarks.  Only valid for vertical scrollbars.  "tick_marks"
  // contains "count" PP_Rect objects.
  void (*SetTickMarks)(
      PP_Resource resource,
      const PP_Rect* tick_marks,
      uint32_t count);

  // Scroll by "multiplier" pixels/lines/pages units.  Positive values are
  // forward and negative are backward.  If "unit" is document then any positive
  // value goes to the end while any negative value goes to the beginning.
  void (*ScrollBy)(
      PP_Resource resource,
      PP_ScrollBy unit,
      int32_t multiplier);
} PPB_Scrollbar;

#endif  // PPAPI_C_PPB_SCROLLBAR_H_

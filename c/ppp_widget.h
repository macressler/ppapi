// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_WIDGET_H_
#define PPAPI_C_PPP_WIDGET_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_rect.h"

// Interface for the plugin to implement when using a widget.
#define PPP_WIDGET_INTERFACE "PPP_Widget;1"

typedef struct _ppp_Widget {
  // Informs the instance that the given rectangle needs to be repainted.
  void (*Invalidate)(PP_Instance instance,
                     PP_Resource widget,
                     const PP_Rect* dirty_rect);
} PPP_Widget;

#endif  // PPAPI_C_PPP_WIDGET_H_

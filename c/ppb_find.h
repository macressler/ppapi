// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_FIND_H_
#define PPAPI_C_PPB_FIND_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_FIND_INTERFACE "PPB_Find;1"

typedef struct _ppb_Find {
  // Updates the number of find results for the current search term.  If
  // there are no matches 0 should be passed in.  Only when the plugin has
  // finished searching should it pass in the final count with finalResult set
  // to true.
  void (*NumberOfFindResultsChanged)(PP_Instance instance,
                                     int32_t total,
                                     bool final_result);

  // Updates the index of the currently selected search item.
  void (*SelectedFindResultChanged)(PP_Instance instance,
                                    int32_t index);

} PPB_Find;

#endif  // PPAPI_C_PPB_FIND_H_

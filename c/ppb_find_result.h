// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_FIND_RESULT_H_
#define PPAPI_C_PPB_FIND_RESULT_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"

#define PPB_FINDRESULT_INTERFACE "PPB_FindResult;1"

typedef struct _ppb_FindResult {
  // Creates a find result box associated with a particular instance, so that
  // it may be positioned on the screen relative to the tab containing the
  // instance.  Returns 0 if passed an invalid instance.
  PP_Resource (*Create)(PP_Instance instance);

  // Updates the number of find results for the current search term.  If
  // there are no matches 0 should be passed in.  Only when the plugin has
  // finished searching should it pass in the final count with finalResult set
  // to true.
  void NumberOfFindResultsChanged(PP_Resource resource,
                                  int32_t total,
                                  bool final_result);

  // Updates the index of the currently selected search item. */
  void SelectedFindResultChanged(PP_Resource resource,
                                 int32_t index);

} PPB_FindResult;

#endif  // PPAPI_C_PPB_FIND_RESULT_H_

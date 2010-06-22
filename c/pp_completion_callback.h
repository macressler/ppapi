// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_COMPLETION_CALLBACK_H_
#define PPAPI_C_PP_COMPLETION_CALLBACK_H_

#include "ppapi/c/pp_stdint.h"

// Any method that takes a PP_CompletionCallback has the option of completing
// asynchronously if the operation would block.  Such a method should return
// PP_Error_WouldBlock to indicate when the method will complete
// asynchronously.  If the completion callback is NULL, then the operation will
// block if necessary to complete its work.  PP_BlockUntilComplete() provides a
// convenient way to specify blocking behavior.
//
// The result parameter passes an int32_t that if negative indicates an error
// code.  Otherwise the result value indicates success.  If it is a positive
// value then it may carry additional information.
typedef struct _pp_CompletionCallback {
  void (*Run)(void* user_data, int32_t result);
  void* user_data;
} PP_CompletionCallback;

inline PP_CompletionCallback PP_BlockUntilComplete() {
  PP_CompletionCallback cc = {0};
  return cc;
};

#endif  // PPAPI_C_PP_COMPLETION_CALLBACK_H_

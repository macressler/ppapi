// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_ERRORS_H_
#define PPAPI_C_PP_ERRORS_H_

// Errors are negative valued.
enum {
  PP_OK = 0,

  // Returned by a function, taking a PP_CompletionCallback, that cannot
  // complete synchronously.  This return value indicates that the given
  // callback will be asynchronously notified of the final result once it is
  // available.
  PP_Error_WouldBlock = -1,

  // Indicates failure for unspecified reasons.
  PP_Error_Failed = -2,

  // Indicates failure due to an asynchronous operation being interupted,
  // typically as a result of user action.
  PP_Error_Aborted = -3,

  // Indicates failure due to an invalid argument.
  PP_Error_BadArgument = -4,
 
  // Indicates failure due to an invalid PP_Resource.
  PP_Error_BadResource = -5,

  // Indicates failure due to an unavailable PPAPI interface.
  PP_Error_NoInterface = -6,

  // Indicates failure due to insufficient privileges.
  PP_Error_NoAccess = -7,

  // Indicates failure due to insufficient memory.
  PP_Error_NoMemory = -8,

  // Indicates failure due to insufficient storage space.
  PP_Error_NoSpace = -9,

  // Indicates failure due to insufficient storage quota.
  PP_Error_NoQuota = -10,
 
  // Indicates failure due to a file that does not exist.
  PP_Error_FileNotFound = -20,

  // Indicates failure due to a file that already exists.
  PP_Error_FileExists = -21,

  // Indicates failure due to a file that is too big.
  PP_Error_FileTooBig = -22,

  // Indicates failure due to a file having been modified unexpectedly.
  PP_Error_FileChanged = -23,
 
  // Indicates failure due to a time limit being exceeded.
  PP_Error_TimedOut = -30,
};

#endif  // PPAPI_C_PP_ERRORS_H_

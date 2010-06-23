// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_URL_LOADER_H_
#define PPAPI_C_PPB_URL_LOADER_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_CompletionCallback PP_CompletionCallback;

#define PPB_URLLOADER_INTERFACE "PPB_URLLoader;1"

// The interface for loading URLs.
//
// Typical steps for loading an URL:
// 1- Create an URLLoader object.
// 2- Create an URLRequestInfo object and set properties on it.
// 3- Call URLLoader's Open method passing the URLRequestInfo.
// 4- When Open completes, call GetResponse and examine the status code.
// 5- If the status code indicates a redirect, call FollowRedirect to continue
//    loading using the new URL.
// 6- If the status code is 200, then call ReadResponseBody to stream the data
//    for the response.
//
// Alternatively, if StreamToFile is specified in the URLRequestInfo, then Open
// will complete once the file is fully downloaded.  The downloaded file may be
// accessed from the URLResponseInfo.
typedef struct _ppb_URLLoader {
  // Create a new URLLoader object.  Returns 0 if the instance is invalid.  The
  // URLLoader is associated with a particular instance, so that any UI dialogs
  // that need to be shown to the user can be positioned relative to the window
  // containing the instance.  It is also important for security reasons to
  // know the origin of the URL request.
  PP_Resource (*Create)(PP_Instance instance);

  // Returns true if the given resource is an URLLoader. Returns false if the
  // resource is invalid or some type other than an URLLoader.
  bool (*IsURLLoader)(PP_Resource resource);

  // Begins loading the URLRequestInfo.  Completes when response headers are
  // received or when an error occurs.  Use the GetResponseInfo method to
  // access the response headers.
  int32_t (*Open)(PP_Resource loader,
                  PP_Resource request_info,
                  PP_CompletionCallback callback);

  // If the current URLResponseInfo object corresponds to a redirect, then call
  // this method to follow the redirect.
  int32_t (*FollowRedirect)(PP_Resource loader,
                            PP_CompletionCallback callback);

  // Returns the current upload progress, which is meaningful after Open has
  // been called, and the request given to Open must have been configured with
  // the ReportUploadProgress property set to true.  Progress only refers to
  // the request body.  This data is only available if the RecordUploadProgress
  // was set to true on the URLRequestInfo.  This method returns false if
  // upload progress is not available.
  bool (*GetUploadProgress)(PP_Resource loader,
                            int64_t* bytes_sent,
                            int64_t* total_bytes_to_be_sent);

  // Returns the current download progress, which is meaningful after Open has
  // been called.  Progress only refers to the response body.  The total bytes
  // to be received may be unknown, in which case -1 is returned.  This method
  // returns false if download progress is not available.
  bool (*GetDownloadProgress)(PP_Resource loader,
                              int64_t* bytes_received,
                              int64_t* total_bytes_to_be_received);

  // Returns the current URLResponseInfo object.
  PP_Resource (*GetResponseInfo)(PP_Resource loader);

  // Call this method to read the response body.  The size of the buffer must
  // be large enough to hold the specified number of bytes to read.  May
  // perform a partial read.  Returns the number of bytes read or an error
  // code.
  int32_t (*ReadResponseBody)(PP_Resource loader,
                              char* buffer,
                              int32_t bytes_to_read,
                              PP_CompletionCallback callback);

  // Cancels any IO that may be pending, and closes the URLLoader object.  Any
  // pending callbacks will still run, reporting PP_Error_Aborted if pending IO
  // was interrupted.  It is NOT valid to call Open again after a call to this
  // method.  Note: If the URLLoader object is destroyed, and it is still open,
  // then it will be implicitly closed, so you are not required to call the
  // Close method.
  void (*Close)(PP_Resource loader);
} PPB_URLLoader;

#endif  // PPAPI_C_PPB_URL_LOADER_H_

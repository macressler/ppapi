// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_URL_LOADER_H_
#define PPAPI_CPP_URL_LOADER_H_

#include <stdlib.h>

#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class CompletionCallback;
class URLRequestInfo;
class URLResponseInfo;

class URLLoader : public Resource {
 public:
  URLLoader();
  URLLoader(const URLLoader& other);

  URLLoader& operator=(const URLLoader& other);
  void swap(URLLoader& other);

  // PPB_URLLoader methods:
  int32_t Open(const URLRequestInfo& request_info,
               CompletionCallback* callback = NULL);
  int32_t FollowRedirect(CompletionCallback* callback = NULL);
  bool GetUploadProgress(int64_t* bytes_sent,
                         int64_t* total_bytes_to_be_sent) const;
  bool GetDownloadProgress(int64_t* bytes_received,
                           int64_t* total_bytes_to_be_received) const;
  URLResponseInfo GetResponseInfo() const;
  int32_t ReadResponseBody(char* buffer,
                           int32_t bytes_to_read,
                           CompletionCallback* callback = NULL);
  void Close();
};

}  // namespace pp

#endif  // PPAPI_CPP_URL_LOADER_H_

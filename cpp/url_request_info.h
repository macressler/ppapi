// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_URL_REQUEST_INFO_H_
#define PPAPI_CPP_URL_REQUEST_INFO_H_

#include "ppapi/c/ppb_url_request_info.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/var.h"

namespace pp {

class FileRef;

class URLRequestInfo : public Resource {
 public:
  URLRequestInfo();
  URLRequestInfo(const URLRequestInfo& other);

  URLRequestInfo& operator=(const URLRequestInfo& other);
  void swap(URLRequestInfo& other);

  // PPB_URLRequestInfo methods:
  bool SetProperty(PP_URLRequestProperty property, const Var& value);
  bool AppendDataToBody(const Var& data);
  bool AppendFileToBody(const FileRef& file_ref,
                        PP_Time expected_last_modified_time = 0);
  bool AppendFileRangeToBody(const FileRef& file_ref,
                             int64_t start_offset,
                             int64_t length,
                             PP_Time expected_last_modified_time = 0);

  // Convenient helpers for setting properties:
  bool SetURL(const Var& url_string) {
    return SetProperty(PP_URLRequestProperty_URL, url_string);
  }
  bool SetMethod(const Var& method_string) {
    return SetProperty(PP_URLRequestProperty_Method, method_string);
  }
  bool SetHeaders(const Var& headers_string) {
    return SetProperty(PP_URLRequestProperty_Headers, headers_string);
  }
  bool SetStreamToFile(bool enable) {
    return SetProperty(PP_URLRequestProperty_StreamToFile, enable);
  }
  bool SetFollowRedirects(bool enable) {
    return SetProperty(PP_URLRequestProperty_FollowRedirects, enable);
  }
  bool SetRecordUploadProgress(bool enable) {
    return SetProperty(PP_URLRequestProperty_RecordUploadProgress, enable);
  }
  bool SetAllowAuthentication(bool allow) {
    return SetProperty(PP_URLRequestProperty_AllowAuthentication, allow);
  }
  bool SetAllowCookies(bool allow) {
    return SetProperty(PP_URLRequestProperty_AllowCookies, allow);
  }
  bool SetAllowReferrer(bool allow) {
    return SetProperty(PP_URLRequestProperty_AllowReferrer, allow);
  }
};

}  // namespace pp

#endif  // PPAPI_CPP_URL_REQUEST_INFO_H_

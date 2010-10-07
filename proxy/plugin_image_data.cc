// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_image_data.h"

#include <stdio.h>
#include <string.h>

#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/c/ppb_image_data.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

namespace ppapi_proxy {

namespace {
PP_ImageDataFormat GetNativeImageDataFormat() {
  return PP_IMAGEDATAFORMAT_BGRA_PREMUL;
}

bool IsImageDataFormatSupported(PP_ImageDataFormat format) {
  return format == PP_IMAGEDATAFORMAT_BGRA_PREMUL;
}

PP_Resource Create(PP_Module module,
                   PP_ImageDataFormat format,
                   const struct PP_Size* size,
                   bool init_to_zero) {
  UNREFERENCED_PARAMETER(module);
  UNREFERENCED_PARAMETER(format);
  UNREFERENCED_PARAMETER(size);
  UNREFERENCED_PARAMETER(init_to_zero);
  return kInvalidResourceId;
}

bool IsImageData(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

bool Describe(PP_Resource resource,
              struct PP_ImageDataDesc* desc) {
  UNREFERENCED_PARAMETER(resource);
  UNREFERENCED_PARAMETER(desc);
  return false;
}

void* Map(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return NULL;
}

void Unmap(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
}
}  // namespace

const PPB_ImageData* PluginImageData::GetInterface() {
  static const PPB_ImageData intf = {
    GetNativeImageDataFormat,
    IsImageDataFormatSupported,
    Create,
    IsImageData,
    Describe,
    Map,
    Unmap,
  };
  return &intf;
}

}  // namespace ppapi_proxy

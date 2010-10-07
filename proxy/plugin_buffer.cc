// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_buffer.h"

#include <stdio.h>
#include <string.h>

#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/c/dev/ppb_buffer_dev.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

namespace ppapi_proxy {

namespace {
PP_Resource Create(PP_Module module, int32_t size_in_bytes) {
  UNREFERENCED_PARAMETER(module);
  UNREFERENCED_PARAMETER(size_in_bytes);
  return kInvalidResourceId;
}

bool IsBuffer(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

bool Describe(PP_Resource resource, int32_t* size_in_bytes) {
  UNREFERENCED_PARAMETER(resource);
  UNREFERENCED_PARAMETER(size_in_bytes);
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

const PPB_Buffer_Dev* PluginBuffer::GetInterface() {
  static const PPB_Buffer_Dev intf = {
    Create,
    IsBuffer,
    Describe,
    Map,
    Unmap,
  };
  return &intf;
}

}  // namespace ppapi_proxy

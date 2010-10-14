// Copyright (c) 2010 The Native Client Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/proxy/plugin_audio_config.h"

#include <stdio.h>
#include <string.h>

#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#include "ppapi/c/dev/ppb_audio_config_dev.h"
#include "ppapi/proxy/generated/ppb_rpc_client.h"
#include "ppapi/proxy/plugin_globals.h"
#include "ppapi/proxy/utility.h"

namespace ppapi_proxy {

namespace {
PP_Resource CreateStereo16Bit(PP_Module module,
                              PP_AudioSampleRate_Dev sample_rate,
                              uint32_t sample_frame_count) {
  UNREFERENCED_PARAMETER(module);
  UNREFERENCED_PARAMETER(sample_rate);
  return kInvalidResourceId;
}

uint32_t RecommendSampleFrameCount(uint32_t requested_sample_frame_count) {
  UNREFERENCED_PARAMETER(requested_sample_frame_count);
  return 0;
}

bool IsAudioConfig(PP_Resource resource) {
  UNREFERENCED_PARAMETER(resource);
  return false;
}

PP_AudioSampleRate_Dev GetSampleRate(PP_Resource config) {
  UNREFERENCED_PARAMETER(config);
  return PP_AUDIOSAMPLERATE_NONE;
}

uint32_t GetSampleFrameCount(PP_Resource config) {
  UNREFERENCED_PARAMETER(config);
  return PP_AUDIOMINSAMPLEFRAMECOUNT;
}
}  // namespace

const PPB_AudioConfig_Dev* PluginAudioConfig::GetInterface() {
  static const PPB_AudioConfig_Dev intf = {
    CreateStereo16Bit,
    RecommendSampleFrameCount,
    IsAudioConfig,
    GetSampleRate,
    GetSampleFrameCount,
  };
  return &intf;
}

}  // namespace ppapi_proxy

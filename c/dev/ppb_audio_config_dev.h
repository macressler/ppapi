// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_DEV_PPB_AUDIO_CONFIG_DEV_H_
#define PPAPI_C_DEV_PPB_AUDIO_CONFIG_DEV_H_

#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_AUDIO_CONFIG_DEV_INTERFACE "PPB_AudioConfig(Dev);1"

enum {
  PP_AUDIOMINSAMPLEFRAMECOUNT = 64,
  PP_AUDIOMAXSAMPLEFRAMECOUNT = 32768
};

enum {
  PP_AUDIOSAMELERATE_44100 = 44100,
  PP_AUDIOSAMELERATE_48000 = 48000
};

// Audio configuration. This base configuration interface supports only stereo
// 16bit output. This class is not mutable, therefore it is okay to access
// instances from different threads.
struct PPB_AudioConfig_Dev {
  // Create a 16 bit stereo config with the given sample rate. We guarantee that
  // PP_AUDIOSAMELERATE_44100 and PP_AUDIOSAMELERATE_48000 sample rates are
  // supported. Sample counts between PP_AUDIOMINSAMPLEFRAMECOUNT and
  // PP_AUDIOMAXSAMPLEFRAMECOUNT are guaranteed to work. Sample count of 0
  // will yield a reasonable default sample frame count. The sample frame count
  // will determine sample buffer size and overall latency. A single sample
  // frame on a stereo device means one value for the left channel and one value
  // for the right channel.
  //
  // Buffer layout for a stereo int16 configuration:
  // int16_t *buffer16;
  // buffer16[0] is the first left channel sample
  // buffer16[1] is the first right channel sample
  // buffer16[2] is the second left channel sample
  // buffer16[3] is the second right channel sample
  // ...
  // buffer16[2 * (sample_frame_count - 1)] is the last left channel sample
  // buffer16[2 * (sample_frame_count - 1) + 1] is the last right channel sample
  // Data will always be in the native endian format of the platform.
  PP_Resource (*CreateStereo16Bit)(PP_Module module, uint32_t sample_rate,
                                   uint32_t sample_frame_count);

  // Getters for sample rate & frame count.
  uint32_t (*GetSampleRate)(PP_Resource config);
  uint32_t (*GetSampleFrameCount)(PP_Resource config);
};

#endif  // PPAPI_C_DEV_PPB_AUDIO_CONFIG_DEV_H_


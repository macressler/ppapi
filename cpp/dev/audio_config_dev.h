// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEV_AUDIO_CONFIG_DEV_H_
#define PPAPI_CPP_DEV_AUDIO_CONFIG_DEV_H_

#include "ppapi/c/dev/ppb_audio_config_dev.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class AudioConfig_Dev : public Resource {
 public:
  AudioConfig_Dev();
  AudioConfig_Dev(PP_AudioSampleRate_Dev sample_rate,
                  uint32_t requested_sample_count,
                  uint32_t *obtained_sample_count);

  PP_AudioSampleRate_Dev sample_rate() const { return sample_rate_; }
  uint32_t sample_frame_count() { return sample_frame_count_; }

 private:
  PP_AudioSampleRate_Dev sample_rate_;
  uint32_t sample_frame_count_;
};

}  // namespace pp

#endif  // PPAPI_CPP_DEV_AUDIO_CONFIG_DEV_H_


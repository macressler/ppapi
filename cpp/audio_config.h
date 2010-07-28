// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_AUDIO_CONFIG_H_
#define PPAPI_CPP_AUDIO_CONFIG_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/ppb_audio_config.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class AudioConfig : public Resource {
 public:
  AudioConfig() : sample_rate_(0), sample_frame_count_(0) {}
  AudioConfig(uint32_t sample_rate, uint32_t sample_count);

  uint32_t sample_rate() const { return sample_rate_; }
  uint32_t sample_frame_count() { return sample_frame_count_; }

 private:
  uint32_t sample_rate_;
  uint32_t sample_frame_count_;
};

}  // namespace pp

#endif  // PPAPI_CPP_AUDIO_CONFIG_H_


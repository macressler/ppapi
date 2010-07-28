// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/audio_config.h"

#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

DeviceFuncs<PPB_AudioConfig> audio_cfg_f(PPB_AUDIO_CONFIG_INTERFACE);

namespace pp {

AudioConfig::AudioConfig(uint32_t sample_rate, uint32_t sample_frame_count)
    : sample_rate_(sample_rate),
      sample_frame_count_(sample_frame_count) {
  if (audio_cfg_f) {
    PassRefFromConstructor(audio_cfg_f->CreateStereo16Bit(
        Module::Get()->pp_module(), sample_rate, sample_frame_count));
  }
}

}  // namespace pp


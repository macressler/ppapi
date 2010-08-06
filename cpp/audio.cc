// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/audio.h"

#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_Audio> audio_f(PPB_AUDIO_INTERFACE);

}  // namespace

namespace pp {

Audio::Audio(Instance instance, AudioConfig config, PPB_Audio_Callback callback,
             void* user_data)
    : config_(config) {
  if (audio_f) {
    PassRefFromConstructor(audio_f->Create(instance.pp_instance(),
                                           config.pp_resource(),
                                           callback, user_data));
  }
}

bool Audio::StartPlayback() {
  return audio_f && audio_f->StartPlayback(pp_resource());
}

bool Audio::StopPlayback() {
  return audio_f && audio_f->StopPlayback(pp_resource());
}

}  // namespace pp


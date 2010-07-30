// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/video_decoder.h"

#include "ppapi/c/pp_video.h"
#include "ppapi/c/ppb_video_decoder.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"

namespace pp {

static PPB_VideoDecoder const* video_decoder_funcs = NULL;

static bool EnsureFuncs() {
  if (!video_decoder_funcs) {
    video_decoder_funcs = reinterpret_cast<PPB_VideoDecoder const*>(
        Module::Get()->GetBrowserInterface(PPB_VIDEODECODER_INTERFACE));
    if (!video_decoder_funcs)
      return false;
  }
  return true;
}

VideoDecoder::VideoDecoder(PP_Resource resource) : Resource(resource) {
}

VideoDecoder::VideoDecoder(const Instance& instance,
                           const PP_VideoDecoderConfig& decoder_config) {
  if (!EnsureFuncs())
    return;
  PassRefFromConstructor(video_decoder_funcs->Create(instance.pp_instance(),
                                                     &decoder_config));
}

VideoDecoder::VideoDecoder(const VideoDecoder& other)
    : Resource(other) {
}

VideoDecoder& VideoDecoder::operator=(const VideoDecoder& other) {
  VideoDecoder copy(other);
  swap(copy);
  return *this;
}

void VideoDecoder::swap(VideoDecoder& other) {
  Resource::swap(other);
}

// static
bool VideoDecoder::GetConfig(const Instance& instance,
                             PP_VideoCodecId codec,
                             PP_VideoConfig* configs,
                             int32_t config_size,
                             int32_t* num_config) {
  if (!EnsureFuncs())
    return false;
  return video_decoder_funcs->GetConfig(instance.pp_instance(),
                                        codec,
                                        configs,
                                        config_size,
                                        num_config);
}

bool VideoDecoder::Decode(PP_VideoCompressedDataBuffer& input_buffer) {
  if (!EnsureFuncs() || !pp_resource())
    return false;
  return video_decoder_funcs->Decode(pp_resource(),
                                     &input_buffer);
}

void VideoDecoder::Flush(PP_CompletionCallback callback) {
  if (!EnsureFuncs() || !pp_resource())
    return;
  return video_decoder_funcs->Flush(pp_resource(), callback);
}

bool VideoDecoder::ReturnUncompressedDataBuffer(
    PP_VideoUncompressedDataBuffer& buffer) {
  if (!EnsureFuncs() || !pp_resource())
    return false;
  return video_decoder_funcs->ReturnUncompressedDataBuffer(pp_resource(),
                                                           &buffer);
}

}  // namespace pp

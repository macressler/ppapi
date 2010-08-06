// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/video_decoder.h"

#include "ppapi/c/pp_errors.h"
#include "ppapi/c/pp_video.h"
#include "ppapi/c/ppb_video_decoder.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/module_impl.h"

namespace {

DeviceFuncs<PPB_VideoDecoder> video_decoder_f(PPB_VIDEODECODER_INTERFACE);

}  // namespace


namespace pp {

VideoDecoder::VideoDecoder(PP_Resource resource) : Resource(resource) {
}

VideoDecoder::VideoDecoder(const Instance& instance,
                           const PP_VideoDecoderConfig& decoder_config) {
  if (!video_decoder_f)
    return;
  PassRefFromConstructor(video_decoder_f->Create(instance.pp_instance(),
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
  if (!video_decoder_f)
    return false;
  return video_decoder_f->GetConfig(instance.pp_instance(),
                                    codec,
                                    configs,
                                    config_size,
                                    num_config);
}

bool VideoDecoder::Decode(PP_VideoCompressedDataBuffer& input_buffer) {
  if (!video_decoder_f || !pp_resource())
    return false;
  return video_decoder_f->Decode(pp_resource(),
                                 &input_buffer);
}

int32_t VideoDecoder::Flush(PP_CompletionCallback callback) {
  if (!video_decoder_f)
    return PP_ERROR_NOINTERFACE;

  return video_decoder_f->Flush(pp_resource(), callback);
}

bool VideoDecoder::ReturnUncompressedDataBuffer(
    PP_VideoUncompressedDataBuffer& buffer) {
  if (!video_decoder_f || !pp_resource())
    return false;
  return video_decoder_f->ReturnUncompressedDataBuffer(pp_resource(),
                                                       &buffer);
}

}  // namespace pp

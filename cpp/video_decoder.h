// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VIDEO_DECODER_H_
#define PPAPI_CPP_VIDEO_DECODER_H_

#include "ppapi/c/pp_video.h"
#include "ppapi/c/ppb_video_decoder.h"
#include "ppapi/cpp/resource.h"

namespace pp {

class Instance;

// Provides access to video decoders.
class VideoDecoder : public Resource {
 public:
  // Creates an is_null() VideoDecoder object.
  VideoDecoder() {}

  explicit VideoDecoder(PP_Resource resource);

  VideoDecoder(const Instance& instance,
               const PP_VideoDecoderConfig& decoder_config);
  VideoDecoder(const VideoDecoder& other);

  VideoDecoder& operator=(const VideoDecoder& other);
  void swap(VideoDecoder& other);

  // PPB_VideoDecoder methods:
  static bool GetConfig(const Instance& instance,
                        PP_VideoCodecId codec,
                        PP_VideoConfig* configs,
                        int32_t config_size,
                        int32_t* num_config);

  bool Decode(PP_VideoCompressedDataBuffer& input_buffer);

  int32_t Flush(PP_CompletionCallback callback);

  bool ReturnUncompressedDataBuffer(PP_VideoUncompressedDataBuffer& buffer);
};

}  // namespace pp

#endif  // PPAPI_CPP_VIDEO_DECODER_H_

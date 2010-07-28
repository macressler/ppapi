// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_VIDEO_DECODER_H_
#define PPAPI_C_PPB_VIDEO_DECODER_H_

#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_video.h"
#include "ppapi/c/pp_completion_callback.h"

#define PPB_VIDEODECODER_INTERFACE "PPB_VideoDecoder;1"

typedef struct _ppb_VideoDecoder {
  // Queries capability of the decoder for |codec|.
  // |codec| is the requested codec id.
  // |configs| is a pointer to a buffer containing |config_size| elements.
  // The number of configurations is returned in |num_config|. Element 0 through
  // |num_config| - 1 of |configs| are filled in with valid PP_VideoConfig's.
  // No more than |config_size| PP_VideoConfig's will be returned even if more
  // are available on the device.
  // When this function is called with |configs| = NULL, then no configurations
  // are returned, but the total number of configurations available will be
  // returned in |num_config|.
  //
  // Returns true on success, false otherwise.
  // NOTE: browser owns the memory of all PP_VideoConfig's.
  bool (*GetConfig)(PP_Instance instance,
                    PP_VideoCodecId codec,
                    PP_VideoConfig* configs,
                    int32_t config_size,
                    int32_t* num_config);

  // Creates a video decoder with requested |decoder_config|.
  // |input_format| in |decoder_config| specifies the format of input access
  // unit, with PP_VIDEOKEY_CODECID and PP_VIDEOKEY_PAYLOADFORMAT required.
  // Plugin has the option to specify codec profile/level and other
  // information such as PP_VIDEOKEY_ACCELERATION, to let browser choose
  // the most appropriate decoder.
  //
  // |output_format| in |decoder_config| specifies desired decoded frame buffer
  // format, with PP_VIDEOKEY_COLORTYPE and PP_VIDEOKEY_SURFACETYPE required.
  //
  // |output_callback| in |decoder_config| specifies the callback function
  // for decoder to deliver decoded frame buffers. Decoder shall retain it.
  //
  // |input_callback| in |decoder_config| specifies the callback function
  // for decoder to return compressed data buffers to plugin. Decoder shall
  // retain it. When plugin doesn't expect buffer recycling, it shall set
  // |input_callback| to NULL. In this case, plugin shall allocate buffer via
  // |MemAlloc| in PPB_Core interface, and decoder will free buffer via
  // |MemFree| in the same API.
  //
  // |event_handler| in |decoder_config| specifies the function for decoder
  // to deliver events to plugin. Decoder shall retain it.
  //
  // The created decoder is returned as PP_Resource. NULL means failure.
  PP_Resource (*Create)(PP_Instance instance,
                        const PP_VideoDecoderConfig* decoder_config);

  // Sends bit stream in |input_buffer| to the decoder. |frame_info| contains
  // associated information about the frame.
  // This is a non-blocking call.
  // The decoded frame will be returned by decoder calling |output_callback|
  // provided by plugin during creation of decoder.
  // The input data buffer is returned to plugin by decoder only when plugin
  // provides |input_callback|.
  // Decoder shall retain frame info and return them to plugin when it returns
  // decoded frame.
  // Returns true on decoder successfully accepting buffer, false otherwise.
  //
  bool (*Decode)(PP_Resource decoder,
                 PP_VideoCompressedDataBuffer* input_buffer,
                 PP_VideoFrameInfo* frame_info);

  // Requests the decoder to flush its input and output buffers. Once done with
  // flushing, the decode will call the |callback|.
  void (*Flush)(PP_Resource decoder,
                PP_CompletionCallback callback);

  // Plugin sends uncompressed data buffers to the decoder.
  // Returns true on decoder successfully accepting the buffer, false otherwise.
  bool (*ReturnUncompressedDataBuffer)(PP_Resource decoder,
                                       PP_VideoUncompressedDataBuffer* buffer);
} PPB_VideoDecoder;

#endif  // PPAPI_C_PPB_VIDEO_DECODER_H_

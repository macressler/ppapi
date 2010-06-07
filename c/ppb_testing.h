// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_TESTING_H_
#define PPAPI_C_PPB_TESTING_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

#define PPB_TESTING_INTERFACE "PPB_Testing;1"

// This interface contains functions used for unit testing. They are not
// guaranteed to be available in normal plugin environments so you should not
// depend on them.
typedef struct _ppb_Testing {
  // Reads the bitmap data out of the backing store for the given
  // DeviceContext2D and into the given image. If the data was successfully
  // read, it will return true.
  //
  // This function should not generally be necessary for normal plugin
  // operation. If you want to update portions of a device, the expectation is
  // that you will either regenerate the data, or maintain a backing store
  // pushing updates to the device from your backing store via PaintImageData.
  // Using this function will introduce an extra copy which will make your
  // plugin slower. In some cases, this may be a very expensive operation (it
  // may require slow cross-process transitions or graphics card readbacks).
  //
  // Data will be read into the image starting at (x, y) in the device context,
  // and proceeding down and to the right for as many pixels as the image is
  // large. If any part of the image bound would fall outside of the backing
  // store of the device if positioned at (x, y), this function will fail and
  // return false.
  //
  // The image format must be of the format
  // PPB_ImageData.GetNativeImageDataFormat() or this function will fail and
  // return false.
  //
  // The returned image data will represent the current status of the backing
  // store. This will not include any paint, scroll, or replace operations
  // that have not yet been flushed; these operations are only reflected in
  // the backing store (and hence ReadImageData) until after a Flush()
  // operation has completed.
  bool (*ReadImageData)(PP_Resource device_context_2d,
                        PP_Resource image,
                        int32_t x, int32_t y);

} PPB_Testing;

#endif  // PPAPI_C_PPB_TESTING_H_

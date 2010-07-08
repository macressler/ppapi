// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_DEVICE_CONTEXT_2D_H_
#define PPAPI_CPP_DEVICE_CONTEXT_2D_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/cpp/resource.h"
#include "ppapi/cpp/size.h"

typedef struct _pp_Rect PP_Rect;

namespace pp {

class CompletionCallback;
class ImageData;

class DeviceContext2D : public Resource {
 public:
  // Creates an is_null() ImageData object.
  DeviceContext2D();

  // The copied device will refer to the original (since this is just a wrapper
  // around a refcounted resource).
  DeviceContext2D(const DeviceContext2D& other);

  // Allocates a new 2D device context with the given size in the browser,
  // resulting object will be is_null() if the allocation failed.
  DeviceContext2D(int32_t width, int32_t height, bool is_always_opaque);

  virtual ~DeviceContext2D();

  DeviceContext2D& operator=(const DeviceContext2D& other);
  void swap(DeviceContext2D& other);

  const Size& size() const { return size_; }
  int32_t width() const { return size_.width(); }
  int32_t height() const { return size_.height(); }

  // Enqueues paint or scroll commands. THIS COMMAND HAS NO EFFECT UNTIL YOU
  // CALL Flush().
  //
  // Please see PPB_DeviceContext2D.PaintImageData / .Scroll for more details.
  bool PaintImageData(const ImageData& image,
                      int32_t x, int32_t y,
                      const PP_Rect* src_rect);
  bool Scroll(const PP_Rect* clip, int32_t dx, int32_t dy);

  // The browser will take ownership of the given image data. The object
  // pointed to by the parameter will be cleared. To avoid horrible artifacts,
  // you should also not use any other ImageData objects referring to the same
  // resource will no longer be usable. THIS COMMAND HAS NO EFFECT UNTIL YOU
  // CALL Flush().
  //
  // Please see PPB_DeviceContext2D.Swap for more details.
  bool ReplaceContents(ImageData* image);

  // Flushes all the currently enqueued Paint, Scroll, and Replace commands.
  // Can be used in synchronous mode (NULL callback pointer) from background
  // threads.
  //
  // Please see PPB_DeviceContext2D.Flush for more details.
  int32_t Flush(const CompletionCallback& cc);

 private:
  Size size_;
};

}  // namespace pp

#endif  // PPAPI_CPP_DEVICE_CONTEXT_2D_H_

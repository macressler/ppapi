// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_DEVICE_CONTEXT_2D_H_
#define PPAPI_C_PPB_DEVICE_CONTEXT_2D_H_

#include "ppapi/c/pp_module.h"
#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_CompletionCallback PP_CompletionCallback;
typedef struct _pp_Point PP_Point;
typedef struct _pp_Rect PP_Rect;
typedef struct _pp_Size PP_Size;

#define PPB_DEVICECONTEXT2D_INTERFACE "PPB_DeviceContext2D;1"

typedef struct _ppb_DeviceContext2D {
  // The returned device context will not be bound to any plugin instance on
  // creation (call BindGraphicsDeviceContext on the plugin instance to do
  // that. The device context has a lifetime that can exceed that of the given
  // plugin instance.
  //
  // Set the is_always_opaque flag if you know that you will be painting only
  // opaque data to this device. This will disable blending when compositing
  // the plugin with the web page, which will give slightly higher performance.
  // If you aren't sure, it is always correct to specify that it it not opaque.
  PP_Resource (*Create)(PP_Module module,
                        const PP_Size* size,
                        bool is_always_opaque);

  // Returns true if the given resource is a valid DeviceContext2D, false if it
  // is an invalid resource or is a resource of another type.
  bool (*IsDeviceContext2D)(PP_Resource resource);

  // Retrieves the configuration for the given device context, filling the
  // given values (which must not be NULL). On success, returns true. If the
  // resource is invalid, the output parameters will be set to 0 and it will
  // return false.
  bool (*Describe)(PP_Resource device_context,
                   PP_Size* size,
                   bool* is_always_opqaue);

  // Enqueues a paint of the given image into the device. THIS HAS NO EFFECT
  // UNTIL YOU CALL Flush(). As a result, what counts is the contents of the
  // bitmap when you call Flush, not when you call this function.
  //
  // The given image will be placed at |top_left| from the top left of the
  // device's internal backing store. Then the src_rect will be copied into the
  // backing store. This parameter may not be NULL.
  //
  // The src_rect is specified in the coordinate system of the image being
  // painted, not the device. For the common case of copying the entire image,
  // you may specify a NULL |src_rect| pointer. If you are frequently updating
  // the entire image, consider using SwapImageData which will give slightly
  // higher performance.
  //
  // The painted area of the source bitmap must fall entirely within the
  // device. Attempting to paint outside of the device will result in an error.
  // However, the source bitmap may fall outside the device, as long as the
  // src_rect subset of it falls entirely within the device.
  //
  // Returns true on success, false on failure. Failure means one of the
  // resources was invalid, or the coordinates were out of bounds.
  bool (*PaintImageData)(PP_Resource device_context,
                         PP_Resource image,
                         const PP_Point* top_left,
                         const PP_Rect* src_rect);

  // Enqueues a scroll of the device's backing store. THIS HAS NO EFFECT UNTIL
  // YOU CALL Flush(). The data within the given clip rect (you may specify
  // NULL to scroll the entire region) will be shifted by (dx, dy) pixels.
  //
  // This will result in some exposed region which will have undefined
  // contents. The plugin should call PaintImageData on these exposed regions
  // to give the correct contents.
  //
  // The scroll can be larger than the area of the clip rect, which means the
  // current image will be scrolled out of the rect. This is not an error but
  // will be a no-op.
  //
  // Returns true on success, false on failure. Failure means one of the
  // devices was invalid, or the clip rect went out of bounds of the device.
  bool (*Scroll)(PP_Resource device_context,
                 const PP_Rect* clip_rect,
                 const PP_Point* amount);

  // This function provides a slightly more efficient way to paint the entire
  // plugin's image. Normally, calling PaintImageData requires that the browser
  // copy the pixels out of the image and into the device context's backing
  // store. This function replaces the device context's backing store with the
  // given image, avoiding the copy.
  //
  // The new image must be the exact same size as this device context and
  // must be in the browser's native bitmap format (use
  // PPB_ImageData.GetNativeImageDataFormat to retrieve this).
  //
  // THE NEW IMAGE WILL NOT BE PAINTED UNTIL YOU CALL FLUSH.
  //
  // After this call, you should take care to release your references to the
  // image. If you paint to the image after a Swap, there is the possibility of
  // significant painting artifacts because the page might use partially-
  // rendered data when copying out of the backing store.
  //
  // In the case of an animation, you will want to allocate a new image for the
  // next frame. It is best if you wait until the flush callback has executed
  // before allocating this bitmap. This gives the browser the option of
  // caching the previous backing store and handing it back to you (assuming
  // the sizes match). In the optimal case, this means no bitmaps are allocated
  // during the animation, and the backing store and "front buffer" (which the
  // plugin is painting into) are just being swapped back and forth.
  //
  // Returns true on success. Failure indicates the device context or image is
  // invalid, or the input image is a different size than the device context.
  // In the failure case, the image will still be valid.
  bool (*ReplaceContents)(PP_Resource device_context, PP_Resource image);

  // Flushes any enqueued paint, scroll, and swap commands for the backing
  // store. This actually executes the updates, and causes a repaint of the
  // webpage, assuming this device context is bound to a plugin instance. This
  // can run in two modes:
  //
  // - In synchronous mode, you specify NULL for the callback and the callback
  //   data. This function will block the calling thread until the image has
  //   been painted to the screen. It is not legal to block the main thread of
  //   the plugin, you can use synchronous mode only from background threads.
  //
  // - In asynchronous mode, you specify a callback function and the argument
  //   for that callback function. The callback function will be executed on
  //   the calling thread when the image has been painted to the screen. While
  //   you are waiting for a Flush callback, additional calls to Flush will
  //   fail.
  //
  // Because the callback is executed (or thread unblocked) only when the
  // plugin's current state is actually on the screen, this function provides a
  // way to rate limit animations. By waiting until the image is on the screen
  // before painting the next frame, you can ensure you're not generating
  // updates faster than the screen can be updated.
  //
  // Unbound devices: If the device is not bound to a plugin instance, you will
  //   still get a callback. It will execute after the Flush function returns
  //   to avoid reentrancy. Of course, it will not wait until anything is
  //   painted to the screen because there will be nothing on the screen. The
  //   timing of this callback is not guaranteed and may be deprioritized by
  //   the browser because it is not affecting the user experience.
  //
  // Off-screen instances: If the device is bound to an instance that is
  //   currently not visible (for example, scrolled out of view) it will behave
  //   like the "unbound device" case.
  //
  // Detaching a device: If you detach a device from a plugin instance, any
  //   pending flush callbacks will be converted into the "unbound device" case.
  //
  // Released devices: A callback may or may not still get called even if you
  //   have released all of your references to the device. This can occur if
  //   there are internal references to the device that means it has not been
  //   internally destroyed (for example, if it is still bound to an instance)
  //   or due to other implementation details. As a result, you should be
  //   careful to check that flush callbacks are for the device you expect and
  //   that you're capable of handling callbacks for devices that you may have
  //   released your reference to.
  //
  // Shutdown: If a plugin instance is removed when a Flush is pending, the
  //   callback will not be executed.
  //
  // TODO(darin): We should ensure that the completion callback always runs, so
  // that it is easier for consumers to manage memory referenced by a callback.
  //
  // Returns PP_OK on success, PP_Error_BadResource if the device context is
  // invalid, PP_Error_BadArgument if the callback is null and Flush is being
  // called from the main thread of the plugin, or PP_Error_InProgress if a
  // Flush is already pending that has not issued its callback yet.  In the
  // failure case, nothing will be updated and no callback will be scheduled.
  int32_t (*Flush)(PP_Resource device_context, PP_CompletionCallback callback);

} PPB_DeviceContext2D;

#endif  // PPAPI_C_PPB_DEVICE_CONTEXT_2D_H_

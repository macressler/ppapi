# Introduction #

Most window systems and previous browser plugin rendering models use an invalidation model where the browser or window system notifies the plugin or application when a region has been exposed. PPAPI uses a retained mode where the browser always maintains an internal backing store for the plugin representing the last painted image.

This mode is important because PPAPI plugins are expected to be run in separate processes from the browser. Retained mode allows the browser to paint parts of the page without requiring locking or cross-process communication, while the plugin is simultaneously generating a new image in the background.

There is no way to access the internal backing store except through the 2D device API. The browser may use the backing store to paint at any time. If the plugin could write directly to the backing store, the browser might paint partial updates which would cause visual artifacts.

# Basic usage overview #

  1. Create a `DeviceContext2D` of the correct size (normally this is done from the `ViewChanged` notification when the size changes.
  1. Bind the device context to the plugin instance.
  1. Create an `ImageData` to hold painted bits.
  1. Write to the `ImageData` yourself.
  1. Call `PaintImageData` or `ReplaceContents` to copy your bits to the backing store.
  1. Call `Flush` to apply the updates.
  1. Wait for the flush callback before painting a new frame.

Note that PPAPI doesn't include a 2D graphics library. The most the API provides is a bitmap in a certain format that the plugin is responsible for writing into. If you want advanced 2D graphics generation, you will need to link a graphics library with your plugin such as [Cairo](http://cairographics.org/) or [Skia](http://code.google.com/p/skia/).

# Device context details #

### Simple operations ###

The most basic operation is `PaintImageData` which copies pixels from an `ImageData` you supply to the backing store. You can update all or part of the plugin image using this function. Note that the pixels are not actually copied until you call `Flush`.

Some plugins may also find the `Scroll` function useful. This offsets the pixels in a given rectangle by a certain amount. Normal usage is that you would subsequently call `PaintImageData` for the newly exposed region, and then `Flush` both changes to the screen at once. Calling the scroll function can be somewhat more efficient than offsetting the pixels yourself and copying because accelerated video hardware can be used to move the pixels.

### Optimized updating of the entire frame ###

Some types of plugins may update the entire plugin area for every frame (for example, a video player). In this case, using `PaintImageData` requires an extra copy to copy the data from the `ImageData` to the backing store of the device.

You can avoid this extra copy by replacing the contents of the backing store with a new `ImageData` object that you supply. After calling `ReplaceContents` the image you supply will be used as the new backing store for the device. The limitations of this approach are that your image must be sized exactly the same as the device's existing backing store, and that the plugin will no longer have access to the bitmap once it is owned by the device.

If you are using `ReplaceContents`, you allocate a new `ImageData` each time you want to paint. The intent is that when you do a replace, the browser can cache the old backing store image and supply it back to you if you request an image of the same size soon after. If you set `init_to_zero` to false when allocating the image, the browser doesn't even need to clear the old image. Painting in this mode is as efficient as a swap operation while giving the browser a little extra flexibility to allocate a new image if the sizes don't match, or to throw away the cached image if memory is low. Since the browser may choose to reallocate the image, you can not count on receiving the old contents of the device in the next image data allocation.

### Flushing ###

None of the update operations take effect until `Flush` is called. This allows the plugin to update discontinuous parts of the image in one "frame" without getting tearing or partial painting artifacts. It also allows a scroll and a paint of the exposed region to be done atomically.

A flush is complete when the bits are sent to the screen. A callback when this flush is complete allows the plugin to rate limit the number of updates it provides to be as fast as it can be copied to the screen. Since the plugin can generate paint messages asynchronously with the browser, if this rate limiting didn't happen, the plugin could get significantly ahead of the browser in processing paints.

<blockquote><i>Advanced note:</i> A complete flush doesn't literally mean that the bits are on the screen, only that they're "on the way." A browser like Chrome can use this to overlap generating of the next frame with the update of the screen, while also preventing the plugin from getting more than one frame behind.</blockquote>

The application can run is synchronous or asynchronous flush mode. Asynchronous flush mode will notify the application that the flush has completed via a callback. Synchronous flush mode will block the caller until the Flush has been completed. Synchronous flush mode can only be used when running on a background thread. If a plugin's main thread could block on a flush call, it could not respond to input events which could cause a deadlock.

### See also ###

  * C API header files for [DeviceContext2D](http://code.google.com/p/ppapi/source/browse/trunk/c/ppb_device_context_2d.h) and [ImageData](http://code.google.com/p/ppapi/source/browse/trunk/c/ppb_image_data.h).
  * C++ API header files for [DeviceContext2D](http://code.google.com/p/ppapi/source/browse/trunk/cpp/device_context_2d.h) and [ImageData](http://code.google.com/p/ppapi/source/browse/trunk/cpp/image_data.h).
  * Simple animation plugin TODO(brettw) write this!

# Managing invalid rects with the `PaintManager` #

Some applications are inconvenient to write using immediate mode painting. In these cases, a better model is to request invalidates for dirty regions of the plugin, and to get a callback when it's a "good time" to paint. This mode is generally what authors of client applications, such as on Microsoft Windows, use.

To support this type of application, the PPAPI C++ wrapper provides a `PaintManager` convenience object. It collects dirty rectangles and scroll requests and supplies an aggregated set of scrolls and paints to actually update the screen as a callback. Typical usage is as follows:

  1. Create a `pp::PaintManager` object associated with your plugin instance.
  1. Add a call to the paint manager when your plugin size changes so it can keep itself in sync.
  1. When your plugin changes, call `InvalidateRect` and `ScrollRect` on the paint manager as needed.
  1. Implement an `OnPaint` handler to process the batched updates.

The paint manager will automatically handle calling `Flush` for you and listening for the flush complete callback before doing another paint. It will also aggregate the dirty rectangles in an intelligent way using the `pp::PaintAggregator` object. In your paint handler, you have the option to either generate a new image for the union of all dirty regions, or to paint the specific dirty regions individually (the aggregator ensures these won't overlap).

### See also ###

  * The header file for [PaintManager](http://code.google.com/p/ppapi/source/browse/trunk/cpp/paint_manager.h).
  * [Example plugin](http://code.google.com/p/ppapi/source/browse/trunk/examples/2d/paint_manager_example.cc) using the paint manager.
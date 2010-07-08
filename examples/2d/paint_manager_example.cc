// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/c/pp_event.h"
#include "ppapi/cpp/device_context_2d.h"
#include "ppapi/cpp/image_data.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/paint_manager.h"

// Number of pixels to each side of the center of the square that we draw.
static const int kSquareRadius = 2;

// We identify our square by the center point. This computes the rect for the
// square given that point.
pp::Rect SquareForPoint(int x, int y) {
  return pp::Rect(x - kSquareRadius, y - kSquareRadius,
                  kSquareRadius * 2 + 1, kSquareRadius * 2 + 1);
}

static void FillRect(pp::ImageData* image,
                     int left, int top, int width, int height,
                     uint32_t color) {
  for (int y = std::max(0, top);
       y < std::min(image->height(), top + height);
       y++) {
    for (int x = std::max(0, left);
         x < std::min(image->width(), left + width);
         x++)
      *image->GetAddr32(x, y) = color;
  }
}

class MyInstance : public pp::Instance, public pp::PaintManager::Client {
 public:
  MyInstance(PP_Instance instance)
      : pp::Instance(instance),
        paint_manager_(),
        last_x_(0),
        last_y_(0) {
    paint_manager_.Initialize(this, this, false);
  }

  virtual ~MyInstance() {
  }

  virtual bool Init(size_t argc, const char* argn[], const char* argv[]) {
    return true;
  }

  virtual bool HandleEvent(const PP_Event& event) {
    switch (event.type) {
      case PP_Event_Type_MouseDown:
        // Update the square on a mouse down.
        if (event.u.mouse.button == PP_Event_MouseButton_Left)
          UpdateSquare(event.u.mouse.x, event.u.mouse.y);
        return true;
      case PP_Event_Type_MouseMove:
        // Update the square on a drag.
        if (event.u.mouse.button == PP_Event_MouseButton_Left)
          UpdateSquare(event.u.mouse.x, event.u.mouse.y);
        return true;
      default:
        return false;
    }
  }

  virtual void ViewChanged(const PP_Rect& position, const PP_Rect& clip) {
    paint_manager_.SetSize(position.size);
  }

  // PaintManager::Client implementation.
  virtual bool OnPaint(pp::DeviceContext2D& device,
                       const pp::PaintUpdate& update) {
    // Although we don't actually do any scroll operations, for completeness
    // of this example, this is how you would apply it.
    if (update.has_scroll) {
      device.Scroll(&update.scroll_rect.pp_rect(),
                    update.scroll_delta.x(), update.scroll_delta.y());
    }

    // Make an image just large enough to hold all dirty rects.
    pp::ImageData updated_image(PP_IMAGEDATAFORMAT_BGRA_PREMUL,
                                update.paint_bounds.width(),
                                update.paint_bounds.height(), false);

    // We repaint everything inside the image we made above. This is just a
    // light blue background.
    //
    // If our per-pixel overhead was large or we had a large plugin and often
    // got small invalidates far apart from each other, this would be
    // inefficient. In that case, it would be more efficient to paint the
    // only the dirty rects into our image, and individually call
    // PaintImageData, selecting out the subsets we actually drew.
    //
    // You could also make separate images for each invalid region, but that
    // would normally be less efficient than allocating one big one and
    // selecting parts to paint due to overhead allocating each bitmap.
    FillRect(&updated_image, 0, 0,
             update.paint_bounds.width(),
             update.paint_bounds.height(),
             0xFFAAAAFF);
             
    // Paint the square black. Since our image is just the invalid region
    // rather than the whole plugin, we need to offset the areas we paint by
    // that much. 
    pp::Rect square = SquareForPoint(last_x_, last_y_);
    FillRect(&updated_image,
             square.x() - update.paint_bounds.x(),
             square.y() - update.paint_bounds.y(),
             square.width(),
             square.height(),
             0xFF000000);

    // Paint the image we just made to the device.
    device.PaintImageData(updated_image,
                          update.paint_bounds.x(),
                          update.paint_bounds.y(),
                          NULL);
    return true;
  }

 private:
  void UpdateSquare(int x, int y) {
    if (x == last_x_ && y == last_y_)
      return;  // Nothing changed.

    // Invalidate the region around the old square which needs to be repainted
    // because it's no longer there.
    paint_manager_.InvalidateRect(SquareForPoint(last_x_, last_y_));

    // Update the current position.
    last_x_ = x;
    last_y_ = y;

    // Also invalidate the region around the new square.
    paint_manager_.InvalidateRect(SquareForPoint(last_x_, last_y_));
  }

  pp::PaintManager paint_manager_;

  int last_x_;
  int last_y_;
};

class MyModule : public pp::Module {
 public:
  MyModule() : pp::Module() {}
  virtual ~MyModule() {}

  virtual pp::Instance* CreateInstance(PP_Instance instance) {
    return new MyInstance(instance);
  }
};

namespace pp {

// Factory function for your specialization of the Module object.
Module* CreateModule() {
  return new MyModule();
}

}  // namespace pp

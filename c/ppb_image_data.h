// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPB_IMAGE_DATA_H_
#define PPAPI_C_PPB_IMAGE_DATA_H_

#include "ppapi/c/pp_resource.h"
#include "ppapi/c/pp_stdint.h"

typedef struct _pp_ImageDataDesc PP_ImageDataDesc;
typedef struct _pp_Module PP_Module;
typedef struct _pp_Rect PP_Rect;
typedef struct _pp_Var PP_Var;

enum PP_ImageDataFormat {
  PP_IMAGEDATAFORMAT_BGRA_PREMUL
};

typedef struct _pp_ImageDataDesc {
  PP_ImageDataFormat format;

  // Size of the bitmap in pixels.
  int32_t width;
  int32_t height;

  // The row width in bytes. This may be different than width * 4 since there
  // may be padding at the end of the lines.
  int32_t stride;
} PP_ImageDataDesc;

#define PPB_IMAGEDATA_INTERFACE "PPB_ImageData;1"

typedef struct _ppb_ImageData {
  // Allocates an image data resource with the given format and size. The
  // return value will have a nonzero ID on success, or zero on failure.
  // Failure means the module handle, image size, or format was invalid.
  //
  // Set the init_to_zero flag if you want the bitmap initialized to
  // transparent during the creation process. If this flag is not set, the
  // current contents of the bitmap will be undefined, and the plugin should
  // be sure to set all the pixels.
  //
  // For security reasons, if uninitialized, the bitmap will not contain random
  // memory, but may contain data from a previous image produced by the same
  // plugin if the bitmap was cached and re-used.
  PP_Resource (*Create)(PP_Module module,
                        PP_ImageDataFormat format,
                        int32_t width,
                        int32_t height,
                        bool init_to_zero);

  // Returns true if the given resource is an image data. Returns false if the
  // resource is invalid or some type other than an image data.
  bool (*IsImageData)(PP_Resource resource);

  // Computes the description of the image data. Returns true on success, false
  // if the resource is not an image data. On false, the |desc| structure will
  // be filled with 0.
  bool (*Describe)(PP_Resource resource,
                   PP_ImageDataDesc* desc);

  // Maps this bitmap into the plugin address space and returns a pointer to the
  // beginning of the data.
  void* (*Map)(PP_Resource resource);

  void (*Unmap)(PP_Resource resource);
} PPB_ImageData;

#endif  // PPAPI_C_PPB_IMAGE_DATA_H_

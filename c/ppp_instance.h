// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PPP_INSTANCE_H_
#define PPAPI_C_PPP_INSTANCE_H_

#include "ppapi/c/pp_instance.h"
#include "ppapi/c/pp_rect.h"

struct PP_InputEvent;
struct PP_Var;

#define PPP_INSTANCE_INTERFACE "PPP_Instance;1"

/**
 * @file
 * Defines the API ...
 *
 * @addtogroup PPP
 * @{
 */

struct PPP_Instance {
  /**
   * You may not call other parts of PPAPI from this function (for example, do
   * not execute JS or modify the DOM. Put such code in InitializeInstance.
   *
   * If this returns false, the plugin will be deleted and the DeleteInstance
   * will NOT be called (since we assume it was not properly initialized).
   */
  bool (*New)(PP_Instance instance);

  /** Called when the plugin instance is destroyed. */
  void (*Delete)(PP_Instance instance);

  /**
   * Called immediately after New. Plugins can do instance-specific
   * initialization that requires other parts of PPAPI.
   *
   * If the plugin reports failure from this function, the plugin to be deleted
   * and DeleteInstance will be called.
   */
  bool (*Initialize)(PP_Instance instance,
                     uint32_t argc,
                     const char* argn[],
                     const char* argv[]);

  /**
   * Called after Initialize for a full-frame plugin that was instantiated
   * based on the MIME type of a DOMWindow navigation.  This only applies to
   * plugins that are registered to handle certain MIME types.  The given
   * url_loader corresponds to a PPB_URLLoader instance that is already opened.
   * Its response headers may be queried using PPB_URLLoader::GetResponseInfo.
   * This method returns false if the plugin cannot handle the data.  In
   * response to this method, the plugin should call ReadResponseBody to read
   * the incoming data.
   */
  bool (*HandleDocumentLoad)(PP_Instance instance, PP_Resource url_loader);

  /**
   * General handler for input events. Returns true if the event was handled or
   * false if it was not.
   *
   * If the event was handled, it will not be forwarded to the web page or
   * browser. If it was not handled, it will bubble according to the normal
   * rules. So it is important that a plugin respond accurately with whether
   * event propogation should continue.
   * 
   * Event propogation also controls focus. If you handle an event like a mouse
   * event, typically your plugin will be given focus. Returning false means
   * that the click will be given to a lower part of the page and the plugin
   * will not receive focus. This allows a plugin to be partially transparent,
   * where clicks on the transparent areas will behave like clicks to the
   * underlying page.
   */
  bool (*HandleInputEvent)(PP_Instance instance,
                           const struct PP_InputEvent* event);

  /**
   * Notification that the given plugin instance has gained or lost focus.
   * Having focus means that keyboard events will be sent to your plugin
   * instance. A plugin's default condition is that it will not have focus.
   *
   * Note: clicks on your plugins will give focus only if you handle the
   * click event. You signal if you handled it by returning true from
   * HandleInputEvent. Otherwise the browser will bubble the event and give
   * focus to the element on the page that actually did end up consuming it.
   * If you're not getting focus, check to make sure you're returning true from
   * the mouse click in HandleInputEvent.
   */
  void (*FocusChanged)(PP_Instance instance, bool has_focus);

  /**
   * Returns a Var representing the instance object to the web page. Normally
   * this will be a PPP_Class object that exposes certain methods the page
   * may want to call.
   *
   * On Failure, the returned var should be a "void" variant.
   *
   * The returned PP_Var should have a reference added for the caller, which
   * will be responsible for Release()ing that reference.
   */
  struct PP_Var (*GetInstanceObject)(PP_Instance instance);

  /**
   * Called when the position, the size, or the clip rect has changed.
   *
   * The |position| is the location on the page of this plugin instance. This is
   * relative to the top left corner of the viewport, which changes as the page
   * is scrolled.
   *
   * The |clip| indicates the visible region of the plugin instance. This is
   * relative to the top left of the plugin's coordinate system (not the page).
   * If the plugin is invisible, the clip rect will be (0, 0, 0, 0).
   */
  void (*ViewChanged)(PP_Instance instance,
                      const struct PP_Rect* position,
                      const struct PP_Rect* clip);

  /**
   * Returns the selection, either as plain text or as html depending on "html".
   * If nothing is selected, or if the given format is unavailable, return a
   * void string.
   */
  struct PP_Var (*GetSelectedText)(PP_Instance instance,
                                   bool html);
};

/**
 * @}
 * End addtogroup PPP
 */
#endif  // PPAPI_C_PPP_INSTANCE_H_

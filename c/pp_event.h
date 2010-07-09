// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_EVENT_H_
#define PPAPI_C_PP_EVENT_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

typedef enum {
  PP_EVENT_MOUSEBUTTON_NONE   = -1,
  PP_EVENT_MOUSEBUTTON_LEFT   = 0,
  PP_EVENT_MOUSEBUTTON_MIDDLE = 1,
  PP_EVENT_MOUSEBUTTON_RIGHT  = 2
} PP_Event_MouseButton;

typedef enum {
  PP_EVENT_TYPE_UNDEFINED  = -1,
  PP_EVENT_TYPE_MOUSEDOWN  = 0,
  PP_EVENT_TYPE_MOUSEUP    = 1,
  PP_EVENT_TYPE_MOUSEMOVE  = 2,
  PP_EVENT_TYPE_MOUSEENTER = 3,
  PP_EVENT_TYPE_MOUSELEAVE = 4,
  PP_EVENT_TYPE_MOUSEWHEEL = 5,
  PP_EVENT_TYPE_RAWKEYDOWN = 6,
  PP_EVENT_TYPE_KEYDOWN    = 7,
  PP_EVENT_TYPE_KEYUP      = 8,
  PP_EVENT_TYPE_CHAR       = 9,
  PP_EVENT_TYPE_FOCUS      = 11,
} PP_Event_Type;

typedef enum {
  PP_EVENT_MODIFIER_SHIFTKEY         = 1 << 0,
  PP_EVENT_MODIFIER_CONTROLKEY       = 1 << 1,
  PP_EVENT_MODIFIER_ALTKEY           = 1 << 2,
  PP_EVENT_MODIFIER_METAKEY          = 1 << 3,
  PP_EVENT_MODIFIER_ISKEYPAD         = 1 << 4,
  PP_EVENT_MODIFIER_ISAUTOREPEAT     = 1 << 5,
  PP_EVENT_MODIFIER_LEFTBUTTONDOWN   = 1 << 6,
  PP_EVENT_MODIFIER_MIDDLEBUTTONDOWN = 1 << 7,
  PP_EVENT_MODIFIER_RIGHTBUTTONDOWN  = 1 << 8
} PP_Event_Modifier;

typedef struct _pp_Event_Key {
  uint32_t modifier;
  uint32_t normalizedKeyCode;
} PP_Event_Key;

typedef struct _pp_Event_Character {
  uint32_t modifier;
  uint16_t text[4];
  uint16_t unmodifiedText[4];
} PP_Event_Character;

typedef struct _pp_Event_Mouse {
  uint32_t modifier;
  int32_t button;
  int32_t x;
  int32_t y;
  int32_t clickCount;
} PP_Event_Mouse;

typedef struct _pp_Event_Wheel {
  uint32_t modifier;
  float deltaX;
  float deltaY;
  float wheelTicksX;
  float wheelTicksY;
  uint32_t scrollByPage;
} PP_Event_Wheel;

typedef struct _pp_Event_Focus {
  int32_t value;
} PP_Event_Focus;

typedef struct _pp_Event {
  uint32_t size;
  int32_t type;
  PP_Time time_stamp_seconds;
  union {
    PP_Event_Key key;
    PP_Event_Character character;
    PP_Event_Mouse mouse;
    PP_Event_Wheel wheel;
    PP_Event_Focus focus;
  } u;
} PP_Event;

#endif  // PPAPI_C_PP_EVENT_H_

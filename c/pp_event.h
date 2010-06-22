// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_C_PP_EVENT_H_
#define PPAPI_C_PP_EVENT_H_

#include "ppapi/c/pp_stdint.h"
#include "ppapi/c/pp_time.h"

typedef enum {
  PP_Event_MouseButton_None   = -1,
  PP_Event_MouseButton_Left   = 0,
  PP_Event_MouseButton_Middle = 1,
  PP_Event_MouseButton_Right  = 2
} PP_Event_MouseButton;

typedef enum {
  PP_Event_Type_Undefined  = -1,
  PP_Event_Type_MouseDown  = 0,
  PP_Event_Type_MouseUp    = 1,
  PP_Event_Type_MouseMove  = 2,
  PP_Event_Type_MouseEnter = 3,
  PP_Event_Type_MouseLeave = 4,
  PP_Event_Type_MouseWheel = 5,
  PP_Event_Type_RawKeyDown = 6,
  PP_Event_Type_KeyDown    = 7,
  PP_Event_Type_KeyUp      = 8,
  PP_Event_Type_Char       = 9,
  PP_Event_Type_Focus      = 11,
} PP_Event_Type;

typedef enum {
  PP_Event_Modifier_ShiftKey         = 1 << 0,
  PP_Event_Modifier_ControlKey       = 1 << 1,
  PP_Event_Modifier_AltKey           = 1 << 2,
  PP_Event_Modifier_MetaKey          = 1 << 3,
  PP_Event_Modifier_IsKeyPad         = 1 << 4,
  PP_Event_Modifier_IsAutoRepeat     = 1 << 5,
  PP_Event_Modifier_LeftButtonDown   = 1 << 6,
  PP_Event_Modifier_MiddleButtonDown = 1 << 7,
  PP_Event_Modifier_RightButtonDown  = 1 << 8
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

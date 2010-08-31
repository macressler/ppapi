// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/tests/test_var.h"

#include "ppapi/c/ppb_var.h"
#include "ppapi/cpp/instance.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/var.h"
#include "ppapi/tests/test_instance.h"

static uint32_t kInvalidLength = static_cast<uint32_t>(-1);

REGISTER_TEST_CASE(Var);

bool TestVar::Init() {
  var_interface_ = reinterpret_cast<PPB_Var const*>(
      pp::Module::Get()->GetBrowserInterface(PPB_VAR_INTERFACE));
  return !!var_interface_;
}

void TestVar::RunTest() {
  instance_->LogTest("InvalidUtf8", TestInvalidUtf8());
  instance_->LogTest("NullInputInUtf8Conversion",
                     TestNullInputInUtf8Conversion());
  instance_->LogTest("ValidUtf8", TestValidUtf8());
  instance_->LogTest("Utf8WithEmbeddedNulls", TestUtf8WithEmbeddedNulls());
  instance_->LogTest("VarToUtf8ForWrongType", TestVarToUtf8ForWrongType());
}

std::string TestVar::TestInvalidUtf8() {
  // utf8じゃない (japanese for "is not utf8") in shift-jis encoding.
  static const char kSjisString[] = "utf8\x82\xb6\x82\xe1\x82\xc8\x82\xa2";
  pp::Var sjis(kSjisString);
  if (!sjis.is_null())
    return "Non-UTF8 string permitted.";

  return "";
}

std::string TestVar::TestNullInputInUtf8Conversion() {
  // This test talks directly to the C interface to access edge cases that
  // cannot be exercised via the C++ interface.
  PP_Var converted_string;

  // 0-length string should not dereference input string, and should produce
  // an empty string.
  converted_string = var_interface_->VarFromUtf8(NULL, 0);
  if (converted_string.type != PP_VARTYPE_STRING) {
    return "Expected 0 length to return empty string.";
  }

  // Now convert it back.
  uint32_t length = kInvalidLength;
  const char* result = NULL;
  result = var_interface_->VarToUtf8(converted_string, &length);
  if (length != 0) {
    return "Expected 0 length string on conversion.";
  }
  if (result == NULL) {
    return "Expected a non-null result for 0-lengthed string from VarToUtf8.";
  }

  return "";
}

std::string TestVar::TestValidUtf8() {
  // From UTF8 string -> PP_Var.
  // Chinese for "I am utf8."
  static const char kValidUtf8[] = "\xe6\x88\x91\xe6\x98\xafutf8.";
  pp::Var converted_string(kValidUtf8);

  if (converted_string.is_null())
    return "Unable to convert valid utf8 to var.";

  // Since we're already here, test PP_Var back to UTF8 string.
  std::string returned_string = converted_string.AsString();

  // We need to check against 1 less than sizeof because the resulting string
  // is technically not NULL terminated by API design.
  if (returned_string.size() != sizeof(kValidUtf8) - 1) {
    return "Unable to convert utf8 string back from var.";
  }
  if (returned_string != kValidUtf8) {
    return "String mismatches on conversion back from PP_Var.";
  }

  return "";
}

std::string TestVar::TestUtf8WithEmbeddedNulls() {
  // From UTF8 string with embedded nulls -> PP_Var.
  // Chinese for "also utf8."
  static const char kUtf8WithEmbededNull[] = "\xe6\xb9\x9f\xe6\x98\xaf\0utf8.";
  std::string orig_string(kUtf8WithEmbededNull,
                          sizeof(kUtf8WithEmbededNull) -1);
  pp::Var converted_string(orig_string);

  if (converted_string.is_null())
    return "Unable to convert utf8 with embedded nulls to var.";

  // Since we're already here, test PP_Var back to UTF8 string.
  std::string returned_string = converted_string.AsString();

  if (returned_string.size() != orig_string.size()) {
    return "Unable to convert utf8 with embedded nulls back from var.";
  }
  if (returned_string != orig_string) {
    return "String mismatches on conversion back from PP_Var.";
  }

  return "";
}

std::string TestVar::TestVarToUtf8ForWrongType() {
  uint32_t length = kInvalidLength;
  const char* result = NULL;
  result = var_interface_->VarToUtf8(PP_MakeVoid(), &length);
  if (length != 0) {
    return "Expected 0 on string conversion from Void var.";
  }
  if (result != NULL) {
    return "Expected NULL on string conversion from Void var.";
  }

  length = kInvalidLength;
  result = NULL;
  result = var_interface_->VarToUtf8(PP_MakeNull(), &length);
  if (length != 0) {
    return "Expected 0 on string conversion from Null var.";
  }
  if (result != NULL) {
    return "Expected NULL on string conversion from Null var.";
  }

  length = kInvalidLength;
  result = NULL;
  result = var_interface_->VarToUtf8(PP_MakeBool(true), &length);
  if (length != 0) {
    return "Expected 0 on string conversion from Bool var.";
  }
  if (result != NULL) {
    return "Expected NULL on string conversion from Bool var.";
  }

  length = kInvalidLength;
  result = NULL;
  result = var_interface_->VarToUtf8(PP_MakeInt32(1), &length);
  if (length != 0) {
    return "Expected 0 on string conversion from Int32 var.";
  }
  if (result != NULL) {
    return "Expected NULL on string conversion from Int32 var.";
  }

  length = kInvalidLength;
  result = NULL;
  result = var_interface_->VarToUtf8(PP_MakeDouble(1.0), &length);
  if (length != 0) {
    return "Expected 0 on string conversion from Double var.";
  }
  if (result != NULL) {
    return "Expected NULL on string conversion from Double var.";
  }

  return "";
}

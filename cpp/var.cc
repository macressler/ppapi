// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/var.h"

#include <string.h>

#include <algorithm>

#include "ppapi/c/pp_var.h"
#include "ppapi/c/ppb_var.h"
#include "ppapi/cpp/logging.h"
#include "ppapi/cpp/module.h"
#include "ppapi/cpp/scriptable_object.h"

namespace pp {

namespace {

static PPB_Var const* ppb_var = NULL;

// Technically you can call AddRef and Release on any Var, but it may involve
// cross-process calls depending on the plugin. This is an optimization so we
// only do refcounting on the necessary objects.
inline bool NeedsRefcounting(const PP_Var& var) {
  return var.type == PP_VarType_String || var.type == PP_VarType_Object;
}

void EnsureInit() {
  // We assume the Var interface is always available.
  if (!ppb_var) {
    ppb_var = reinterpret_cast<PPB_Var const*>(
        Module::Get()->GetBrowserInterface(PPB_VAR_INTERFACE));
  }
}

}  // namespace

Var::Var() {
  var_.type = PP_VarType_Void;
  needs_release_ = false;
}

Var::Var(Null) {
  var_.type = PP_VarType_Null;
  needs_release_ = false;
}

Var::Var(bool b) {
  var_.type = PP_VarType_Bool;
  var_.value.as_bool = b;
  needs_release_ = false;
}

Var::Var(int32_t i) {
  var_.type = PP_VarType_Int32;
  var_.value.as_int = i;
  needs_release_ = false;
}

Var::Var(double d) {
  var_.type = PP_VarType_Double;
  var_.value.as_double = d;
  needs_release_ = false;
}

Var::Var(const char* str) {
  EnsureInit();
  var_ = ppb_var->VarFromUtf8(str, strlen(str));
  needs_release_ = true;
}

Var::Var(const std::string& str) {
  EnsureInit();
  var_ = ppb_var->VarFromUtf8(str.c_str(), str.size());
  needs_release_ = true;
}

Var::Var(ScriptableObject* object) {
  EnsureInit();
  var_ = ppb_var->CreateObject(object->GetClass(), object);
  needs_release_ = true;
}

Var::Var(const Var& other) {
  var_ = other.var_;
  if (NeedsRefcounting(var_)) {
    needs_release_ = true;
    EnsureInit();
    ppb_var->AddRef(var_);
  } else {
    needs_release_ = false;
  }
}

Var::~Var() {
  if (needs_release_) {
    EnsureInit();
    ppb_var->Release(var_);
  }
}

Var& Var::operator=(const Var& other) {
  // Use the copy constructor to make a copy, then swap into that. This makes
  // sure we call the correct init and destruct for everything, without actually
  // needing to write the refcounting code here.
  Var copy(other);
  swap(copy);
  return *this;
}

void Var::swap(Var& other) {
  std::swap(var_, other.var_);
  std::swap(needs_release_, other.needs_release_);
}

bool Var::AsBool() const {
  if (!is_bool()) {
    PP_NOTREACHED();
    return false;
  }
  return var_.value.as_bool;
}

int32_t Var::AsInt() const {
  if (is_int())
    return var_.value.as_int;
  if (is_double())
    return static_cast<int>(var_.value.as_double);
  PP_NOTREACHED();
  return 0;
}

double Var::AsDouble() const {
  if (is_double())
    return var_.value.as_double;
  if (is_int())
    return static_cast<double>(var_.value.as_int);
  PP_NOTREACHED();
  return 0.0;
}

std::string Var::AsString() const {
  if (!is_string()) {
    PP_NOTREACHED();
    return std::string();
  }

  EnsureInit();
  uint32_t len;
  const char* str = ppb_var->VarToUtf8(var_, &len);
  return std::string(str, len);
}

bool Var::HasProperty(const Var& name, Var* exception) const {
  EnsureInit();
  return ppb_var->HasProperty(var_, name.var_, OutException(exception));
}

Var Var::GetProperty(const Var& name, Var* exception) const {
  EnsureInit();
  return Var(PassRef(), ppb_var->GetProperty(var_, name.var_,
                                             OutException(exception)));
}

void Var::GetAllPropertyNames(std::vector<Var>* properties,
                              Var* exception) const {
  EnsureInit();
  PP_Var* props = NULL;
  uint32_t prop_count = 0;
  ppb_var->GetAllPropertyNames(var_, &prop_count, &props,
                               OutException(exception));
  if (!prop_count)
    return;
  properties->resize(prop_count);
  for (uint32_t i = 0; i < prop_count; ++i) {
    Var temp(PassRef(), props[i]);
    (*properties)[i].swap(temp);
  }
  Module::Get()->core().MemFree(props);
}

void Var::SetProperty(const Var& name, const Var& value, Var* exception) {
  EnsureInit();
  ppb_var->SetProperty(var_, name.var_, value.var_, OutException(exception));
}

void Var::RemoveProperty(const Var& name, Var* exception) {
  EnsureInit();
  ppb_var->RemoveProperty(var_, name.var_, OutException(exception));
}

Var Var::Call(const Var& method_name, size_t argc, Var* argv, Var* exception) {
  EnsureInit();
  if (argc > 0) {
    std::vector<PP_Var> args;
    args.reserve(argc);
    for (size_t i = 0; i < argc; i++)
      args.push_back(argv[i].var_);
    return Var(PassRef(), ppb_var->Call(var_, method_name.var_, argc, &args[0],
                                        OutException(exception)));
  } else {
    // Don't try to get the address of a vector if it's empty.
    return Var(PassRef(), ppb_var->Call(var_, method_name.var_, 0, NULL,
                                        OutException(exception)));
  }
}

Var Var::Construct(size_t argc, Var* argv, Var* exception) const {
  EnsureInit();
  if (argc > 0) {
    std::vector<PP_Var> args;
    args.reserve(argc);
    for (size_t i = 0; i < argc; i++)
      args.push_back(argv[i].var_);
    return Var(PassRef(), ppb_var->Construct(var_, argc, &args[0],
                                             OutException(exception)));
  } else {
    // Don't try to get the address of a vector if it's empty.
    return Var(PassRef(), ppb_var->Construct(var_, 0, NULL,
                                             OutException(exception)));
  }
}

Var Var::Call(const Var& method_name, const Var& arg1, Var* exception) {
  EnsureInit();
  PP_Var args[1] = {arg1.var_};
  return Var(PassRef(), ppb_var->Call(var_, method_name.var_, 1, args,
                                      OutException(exception)));
}

Var Var::Call(const Var& method_name, const Var& arg1, const Var& arg2,
              Var* exception) {
  EnsureInit();
  PP_Var args[2] = {arg1.var_, arg2.var_};
  return Var(PassRef(), ppb_var->Call(var_, method_name.var_, 2, args,
                                      OutException(exception)));
}

Var Var::Call(const Var& method_name, const Var& arg1, const Var& arg2,
              const Var& arg3, Var* exception) {
  EnsureInit();
  PP_Var args[3] = {arg1.var_, arg2.var_, arg3.var_};
  return Var(PassRef(), ppb_var->Call(var_, method_name.var_, 3, args,
                                      OutException(exception)));
}

Var Var::Call(const Var& method_name, const Var& arg1, const Var& arg2,
              const Var& arg3, const Var& arg4, Var* exception) {
  EnsureInit();
  PP_Var args[4] = {arg1.var_, arg2.var_, arg3.var_, arg4.var_};
  return Var(PassRef(), ppb_var->Call(var_, method_name.var_, 4, args,
                                      OutException(exception)));
}

}  // namespace pp

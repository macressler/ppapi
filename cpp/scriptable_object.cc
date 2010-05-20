// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ppapi/cpp/scriptable_object.h"

#include "ppapi/c/ppp_class.h"
#include "ppapi/cpp/var.h"

namespace pp {

namespace {

// Allows converting an output param of a Var to an output param of a PP_Var
// for exceptions. The object is only copied if it is not void, which we
// take to mean an exception occurred.
class ExceptionConverter {
 public:
  ExceptionConverter(PP_Var* out) : out_(out) {
  }
  ~ExceptionConverter() {
    if (!exception_.is_void())
      *out_ = exception_.Detach();
  }

  Var* Get() { return &exception_; };

 private:
  PP_Var* out_;
  Var exception_;
};

// Used internally to convert a C-style array of PP_Var to a vector of Var.
void ArgListToVector(uint32_t argc, PP_Var* argv, std::vector<Var>* output) {
  output->reserve(argc);
  for (size_t i = 0; i < argc; i++)
    output->push_back(Var(Var::DontManage(), argv[i]));
}

bool HasProperty(void* object, PP_Var name, PP_Var* exception) {
  ExceptionConverter e(exception);
  return reinterpret_cast<ScriptableObject*>(object)->HasProperty(
      Var(Var::DontManage(), name), e.Get());
}

bool HasMethod(void* object, PP_Var name, PP_Var* exception) {
  ExceptionConverter e(exception);
  return reinterpret_cast<ScriptableObject*>(object)->HasMethod(
      Var(Var::DontManage(), name), e.Get());
}

PP_Var GetProperty(void* object,
                   PP_Var name,
                   PP_Var* exception) {
  ExceptionConverter e(exception);
  return reinterpret_cast<ScriptableObject*>(object)->GetProperty(
      Var(Var::DontManage(), name), e.Get()).Detach();
}

void GetAllProperties(void* object,
                      uint32_t* property_count,
                      PP_Var** properties) {
  std::vector<Var> props;
  reinterpret_cast<ScriptableObject*>(object)->GetAllProperties(&props);

  // FIXME(brettw) convert to an array type.
  // PP_Var* 
}

void SetProperty(void* object,
                 PP_Var name,
                 PP_Var value,
                 PP_Var* exception) {
  ExceptionConverter e(exception);
  reinterpret_cast<ScriptableObject*>(object)->SetProperty(
      Var(Var::DontManage(), name), Var(Var::DontManage(), value), e.Get());
}

void RemoveProperty(void* object,
                    PP_Var name,
                    PP_Var* exception) {
  ExceptionConverter e(exception);
  reinterpret_cast<ScriptableObject*>(object)->RemoveProperty(
      Var(Var::DontManage(), name), e.Get());
}

PP_Var Call(void* object,
            PP_Var method_name,
            uint32_t argc,
            PP_Var* argv,
            PP_Var* exception) {
  ExceptionConverter e(exception);

  std::vector<Var> args;
  ArgListToVector(argc, argv, &args);
  return reinterpret_cast<ScriptableObject*>(object)->Call(
      Var(Var::DontManage(), method_name), args, e.Get()).Detach();
}

PP_Var Construct(void* object,
                 uint32_t argc,
                 PP_Var* argv,
                 PP_Var* exception) {
  ExceptionConverter e(exception);

  std::vector<Var> args;
  ArgListToVector(argc, argv, &args);
  return reinterpret_cast<ScriptableObject*>(object)->Construct(
      args, e.Get()).Detach();
}

void Deallocate(void* object) {
  delete reinterpret_cast<ScriptableObject*>(object);
}

PPP_Class plugin_class = {
  &HasProperty,
  &HasMethod,
  &GetProperty,
  &GetAllProperties,
  &SetProperty,
  &RemoveProperty,
  &Call,
  &Construct,
  &Deallocate
};

}  // namespace

bool ScriptableObject::HasProperty(const Var& name, Var* exception) {
  return false;
}

bool ScriptableObject::HasMethod(const Var& name, Var* exception) {
  return false;
}

Var ScriptableObject::GetProperty(const Var& name, Var* exception) {
  *exception = Var("Property does not exist on ScriptableObject");
  return Var();
}

void ScriptableObject::GetAllProperties(std::vector<Var>* properties) {
}

void ScriptableObject::SetProperty(const Var& name,
                                   const Var& value,
                                   Var* exception) {
  *exception = Var("Property can not be set on ScriptableObject");
}

void ScriptableObject::RemoveProperty(const Var& name,
                                      Var* exception) {
  *exception = Var(
      "Property does does not exist to be removed in ScriptableObject");
}

Var ScriptableObject::Call(const Var& method_name,
                           const std::vector<Var>& args,
                           Var* exception) {
  *exception = Var("Method does not exist to call in ScriptableObject");
  return Var();
}

Var ScriptableObject::Construct(const std::vector<Var>& args,
                                Var* exception) {
  *exception = Var("Constuct method does not exist in ScriptableObject");
  return Var();
}

// static
const PPP_Class* ScriptableObject::GetClass() {
  return &plugin_class;
}

}  // namespace pp

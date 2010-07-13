// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_VAR_H_
#define PPAPI_CPP_VAR_H_

#include <string>
#include <vector>

#include "ppapi/c/pp_var.h"

typedef struct _ppp_Class PPP_Class;

namespace pp {

class ScriptableObject;

class Var {
 public:
  struct Null {};  // Special value passed to constructor to make NULL.

  Var();  // PP_Var of type Void.
  Var(Null);  // PP_Var of type Null.
  Var(bool b);
  Var(int32_t i);
  Var(double d);
  Var(const char* str);
  Var(const std::string& str);

  // This magic constructor is used when we've gotten a PP_Var as a return
  // value that has already been addref'ed for us.
  struct PassRef {};
  Var(PassRef, PP_Var var) {
    var_ = var;
    needs_release_ = true;
  }

  // This magic constructor is used when we've given a PP_Var as an input
  // argument from somewhere and that reference is managing the reference
  // count for us. The object will not be AddRef'ed or Release'd by this
  // class instance..
  struct DontManage {};
  Var(DontManage, PP_Var var) {
    var_ = var;
    needs_release_ = false;
  }

  // Takes ownership of the given pointer.
  Var(ScriptableObject* object);

  Var(const Var& other);

  virtual ~Var();

  Var& operator=(const Var& other);
  void swap(Var& other);

  bool is_void() const { return var_.type == PP_VARTYPE_VOID; }
  bool is_null() const { return var_.type == PP_VARTYPE_NULL; }
  bool is_bool() const { return var_.type == PP_VARTYPE_BOOL; }
  bool is_string() const { return var_.type == PP_VARTYPE_STRING; }
  bool is_object() const { return var_.type == PP_VARTYPE_OBJECT; }

  // IsInt and IsDouble return the internal representation. The JavaScript
  // runtime may convert between the two as needed, so the distinction may
  // not be relevant in all cases (int is really an optimization inside the
  // runtime). So most of the time, you will want to check IsNumber.
  bool is_int() const { return var_.type == PP_VARTYPE_INT32; }
  bool is_double() const { return var_.type == PP_VARTYPE_DOUBLE; }
  bool is_number() const {
    return var_.type == PP_VARTYPE_INT32 ||
           var_.type == PP_VARTYPE_DOUBLE;
  }

  // Assumes the internal representation IsBool. If it's not, it will assert
  // in debug mode, and return false.
  bool AsBool() const;

  // AsInt and AsDouble implicitly convert between ints and doubles. This is
  // because JavaScript doesn't have a concept of ints and doubles, only
  // numbers. The distinction between the two is an optimization inside the
  // compiler. Since converting from a double to an int may be lossy, if you
  // care about the distinction, either always work in doubles, or check
  // !IsDouble() before calling AsInt().
  //
  // These functions will assert in debug mode and return 0 if the internal
  // representation is not IsNumber().
  int32_t AsInt() const;
  double AsDouble() const;

  // This assumes the object is of type string. If it's not, it will assert
  // in debug mode, and return an empty string.
  std::string AsString() const;

  bool HasProperty(const Var& name, Var* exception = NULL) const;
  Var GetProperty(const Var& name, Var* exception = NULL) const;
  void GetAllPropertyNames(std::vector<Var>* properties,
                           Var* exception = NULL) const;
  void SetProperty(const Var& name, const Var& value, Var* exception = NULL);
  void RemoveProperty(const Var& name, Var* exception = NULL);
  Var Call(const Var& method_name, uint32_t argc, Var* argv,
           Var* exception = NULL);
  Var Construct(uint32_t argc, Var* argv, Var* exception = NULL) const;

  // Convenience functions for calling functions with small # of args.
  Var Call(const Var& method_name, const Var& arg1, Var* exception = NULL);
  Var Call(const Var& method_name, const Var& arg1, const Var& arg2,
           Var* exception = NULL);
  Var Call(const Var& method_name, const Var& arg1, const Var& arg2,
           const Var& arg3, Var* exception = NULL);
  Var Call(const Var& method_name, const Var& arg1, const Var& arg2,
           const Var& arg3, const Var& arg4, Var* exception = NULL);

  // Returns a const reference to the PP_Var managed by this Var object.
  const PP_Var& pp_var() const {
    return var_;
  }

  // Detaches from the internal PP_Var of this object, keeping the reference
  // count the same. This is used when returning a PP_Var from an API function
  // where the caller expects the return value to be AddRef'ed for it.
  PP_Var Detach() {
    PP_Var ret = var_;
    var_ = PP_MakeVoid();
    needs_release_ = false;
    return ret;
  }

 private:
  // For internal use, this will handle getting the address of the internal
  // value out if it's non-NULL for exception handling.
  static inline PP_Var* OutException(Var* v) {
    return v ? &v->var_ : 0;
  }

  PP_Var var_;
  bool needs_release_;
};

}  // namespace pp

#endif  // PPAPI_CPP_VAR_H_


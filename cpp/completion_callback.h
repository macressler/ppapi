// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_COMPLETION_CALLBACK_H_
#define PPAPI_CPP_COMPLETION_CALLBACK_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/cpp/logging.h"

namespace pp {

class CompletionCallback {
 public:
  void Run(int32_t result) {
    thunk_(this, result);
  }

  static PP_CompletionCallback ToPP(CompletionCallback* cc) {
    if (!cc)
      return PP_BlockUntilComplete();
    PP_CompletionCallback result = { cc->thunk_, cc };
    return result;
  }

 protected:
  typedef void (*ThunkType)(void* user_data, int32_t result);

  CompletionCallback(ThunkType thunk) : thunk_(thunk) {}
  ~CompletionCallback() {}

 private:
  // Disallowed:
  CompletionCallback(const CompletionCallback&);
  CompletionCallback& operator=(const CompletionCallback&);

  ThunkType thunk_;
};

// CompletionCallbackFactory<T> should be used to create CompletionCallback
// objects that are bound to member functions.
// 
// EXAMPLE USAGE:
//
//   class MyHandler {
//    public:
//     MyHandler() : factory_(this), offset_(0) {
//     }
//     void ProcessFile(const FileRef& file) {
//       CompletionCallback* cc = NewCallback();
//       int32_t rv = fio_.Open(file, PP_FileOpenFlag_Read, cc);
//       if (rv != PP_Error_WouldBlock)
//         cc->Run(rv);
//     }
//    private:
//     CompletionCallback* NewCallback() {
//       return factory_.NewCallback(&MyHandler::DidCompleteIO);
//     }
//     void DidCompleteIO(int32_t result) {
//       if (result > 0) {
//         // buf_ now contains 'result' number of bytes from the file.
//         ProcessBytes(buf_, result);
//         offset_ += result;
//         ReadMore();
//       } else if (result == PP_OK && offset_ == 0) {
//         // The file is open, and we can begin reading.
//         ReadMore();
//       } else {
//         // Done reading (possibly with an error given by 'result').
//       }
//     }
//     void ReadMore() {
//       CompletionCallback* cc = NewCallback();
//       int32_t rv = fio_.Read(offset_, buf_, sizeof(buf_), cc);
//       if (rv != PP_Error_WouldBlock)
//         cc->Run(rv);
//     }
//     void ProcessBytes(const char* bytes, int32_t length) {
//       // Do work ...
//     }
//     pp::CompletionCallbackFactory<MyHandler> factory_;
//     pp::FileIO fio_;
//     char buf_[4096];
//     int64_t offset_;
//   };
// 
template <typename T>
class CompletionCallbackFactory {
 public:
  typedef void (T::*Method)(int32_t);

  explicit CompletionCallbackFactory(T* object = NULL)
      : object_(object) {
    PP_DCHECK(object_);  // Expects a non-null object!
    InitBackPointer();
  }

  ~CompletionCallbackFactory() {
    ResetBackPointer();
  }

  void CancelAll() {
    ResetBackPointer();
    InitBackPointer();
  }

  T* GetObject() {
    return object_;
  }

  CompletionCallback* NewCallback(Method method) {
    return new CallbackImpl<T>(back_pointer_, method);
  }

 private:
  class BackPointer {
   public:
    BackPointer() : ref_(0), factory_(NULL) {
    }

    void AddRef() {
      ref_++;
    }

    void Release() {
      if (--ref_ == 0)
        delete this;
    }

    void DropFactory() {
      factory_ = NULL;
    }

    T* GetObject() {
      return factory_ ? factory_->GetObject() : NULL;
    }

   private:
    int32_t ref_;
    CompletionCallbackFactory<T>* factory_;
  };

  template <typename T>
  class CallbackImpl : public CompletionCallback {
   public:
    CallbackImpl(BackPointer* back_pointer, Method method)
        : CompletionCallback(&Self::Thunk),
          back_pointer_(back_pointer),
          method_(method) {
      back_pointer_->AddRef();
    }

    ~CallbackImpl() {
      back_pointer_->Release();
    }

   private:
    static void Thunk(void* user_data, int32_t result) {
      CallbackImpl<T>* self = static_cast<CallbackImpl<T>*>(user_data);
      T* object = self->back_pointer_->GetObject();
      if (object)
        (object->*(self->method_))(result);
      delete self;
    }

    BackPointer* back_pointer_;
    Method method_;
  };

  void InitBackPointer() {
    back_pointer_ = new BackPointer(this);
    back_pointer_->AddRef();
  }

  void ResetBackPointer() {
    back_pointer_->DropFactory();
    back_pointer_->Release();
  }

  // Disallowed:
  CompletionCallbackFactory(const CompletionCallbackFactory&);
  CompletionCallbackFactory& operator=(const CompletionCallbackFactory&);

  T* object_;
  BackPointer* back_pointer_;
};

}  // namespace pp

#endif  // PPAPI_CPP_COMPLETION_CALLBACK_H_

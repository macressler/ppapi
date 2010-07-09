// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef PPAPI_CPP_COMPLETION_CALLBACK_H_
#define PPAPI_CPP_COMPLETION_CALLBACK_H_

#include "ppapi/c/pp_completion_callback.h"
#include "ppapi/cpp/logging.h"

namespace pp {

// A CompletionCallback provides a wrapper around PP_CompletionCallback.
class CompletionCallback {
 public:
  // Use this special constructor to create a 'blocking' CompletionCallback
  // that may be passed to a method to indicate that the calling thread should
  // be blocked until the asynchronous operation corresponding to the method
  // completes.
  struct Block {};
  CompletionCallback(Block) {
    cc_ = PP_BlockUntilComplete();
  }

  CompletionCallback(PP_CompletionCallback_Func func, void* user_data) {
    cc_ = PP_MakeCompletionCallback(func, user_data);
  }

  // Call this method to explicitly run the CompletionCallback.  Normally, the
  // system runs a CompletionCallback after an asynchronous operation
  // completes, but programs may wish to run the CompletionCallback manually
  // in order to reuse the same code paths.
  void Run(int32_t result) {
    PP_DCHECK(cc_.func);
    PP_RunCompletionCallback(&cc_, result);
  }

  const PP_CompletionCallback& pp_completion_callback() const { return cc_; }

 protected:
  PP_CompletionCallback cc_;
};

// CompletionCallbackFactory<T> may be used to create CompletionCallback
// objects that are bound to member functions.
//
// If a factory is destroyed, then any pending callbacks will be cancelled
// preventing any bound member functions from being called.  The CancelAll
// method allows pending callbacks to be cancelled without destroying the
// factory.
// 
// EXAMPLE USAGE:
//
//   class MyHandler {
//    public:
//     MyHandler() : factory_(this), offset_(0) {
//     }
//
//     void ProcessFile(const FileRef& file) {
//       CompletionCallback cc = factory_.NewCallback(&MyHandler::DidOpen);
//       int32_t rv = fio_.Open(file, PP_FileOpenFlag_Read, cc);
//       if (rv != PP_ERROR_WOULDBLOCK)
//         cc.Run(rv);
//     }
//
//    private:
//     CompletionCallback NewCallback() {
//       return factory_.NewCallback(&MyHandler::DidCompleteIO);
//     }
//
//     void DidOpen(int32_t result) {
//       if (result == PP_OK) {
//         // The file is open, and we can begin reading.
//         offset_ = 0;
//         ReadMore();
//       } else {
//         // Failed to open the file with error given by 'result'.
//       }
//     }
//
//     void DidRead(int32_t result) {
//       if (result > 0) {
//         // buf_ now contains 'result' number of bytes from the file.
//         ProcessBytes(buf_, result);
//         offset_ += result;
//         ReadMore();
//       } else {
//         // Done reading (possibly with an error given by 'result').
//       }
//     }
//
//     void ReadMore() {
//       CompletionCallback cc = factory_.NewCallback(&MyHandler::DidRead);
//       int32_t rv = fio_.Read(offset_, buf_, sizeof(buf_), cc);
//       if (rv != PP_ERROR_WOULDBLOCK)
//         cc.Run(rv);
//     }
//
//     void ProcessBytes(const char* bytes, int32_t length) {
//       // Do work ...
//     }
//
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
    InitBackPointer();
  }

  ~CompletionCallbackFactory() {
    ResetBackPointer();
  }

  // Cancels all CompletionCallbacks allocated from this factory.
  void CancelAll() {
    ResetBackPointer();
    InitBackPointer();
  }

  void Initialize(T* object) {
    PP_DCHECK(object);
    PP_DCHECK(!object_);  // May only initialize once!
    object_ = object;
  }

  T* GetObject() {
    return object_;
  }

  // Allocates a new, single-use CompletionCallback.  The CompletionCallback
  // must be run in order for the memory allocated by NewCallback to be freed.
  // If after passing the CompletionCallback to a PPAPI method, the method does
  // not return PP_ERROR_WOULDBLOCK, then you should manually call the
  // CompletionCallback's Run method otherwise memory will be leaked.
  CompletionCallback NewCallback(Method method) {
    PP_DCHECK(object_);  // Expects a non-null object!
    return CompletionCallback(&CallbackData::Thunk,
                              new CallbackData(back_pointer_, method));
  }

 private:
  class BackPointer {
   public:
    BackPointer() : ref_(0), factory_(NULL) {
    }
    BackPointer(CompletionCallbackFactory<T>* factory)
        : ref_(0),
          factory_(factory) {
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

  class CallbackData {
   public:
    CallbackData(BackPointer* back_pointer, Method method)
        : back_pointer_(back_pointer),
          method_(method) {
      back_pointer_->AddRef();
    }

    ~CallbackData() {
      back_pointer_->Release();
    }

    static void Thunk(void* user_data, int32_t result) {
      CallbackData* self = static_cast<CallbackData*>(user_data);
      T* object = self->back_pointer_->GetObject();
      if (object)
        (object->*(self->method_))(result);
      delete self;
    }

   private:
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

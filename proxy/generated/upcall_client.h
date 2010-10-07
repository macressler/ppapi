// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// Automatically generated code.  See srpcgen.py
//
// NaCl Simple Remote Procedure Call interface abstractions.

#ifndef PPAPI_PROXY_GENERATED_UPCALL_CLIENT_H_
#define PPAPI_PROXY_GENERATED_UPCALL_CLIENT_H_
#ifdef __native_client__
#include <nacl/nacl_srpc.h>
#else
#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#endif  // __native_client__

class PppUpcallRpcClient {
 public:
  static NaClSrpcError PPP_Core_CallOnMainThread(
      NaClSrpcChannel* channel,
      int32_t closure_number,
      int32_t delay_in_milliseconds
  );

 private:
  PppUpcallRpcClient();
  PppUpcallRpcClient(const PppUpcallRpcClient&);
  void operator=(const PppUpcallRpcClient);

};  // class PppUpcallRpcClient



#endif  // PPAPI_PROXY_GENERATED_UPCALL_CLIENT_H_


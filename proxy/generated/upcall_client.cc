// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// Automatically generated code.  See srpcgen.py
//
// NaCl Simple Remote Procedure Call interface abstractions.

#include "upcall_client.h"
#ifdef __native_client__
#include <nacl/nacl_srpc.h>
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) do { (void) P; } while (0)
#endif  // UNREFERENCED_PARAMETER
#else
#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#endif  // __native_client__

NaClSrpcError PppUpcallRpcClient::PPP_Core_CallOnMainThread(
    NaClSrpcChannel* channel,
    int32_t closure_number,
    int32_t delay_in_milliseconds
)  {
  NaClSrpcError retval;
  retval = NaClSrpcInvokeBySignature(
      channel,
      "PPP_Core_CallOnMainThread:ii:",
      closure_number,
      delay_in_milliseconds
  );
  return retval;
}



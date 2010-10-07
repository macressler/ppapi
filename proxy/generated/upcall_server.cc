// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// Automatically generated code.  See srpcgen.py
//
// NaCl Simple Remote Procedure Call interface abstractions.

#include "upcall_server.h"
#ifdef __native_client__
#include <nacl/nacl_srpc.h>
#ifndef UNREFERENCED_PARAMETER
#define UNREFERENCED_PARAMETER(P) do { (void) P; } while (0)
#endif  // UNREFERENCED_PARAMETER
#else
#include "native_client/src/include/portability.h"
#include "native_client/src/shared/srpc/nacl_srpc.h"
#endif  // __native_client__

namespace {

static NaClSrpcError PPP_Core_CallOnMainThreadDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = PppUpcallRpcServer::PPP_Core_CallOnMainThread(
      channel,
      inputs[0]->u.ival,
      inputs[1]->u.ival
  );
  return retval;
}

}  // namespace

NACL_SRPC_METHOD_ARRAY(PpbUpcalls::srpc_methods) = {
  { "PPP_Core_CallOnMainThread:ii:", PPP_Core_CallOnMainThreadDispatcher },
  { NULL, NULL }
};  // NACL_SRPC_METHOD_ARRAY


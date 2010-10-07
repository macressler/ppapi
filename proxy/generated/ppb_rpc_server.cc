// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// Automatically generated code.  See srpcgen.py
//
// NaCl Simple Remote Procedure Call interface abstractions.

#include "ppb_rpc_server.h"
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

static NaClSrpcError HasPropertyDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::HasProperty(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      inputs[2]->u.caval.count, inputs[2]->u.caval.carr,
      &(outputs[0]->u.ival),
      &(outputs[1]->u.caval.count), outputs[1]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError HasMethodDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::HasMethod(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      inputs[2]->u.caval.count, inputs[2]->u.caval.carr,
      &(outputs[0]->u.ival),
      &(outputs[1]->u.caval.count), outputs[1]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError GetPropertyDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::GetProperty(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      inputs[2]->u.caval.count, inputs[2]->u.caval.carr,
      &(outputs[0]->u.caval.count), outputs[0]->u.caval.carr,
      &(outputs[1]->u.caval.count), outputs[1]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError GetAllPropertyNamesDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::GetAllPropertyNames(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      &(outputs[0]->u.ival),
      &(outputs[1]->u.caval.count), outputs[1]->u.caval.carr,
      &(outputs[2]->u.caval.count), outputs[2]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError SetPropertyDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::SetProperty(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      inputs[2]->u.caval.count, inputs[2]->u.caval.carr,
      inputs[3]->u.caval.count, inputs[3]->u.caval.carr,
      &(outputs[0]->u.caval.count), outputs[0]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError RemovePropertyDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::RemoveProperty(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      inputs[2]->u.caval.count, inputs[2]->u.caval.carr,
      &(outputs[0]->u.caval.count), outputs[0]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError CallDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::Call(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      inputs[2]->u.ival,
      inputs[3]->u.caval.count, inputs[3]->u.caval.carr,
      inputs[4]->u.caval.count, inputs[4]->u.caval.carr,
      &(outputs[0]->u.caval.count), outputs[0]->u.caval.carr,
      &(outputs[1]->u.caval.count), outputs[1]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError ConstructDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::Construct(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr,
      inputs[1]->u.ival,
      inputs[2]->u.caval.count, inputs[2]->u.caval.carr,
      inputs[3]->u.caval.count, inputs[3]->u.caval.carr,
      &(outputs[0]->u.caval.count), outputs[0]->u.caval.carr,
      &(outputs[1]->u.caval.count), outputs[1]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError DeallocateDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = ObjectStubRpcServer::Deallocate(
      channel,
      inputs[0]->u.caval.count, inputs[0]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError PPB_Core_AddRefResourceDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = PpbCoreRpcServer::PPB_Core_AddRefResource(
      channel,
      inputs[0]->u.lval
  );
  return retval;
}

static NaClSrpcError PPB_Core_ReleaseResourceDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = PpbCoreRpcServer::PPB_Core_ReleaseResource(
      channel,
      inputs[0]->u.lval
  );
  return retval;
}

static NaClSrpcError PPB_Core_GetTimeDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(inputs);
  NaClSrpcError retval;
  retval = PpbCoreRpcServer::PPB_Core_GetTime(
      channel,
      &(outputs[0]->u.dval)
  );
  return retval;
}

}  // namespace

NACL_SRPC_METHOD_ARRAY(PpbRpcs::srpc_methods) = {
  { "HasProperty:CCC:iC", HasPropertyDispatcher },
  { "HasMethod:CCC:iC", HasMethodDispatcher },
  { "GetProperty:CCC:CC", GetPropertyDispatcher },
  { "GetAllPropertyNames:CC:iCC", GetAllPropertyNamesDispatcher },
  { "SetProperty:CCCC:C", SetPropertyDispatcher },
  { "RemoveProperty:CCC:C", RemovePropertyDispatcher },
  { "Call:CCiCC:CC", CallDispatcher },
  { "Construct:CiCC:CC", ConstructDispatcher },
  { "Deallocate:C:", DeallocateDispatcher },
  { "PPB_Core_AddRefResource:l:", PPB_Core_AddRefResourceDispatcher },
  { "PPB_Core_ReleaseResource:l:", PPB_Core_ReleaseResourceDispatcher },
  { "PPB_Core_GetTime::d", PPB_Core_GetTimeDispatcher },
  { NULL, NULL }
};  // NACL_SRPC_METHOD_ARRAY


// WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING WARNING
//
// Automatically generated code.  See srpcgen.py
//
// NaCl Simple Remote Procedure Call interface abstractions.

#include "ppp_rpc_server.h"
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

static NaClSrpcError PPP_InitializeModuleDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = PppRpcServer::PPP_InitializeModule(
      channel,
      inputs[0]->u.ival,
      inputs[1]->u.lval,
      inputs[2]->u.hval,
      inputs[3]->u.sval,
      &(outputs[0]->u.ival),
      &(outputs[1]->u.ival)
  );
  return retval;
}

static NaClSrpcError PPP_ShutdownModuleDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(inputs);
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = PppRpcServer::PPP_ShutdownModule(
      channel
  );
  return retval;
}

static NaClSrpcError PPP_GetInterfaceDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = PppRpcServer::PPP_GetInterface(
      channel,
      inputs[0]->u.sval,
      &(outputs[0]->u.ival)
  );
  return retval;
}

static NaClSrpcError PPP_Instance_DidCreateDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_DidCreate(
      channel,
      inputs[0]->u.lval,
      inputs[1]->u.ival,
      inputs[2]->u.caval.count, inputs[2]->u.caval.carr,
      inputs[3]->u.caval.count, inputs[3]->u.caval.carr,
      &(outputs[0]->u.ival)
  );
  return retval;
}

static NaClSrpcError PPP_Instance_DidDestroyDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_DidDestroy(
      channel,
      inputs[0]->u.lval
  );
  return retval;
}

static NaClSrpcError PPP_Instance_DidChangeViewDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_DidChangeView(
      channel,
      inputs[0]->u.lval,
      inputs[1]->u.iaval.count, inputs[1]->u.iaval.iarr,
      inputs[2]->u.iaval.count, inputs[2]->u.iaval.iarr
  );
  return retval;
}

static NaClSrpcError PPP_Instance_DidChangeFocusDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  UNREFERENCED_PARAMETER(outputs);
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_DidChangeFocus(
      channel,
      inputs[0]->u.lval,
      inputs[1]->u.bval
  );
  return retval;
}

static NaClSrpcError PPP_Instance_HandleDocumentLoadDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_HandleDocumentLoad(
      channel,
      inputs[0]->u.lval,
      inputs[1]->u.lval,
      &(outputs[0]->u.ival)
  );
  return retval;
}

static NaClSrpcError PPP_Instance_HandleInputEventDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_HandleInputEvent(
      channel,
      inputs[0]->u.lval,
      inputs[1]->u.caval.count, inputs[1]->u.caval.carr,
      &(outputs[0]->u.ival)
  );
  return retval;
}

static NaClSrpcError PPP_Instance_GetInstanceObjectDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_GetInstanceObject(
      channel,
      inputs[0]->u.lval,
      &(outputs[0]->u.caval.count), outputs[0]->u.caval.carr
  );
  return retval;
}

static NaClSrpcError PPP_Instance_GetSelectedTextDispatcher(
    NaClSrpcChannel* channel,
    NaClSrpcArg** inputs,
    NaClSrpcArg** outputs
) {
  NaClSrpcError retval;
  retval = PppInstanceRpcServer::PPP_Instance_GetSelectedText(
      channel,
      inputs[0]->u.lval,
      inputs[1]->u.ival,
      &(outputs[0]->u.caval.count), outputs[0]->u.caval.carr
  );
  return retval;
}

}  // namespace

NACL_SRPC_METHOD_ARRAY(PppRpcs::srpc_methods) = {
  { "HasProperty:CCC:iC", HasPropertyDispatcher },
  { "HasMethod:CCC:iC", HasMethodDispatcher },
  { "GetProperty:CCC:CC", GetPropertyDispatcher },
  { "GetAllPropertyNames:CC:iCC", GetAllPropertyNamesDispatcher },
  { "SetProperty:CCCC:C", SetPropertyDispatcher },
  { "RemoveProperty:CCC:C", RemovePropertyDispatcher },
  { "Call:CCiCC:CC", CallDispatcher },
  { "Construct:CiCC:CC", ConstructDispatcher },
  { "Deallocate:C:", DeallocateDispatcher },
  { "PPP_InitializeModule:ilhs:ii", PPP_InitializeModuleDispatcher },
  { "PPP_ShutdownModule::", PPP_ShutdownModuleDispatcher },
  { "PPP_GetInterface:s:i", PPP_GetInterfaceDispatcher },
  { "PPP_Instance_DidCreate:liCC:i", PPP_Instance_DidCreateDispatcher },
  { "PPP_Instance_DidDestroy:l:", PPP_Instance_DidDestroyDispatcher },
  { "PPP_Instance_DidChangeView:lII:", PPP_Instance_DidChangeViewDispatcher },
  { "PPP_Instance_DidChangeFocus:lb:", PPP_Instance_DidChangeFocusDispatcher },
  { "PPP_Instance_HandleDocumentLoad:ll:i", PPP_Instance_HandleDocumentLoadDispatcher },
  { "PPP_Instance_HandleInputEvent:lC:i", PPP_Instance_HandleInputEventDispatcher },
  { "PPP_Instance_GetInstanceObject:l:C", PPP_Instance_GetInstanceObjectDispatcher },
  { "PPP_Instance_GetSelectedText:li:C", PPP_Instance_GetSelectedTextDispatcher },
  { NULL, NULL }
};  // NACL_SRPC_METHOD_ARRAY


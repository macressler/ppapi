#!/bin/sh
srpcgen=../../../../native_client/tools/srpcgen.py

# Browser
python $srpcgen -s PpbRpcs PPAPI_PROXY_GENERATED_PPB_RPC_SERVER_H_ ppb_rpc_server.h ppb_rpc_server.cc ../objectstub.srpc ../ppb_core.srpc
python $srpcgen -c PppRpcs PPAPI_PROXY_GENERATED_PPP_RPC_CLIENT_H_ ppp_rpc_client.h ppp_rpc_client.cc ../objectstub.srpc ../ppp.srpc ../ppp_instance.srpc
python $srpcgen -s PpbUpcalls PPAPI_PROXY_GENERATED_UPCALL_SERVER_H_ upcall_server.h upcall_server.cc ../upcall.srpc


# Plugin
python $srpcgen -c PpbRpcs PPAPI_PROXY_GENERATED_PPB_RPC_CLIENT_H_ ppb_rpc_client.h ppb_rpc_client.cc ../objectstub.srpc ../ppb_core.srpc
python $srpcgen -s PppRpcs PPAPI_PROXY_GENERATED_PPP_RPC_SERVER_H_ ppp_rpc_server.h ppp_rpc_server.cc ../objectstub.srpc ../ppp.srpc ../ppp_instance.srpc
python $srpcgen -c PpbUpcalls PPAPI_PROXY_GENERATED_UPCALL_CLIENT_H_ upcall_client.h upcall_client.cc ../upcall.srpc

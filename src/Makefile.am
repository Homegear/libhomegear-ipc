AUTOMAKE_OPTIONS = subdir-objects

AM_CPPFLAGS = -Wall -std=c++11 -DFORTIFY_SOURCE=2 -DGCRYPT_NO_DEPRECATED
AM_LDFLAGS = -Wl,-rpath=/lib/homegear -Wl,-rpath=/usr/lib/homegear -Wl,-rpath=/usr/local/lib/homegear

lib_LTLIBRARIES = libhomegear-ipc.la
libhomegear_ipc_la_SOURCES = Ansi.cpp BinaryDecoder.cpp BinaryEncoder.cpp BinaryRpc.cpp HelperFunctions.cpp IIpcClient.cpp IQueue.cpp IQueueBase.cpp JsonDecoder.cpp JsonEncoder.cpp Math.cpp Output.cpp RpcDecoder.cpp RpcEncoder.cpp Variable.cpp
libhomegear_ipc_la_LDFLAGS = -version-info 1:0:0

otherincludedir = $(includedir)/homegear-ipc
nobase_otherinclude_HEADERS = BinaryDecoder.h BinaryEncoder.h BinaryRpc.h HelperFunctions.h IIpcClient.h IpcException.h IpcResponse.h IQueue.h IQueueBase.h JsonDecoder.h JsonEncoder.h Math.h Output.h RpcDecoder.h RpcEncoder.h RpcHeader.h Variable.h

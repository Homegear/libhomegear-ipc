cmake_minimum_required(VERSION 3.8)
project(libhomegear_ipc)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        src/Ansi.cpp
        src/Ansi.h
        src/BinaryDecoder.cpp
        src/BinaryDecoder.h
        src/BinaryEncoder.cpp
        src/BinaryEncoder.h
        src/BinaryRpc.cpp
        src/BinaryRpc.h
        src/HelperFunctions.cpp
        src/HelperFunctions.h
        src/IIpcClient.cpp
        src/IIpcClient.h
        src/IpcException.h
        src/IpcResponse.h
        src/IQueue.cpp
        src/IQueue.h
        src/IQueueBase.cpp
        src/IQueueBase.h
        src/JsonDecoder.cpp
        src/JsonDecoder.h
        src/JsonEncoder.cpp
        src/JsonEncoder.h
        src/Math.cpp
        src/Math.h
        src/Output.cpp
        src/Output.h
        src/RpcDecoder.cpp
        src/RpcDecoder.h
        src/RpcEncoder.cpp
        src/RpcEncoder.h
        src/RpcHeader.h
        src/Variable.cpp
        src/Variable.h)

add_custom_target(homegear COMMAND ../../makeAll.sh SOURCES ${SOURCE_FILES})

add_library(libhomegear_ipc ${SOURCE_FILES})
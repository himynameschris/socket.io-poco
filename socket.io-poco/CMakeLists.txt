cmake_minimum_required(VERSION 2.8.9)

project(SOCKETIOPOCO)
set(CMAKE_BUILD_TYPE Debug)

find_library(POCO_FOUNDATION PocoFoundation)
find_library(POCO_NET PocoNet)
find_library(POCO_JSON PocoJSON)
find_library(POCO_NETSSL PocoNetSSL)

SET (INCLUDE_SOCKETIOPOCO ${CMAKE_CURRENT_SOURCE_DIR}/include PARENT_SCOPE)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set(source_files
	src/SIOClient.cpp
	src/SIOClientImpl.cpp
	src/SIOClientRegistry.cpp
	src/SIOEventRegistry.cpp
	src/SIONotificationHandler.cpp
	src/SIOPacket.cpp
)

add_library(socketiopoco SHARED ${source_files})
target_link_libraries(socketiopoco ${POCO_FOUNDATION} ${POCO_JSON} ${POCO_NET} ${POCO_NETSSL})

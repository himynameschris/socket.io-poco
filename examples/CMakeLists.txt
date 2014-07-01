cmake_minimum_required(VERSION 2.8.9)

project(SOCKETIOPOCOEXAMPLE)
set(CMAKE_BUILD_TYPE Debug)


set(source_files
	TestClient/main.cpp
	TestClient/TestEndpointTarget.cpp
	TestClient/TestTarget.cpp
)
include_directories(${INCLUDE_SOCKETIOPOCO})

add_executable(socketiopoco_example ${source_files})
target_link_libraries(socketiopoco_example socketiopoco)



set(CMAKE_BUILD_TYPE Debug)

set(source_files
	main.cpp
	TestEndpointTarget.cpp
	TestTarget.cpp
)
include_directories(${INCLUDE_SOCKETIOPOCO})

add_executable(socketiopoco_example ${source_files})
target_link_libraries(socketiopoco_example socketiopoco_shared)

install (TARGETS socketiopoco_example DESTINATION bin)

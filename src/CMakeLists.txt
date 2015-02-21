cmake_minimum_required(VERSION 3.2)

project(socketiopoco)
#
# Project Output Paths
#
SET (MAINFOLDER ${PROJECT_SOURCE_DIR})
SET (EXECUTABLE_OUTPUT_PATH "${MAINFOLDER}/../bin")
SET (LIBRARY_OUTPUT_PATH "${MAINFOLDER}/../lib")

#Third party
set(THIRD_PARTY "${MAINFOLDER}/../third_party/local")
set(THIRD_PARTY_INCLUDE "${THIRD_PARTY}/include")
set(THIRD_PARTY_LIB "${THIRD_PARTY}/lib")


file(GLOB SIOPOCO_SOURCES *.cpp)

include_directories("${MAINFOLDER}/../include" /usr/local/include "${THIRD_PARTY_INCLUDE}")

link_directories(/usr/local/lib "${THIRD_PARTY_LIB}")
add_library(socketiopoco_static STATIC ${SIOPOCO_SOURCES})
add_library(socketiopoco_shared SHARED ${SIOPOCO_SOURCES})
target_link_libraries(socketiopoco_static PocoFoundation PocoJSON PocoNet PocoNetSSL)
target_link_libraries(socketiopoco_shared PocoFoundation PocoJSON PocoNet PocoNetSSL)

install(TARGETS socketiopoco_static DESTINATION lib)
install(TARGETS socketiopoco_shared DESTINATION lib)
install(
    DIRECTORY ${MAINFOLDER}/../include
    DESTINATION .
    COMPONENT Devel
    PATTERN ".svn" EXCLUDE
    )


option (COMPILE_EXAMPLES "COMPILE_EXAMPLES" OFF)

if(COMPILE_EXAMPLES)
  add_subdirectory(examples)
endif(COMPILE_EXAMPLES)

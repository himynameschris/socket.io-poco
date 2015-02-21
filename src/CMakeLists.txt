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
add_library(socketiopoco SHARED ${SIOPOCO_SOURCES})
target_link_libraries(socketiopoco PocoFoundation PocoJSON PocoNet PocoNetSSL)


option (COMPILE_EXAMPLES "COMPILE_EXAMPLES" OFF)

if(COMPILE_EXAMPLES)
  add_subdirectory(examples)
endif(COMPILE_EXAMPLES)

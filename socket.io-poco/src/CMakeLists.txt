file(GLOB SIOPOCO_SOURCES *.cpp)

include_directories("${CMAKE_SOURCE_DIR}/include" /usr/local/include)

link_directories(/usr/local/lib)
add_library(socketiopoco SHARED ${SIOPOCO_SOURCES})
target_link_libraries(socketiopoco PocoFoundation PocoJSON PocoNet)

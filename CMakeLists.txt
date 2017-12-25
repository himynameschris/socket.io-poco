cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_CXX_STANDARD 11)
execute_process(COMMAND "third_party/installDependancies.sh")
add_subdirectory(src)

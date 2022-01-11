cmake_minimum_required(VERSION 3.0.0)
project(LogTraslate VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include)
aux_source_directory(code_src "./src")
add_executable(LogTraslate main.cpp src/OpenFile.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

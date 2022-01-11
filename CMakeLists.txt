cmake_minimum_required(VERSION 3.0.0)
project(LogTraslate VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(lib/spdlog)
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

include_directories(./include)
aux_source_directory("./src/" code_src)
message("src code : ${code_src}")
add_executable(LogTraslate main.cpp ${code_src})
target_link_libraries(LogTraslate PRIVATE spdlog::spdlog)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

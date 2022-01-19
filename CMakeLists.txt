cmake_minimum_required(VERSION 3.0.0)
project(LogTraslate VERSION 0.1.0)

include(CTest)
enable_testing()

# spdlog的部分配置
add_subdirectory(lib/spdlog)
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

# json的cmake部分配置
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(lib/json)

# google test framework
#add_subdirectory(lib/googletest)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(./include)
aux_source_directory("./src/" code_src)
message("src code : ${code_src}")
add_executable(LogTraslate main.cpp ${code_src})
target_link_libraries(LogTraslate 
                        PRIVATE
                        spdlog::spdlog
                        nlohmann_json::nlohmann_json
                    )



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

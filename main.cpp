#include <iostream>
#include <string>
#include <vector>
// 获取文件路径
#include "OpenFIle.hpp"
#ifdef _WIN32
#include <direct.h>
#elif __APPLE__ || __linux__
#include<unistd.h>
#endif
using namespace std;

static string getWorkPath(){
    char runPath[1024] = {0};
    getcwd(runPath, sizeof(runPath));
    return string(runPath);
}

static void getOprationSystemType(){
    #ifdef _WIN32
    cout << "This is Windows" << endl;
    #elif __APPLE__ || __linux__
    cout << "This is Linux" << endl;
    #endif
}


void testlog(){
    // load_levels_example();
    spdlog::cfg::load_env_levels();

    spdlog::info("Welcome to spdlog version {}.{}.{}  !", SPDLOG_VER_MAJOR, SPDLOG_VER_MINOR, SPDLOG_VER_PATCH);

    spdlog::warn("Easy padding in numbers like {:08d}", 12);
    spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    spdlog::info("Support for floats {:03.2f}", 1.23456);
    spdlog::info("Positional args are {1} {0}..", "too", "supported");
    spdlog::info("{:>8} aligned, {:<8} aligned", "right", "left");
}

int main(int, char**) {
    cout << "workPath:" <<getWorkPath() << endl;
    getOprationSystemType();

    OpenFile hashLog("dddd");
    testlog();

}

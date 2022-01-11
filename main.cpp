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

int main(int, char**) {
    cout << "workPath:" <<getWorkPath() << endl;
    getOprationSystemType();

    OpenFile hashLog("dddd");

}

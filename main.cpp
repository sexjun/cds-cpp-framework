#include <iostream>
#include <string>
#include <vector>
#include <fstream>
// 获取文件路径
#include "OpenFIle.hpp"
#ifdef _WIN32
#include <direct.h>
#elif __APPLE__ || __linux__
#include<unistd.h>
#endif
using namespace std;
using namespace spdlog;

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
    spdlog::set_level(static_cast<level::level_enum>(SPDLOG_LEVEL_ERROR));
}


int json_test(vector<int> &hash_list, vector<string> &string_list)
{
    // read a JSON file
    std::ifstream i("../res/test.json");
    json file_json_obj;
    if(i.is_open()) {
        i >> file_json_obj;
    } else {
        error("file not fonud");
    }
    for(int i = 0; i < file_json_obj.size(); i++) {
        hash_list.push_back(file_json_obj.at(0)["hash_value"]);
        string_list.push_back(file_json_obj.at(0)["string_value"]);
    }
    debug("json map size : {}", file_json_obj.size());

    std::ofstream o("../res/test_3.json");
    o << std::setw(4) << file_json_obj << std::endl;
    o << "end";
    o.close();
    return 1;
}

int main(int, char**) {
    testlog();
    cout << "workPath:" <<getWorkPath() << endl;
    getOprationSystemType();

    vector<int> hash_list;
    vector<string> string_list;
    json_test(hash_list, string_list);


    OpenFile hashLog("../res/test.json");
    hashLog.getHashJsonConfigFile(string_list, hash_list);
    hashLog.parseHashLog2UserLog();
    hashLog.saveUserLog();
}

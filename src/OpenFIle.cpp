#include <iostream>
// #include <ifstream>
#include <fstream>
// #include <ifstream>
#include "OpenFIle.hpp"

using namespace std;

OpenFile::OpenFile(std::string fileName)
{
    std::ifstream file(fileName);
    if(file.is_open())
    {
        std::string line;
        while(std::getline(file, line))
        {
            std::cout << line << std::endl;
        }
        file.close();
    }

    cout << "================================================================" <<std::endl;
    return;
}
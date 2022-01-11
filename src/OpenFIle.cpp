#include <iostream>
#include <fstream>
#include "OpenFIle.hpp"



using namespace std;

OpenFile::OpenFile(std::string fileName):fileSize(0),fileName(fileName),fileContent(nullptr)
{

    this->fileName = fileName;
    std::ifstream file(fileName);
    if(file.is_open())
    {
        file.seekg(0, std::ios::end);
        this->fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string line;
        while(std::getline(file, line))
        {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cout << "OpenFile: Unable to open file:" << this->fileName << std::endl;
    }

    cout << "================================================================" <<std::endl;
    return;
}

int OpenFile::getFileSize()
{
    return this->fileSize;
}

void OpenFile::setfileSize(int fileSize)
{
    this->fileSize = fileSize;
}


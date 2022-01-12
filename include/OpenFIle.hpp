#include <string>
#include <iostream>

#include "libHead.h"

class OpenFile {
public:
    OpenFile(std::string fileName);
    OpenFile();
    ~OpenFile(){};
    void readFile();
    void writeFile();
    void setFileName(std::string fileName);
    std::string getFileName();
    std::string getFileContent();
    void setFileContent(std::string fileContent);
    void setfileSize(int fileSize);
    int getFileSize();
private:
    // 文件的内容读取到这里
    char* fileContent;
    int fileSize;
    std::string fileName;
};
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
    void setFileContent(std::string fileContent);
    void setFileSize(int fileSize);
    int getFileSize();
private:
    bool readFileInfo();
    bool readHshLogContent();
    bool parseHashLog2UserLog();
    bool ParseOneHashLogToUserLog(const char* oneHashLog, const int logSize);

    bool logTagHandle(uint8_t logTag);
    bool logLevelHandle(uint8_t logLevel);
    bool logTimeHandle(uint32_t time, uint16_t ms);
    bool logHashValueHandle(uint32_t hashValue, uint8_t parameterNumber, const char* parameterList);
    void getHashJsonConfigFile(const std::vector<std::string> &str, const std::vector<int> &hash);
    // 文件的内容读取到这里
    const int FILE_MAX_SIZE = 1024 * 1024 * 3;
    const int USE_LOG_MAX_SIZE = 1024 * 1024 * 20;
    const uint8_t isAlogHeadFlag = 0xE5;
    // 读取hahs日志的原始内容
    char* fileContent;
    // 顺序排序hash日志后的内容
    char* hashLogContent;
    // 将hash日志转为用户可读的日志信息
    char* userLogContent;
    int fileSize;
    std::string fileName;
    std::vector<std::string> stringLists;
    std::vector<int> hashLists;
};
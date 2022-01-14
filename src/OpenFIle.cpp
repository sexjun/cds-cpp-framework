#include <iostream>
#include <fstream>
//#include <stdint.h>
#include <cstdint>
#include <filesystem>
#include "OpenFIle.hpp"

using namespace std;
namespace fs = std::filesystem;

OpenFile::OpenFile(std::string fileName):fileSize(0),fileName(fileName),fileContent(nullptr),
                                         hashLogContent(nullptr),
                                         userLogContent(nullptr)
{
    this->getFileSize();
    fileContent = (char*) malloc(this->FILE_MAX_SIZE);
    if(!fileContent) {
        spdlog::error("malloc failed");
    }

    hashLogContent = (char*) malloc(this->fileSize);
    if(!hashLogContent) {
        spdlog::error("malloc failed");
    }

    this->userLogContent = (char*) malloc(this->USE_LOG_MAX_SIZE);
    if(!this->userLogContent) {
        spdlog::error("malloc failed");
    }
    cout << "================================================================" <<std::endl;
}

int OpenFile::getFileSize()
{
    if(!this->fileName.empty()) {
        std::ifstream file(fileName);
        if(file.is_open()) {
            file.seekg(0, std::ios::beg);
            file.seekg(0, std::ios::end);
            if(this->fileSize > this->FILE_MAX_SIZE) {
                spdlog::error("file:{} is {} too big than {}", this->fileName, this->fileSize, this->FILE_MAX_SIZE);
                this->fileSize = 0;
                return -1;
            }
            this->fileSize = file.tellg();
            file.close();
        } else {
            spdlog::error("file : {} open failed", this->fileName);
        }
    } else {
        spdlog::error("file name is null");
    }
    spdlog::debug("{}: file size is {}", this->fileName, this->fileSize);
    return this->fileSize;
}

void OpenFile::setFileSize(int fileSize)
{
    this->fileSize = fileSize;
}

bool OpenFile::readFileInfo() {
    ifstream hashLog(this->fileName);
    if(hashLog.is_open()) {
        hashLog.read(this->fileContent, this->fileSize);
    } else {
        spdlog::error("read {} failed", this->fileName);
        return false;
    }
    spdlog::debug("file content:{}",this->fileContent);
    return true;
}

bool OpenFile::readHshLogContent() {
    uint8_t fileIsNotFull = 0;
    memccpy(&fileIsNotFull, this->fileContent, 1, 1);
    spdlog::info("{} fileIsNotFull: {}", this->fileName, fileIsNotFull);
    if(fileIsNotFull == 0) {
        memcpy(this->hashLogContent, this->fileContent, this->fileSize - sizeof(uint8_t) * 8);
    } else if(fileIsNotFull == 1){
        uint32_t startPos = 0;
        // get start position
        memcpy(&startPos, this->fileContent + sizeof(uint32_t), sizeof(uint32_t));
        // copy from start position
        memcpy(this->hashLogContent, this->fileContent + startPos, this->fileSize - startPos);
        this->fileContent += sizeof(uint8_t) * 8;
        // copy left file content
        memcpy(this->hashLogContent + startPos, this->fileContent, startPos - sizeof(uint8_t) * 8);
    } else {
        memcpy(this->hashLogContent, this->fileContent, this->fileSize - sizeof(uint8_t) * 8);
        spdlog::error("{} file full flag is illegality", this->fileName);
        return false;
    }
    spdlog::debug("hash log file info: {}", this->hashLogContent);
    return true;
}

bool OpenFile::parseHashLog2UserLog() {
    if(this->fileContent == nullptr) {
        spdlog::error("哈希文件读取失败");
        return false;
    }

    this->readFileInfo();
    this->readHshLogContent();

    int logSize = this->fileSize - sizeof(uint8_t) * 8;
    uint8_t isALogHeadFlag = 0; // 0xE5
    uint8_t logLength = 0;
    static const int tempLogLength = 1024 * 1024 * 2;
    char *tempLog = (char*)malloc(tempLogLength);
    memset(tempLog, 0, tempLogLength);
    if(!tempLog) {
        spdlog::error("malloc failed");
    }
    int singleLogMaxLength = 0;

    int ignoreLogTimes = 0;
    int maxFaultTimes = 0;
    while(logSize > 0) {
        while(isALogHeadFlag != this->isAlogHeadFlag) {
            memcpy(&isALogHeadFlag, this->hashLogContent, sizeof(uint8_t));
            this->hashLogContent++;
            if (logSize-- <= 0) {
                spdlog::info("parese hash log to user log done");
                break;
            }
            if(++maxFaultTimes == 100) {
                break;
                spdlog::error("isALogHeadFlag error:{} times", maxFaultTimes);
            }
        }

        if(isALogHeadFlag == this->isAlogHeadFlag) {
//                isALogHeadFlag = 0;
            memcpy(&logLength, this->hashLogContent++, sizeof(uint8_t));
            logSize--;
            if(logLength > singleLogMaxLength) {
                spdlog::debug("singleLogMaxLength:{}", singleLogMaxLength);
                singleLogMaxLength = logLength;
            }
            memcpy(tempLog, this->hashLogContent, logLength);
            this->hashLogContent += logLength;
            logSize -= logLength;
            this->ParseOneHashLogToUserLog(tempLog, logLength);
            logLength = 0;
        } else {
            spdlog::info("isAlogHeadFlag: false");
            ignoreLogTimes++;
        }
        memcpy(&isALogHeadFlag, this->hashLogContent++, sizeof(uint8_t));
        logSize--;
    }
    spdlog::critical("singleLogMaxLength:{}, ignoreLogTimes:{}", singleLogMaxLength, ignoreLogTimes);
    return true;
}

bool OpenFile::ParseOneHashLogToUserLog(const char *oneHashLog, const int logSize) {
    uint8_t logTag = 0;
    uint8_t logLevel = 0;
    uint32_t logTime = 0;
    uint16_t logTimeMs = 0;
    uint32_t logHashValue = 0;
    uint8_t argsNumber = logSize - sizeof(uint8_t) * 16;
    memcpy(&logTag, oneHashLog++, sizeof(uint8_t));
    this->logTagHandle(logTag);
    memcpy(&logLevel, oneHashLog++, sizeof(uint8_t));
    this->logLevelHandle(logLevel);
    memcpy(&logTime, oneHashLog, sizeof(uint32_t));
    oneHashLog += 4;
    memcpy(&logTimeMs, oneHashLog, sizeof(uint8_t) * 2);
    oneHashLog += 2;
    this->logTimeHandle(logTime, logTimeMs);
    memcpy(&logHashValue, oneHashLog++, sizeof(uint8_t) * 4);
    oneHashLog += 4;
    this->logHashValueHandle(logHashValue, argsNumber, oneHashLog);
    return true;
}

bool OpenFile::logTagHandle(uint8_t logTag) {
    memcpy(this->userLogContent++, &logTag, sizeof(uint8_t));
    return true;
}

bool OpenFile::logLevelHandle(uint8_t logLevel) {
    memcpy(this->userLogContent++, &logLevel, sizeof(uint8_t));
    return true;
}

bool OpenFile::logTimeHandle(uint32_t time, uint16_t ms) {
    memcpy(this->userLogContent, &time, sizeof(uint32_t));
    this->userLogContent += sizeof(uint32_t);

    memcpy(this->userLogContent, &ms, sizeof(uint32_t));
    this->userLogContent += sizeof(uint16_t);

    return true;
}

bool OpenFile::logHashValueHandle(uint32_t hashValue, uint8_t parameterNumber, const char *parameterList) {
    int i = 0;
    bool findHashValueInListFlag = false;
    string log = nullptr;

    for(; i < this->hashLists.size(); i++) {
        if(this->hashLists[i] == hashValue) {
            findHashValueInListFlag = true;
            log = this->stringLists[i];
            break;
        }
    }
    uint8_t index = 1;
    uint8_t actualFindParameterNumber = 0;
    while (index < parameterNumber) {
        int found = log.find("%d");
        if (found!=std::string::npos) {
            log.replace(found, sizeof(uint16_t), parameterList, index * sizeof(uint32_t), sizeof(uint32_t));
            actualFindParameterNumber++;
        }
        index++;
    }
    if(actualFindParameterNumber != parameterNumber) {
        spdlog::error("logHashValueHandle:参数列表和打印参数数目不一致。log:{}, parameter number:{}"
                      "actualFindParameterNumber:{} ",log, parameterNumber, actualFindParameterNumber);
        return false;
    }
    log.append("\n");
    memcpy(this->userLogContent, &log, log.size());
    this->userLogContent += log.size();
    return true;
}

void OpenFile::getHashJsonConfigFile(const std::vector<std::string> &str, const std::vector<int> &hash) {
    this->hashLists.assign(hash.begin(), hash.end());
    this->stringLists.assign(str.begin(), str.end());
    return;
}

void OpenFile::saveUserLog() {
    std::filesystem::path p = this->fileName;
    std::filesystem::path basePath = fs::absolute(p);
    basePath.append("user_log");
    spdlog::critical("user log file:{}, hash log file:{}", string(basePath), this->fileName);
    return;
}
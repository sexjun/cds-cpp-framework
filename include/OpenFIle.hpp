#include <string>
#include <iostream>

class OpenFile {
public:
    OpenFile(std::string fileName);
    OpenFile();
    ~OpenFile(){};
    void readFile();
    void writeFile();
    void closeFile();
private:
    char* fileInfo;
};
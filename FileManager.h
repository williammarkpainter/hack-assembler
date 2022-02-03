#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#ifndef FILE_MANAGER_H_
#define FILE_MANAGER_H_

namespace hackassembler{

class FileManager{
private:
    bool validFileName;
    bool inputFileOpen;
    bool outputFileOpen;
    string sourceFileName;
    string baseFileName;
    string outputFileName;
    fstream inputFile;
    fstream *ptr_inputFile;
    fstream outputFile;
public:
    FileManager(string fileName);
    ~FileManager();
    string getSourceFileName();
    string getBaseFileName();
    string getOutputFileName();
    void setFileNames(string fileName);
    bool checkValidFileName();
    bool isValidFileName();
    bool isInputFileOpen();
    bool isOutputFileOpen();
    bool openInputFile();
    bool openOutputFile();
    bool closeInputFile();
    bool closeOutputFile();
    fstream *refInputFile(){return ptr_inputFile;};
};

} /* namespace hackassembler */

#endif /* FILE_MANAGER_H_ */
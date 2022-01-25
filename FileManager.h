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
    ofstream inputFile;
    ofstream *ptr_inputFile;
    ofstream outputFile;
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
    ofstream *refInputFile();
};

} /* namespace hackassembler */

#endif /* FILE_MANAGER_H_ */
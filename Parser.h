#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "HackCodeTranslation.h"

using namespace std;

#ifndef _HACK_PARSER_
#define _HACK_PARSER_

namespace hackassembler{

enum CommandType {A_COMMAND, C_COMMAND, L_COMMAND};

class Parser{
private:
    string currentLine;
    string strippedLine;
    fstream  *ptr_inputFile;
    int lineCount;
    int instructionCount;
    
public:
    Parser(fstream *inputFile);
    ~Parser();
    bool hasMoreCommands();
    void advance();
    CommandType commandType();
    string symbol();
    DestinationCode destination();
    ComputeCode compute();
    JumpCode jump();
    string getCurrentLine();
    string getStrippedLine();
    int getLineCount();
    int getInstructionCount();
};


} /* hackassembler */

#endif /* _HACK_PARSER_ */
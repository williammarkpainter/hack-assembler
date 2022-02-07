#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "HackCodeTranslation.h"

using namespace std;

#ifndef _HACK_PARSER_
#define _HACK_PARSER_

namespace hackassembler {

    enum class CommandType { A_COMMAND, C_COMMAND, L_COMMAND, NOT_COMMAND};

    class Parser {
    private:
        string currentLine;
        string strippedLine;
        HackCodeTranslation *hackcodeTranslationService;
        fstream* ptr_inputFile;
        int lineCount;
        int instructionCount;
        void calculateStrippedLine();

    public:
        Parser(fstream* inputFile);
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
        void returnToFirstCommand();
    };


} /* hackassembler */

#endif /* _HACK_PARSER_ */
/* Parser.cpp manages the looping through of the .asm file and idenitfying if the lines are A, C or comments
*/

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "HackCodeTranslation.h"
#include "Parser.h"

using namespace std;
using namespace hackassembler;

/* CONSTRUCTORS */

Parser::Parser(fstream* ptr_inputFile) {
    
    this->ptr_inputFile = ptr_inputFile;
    this->lineCount = 0;
    this->instructionCount = 0;
    this->hackcodeTranslationService = new HackCodeTranslation();

}

Parser::~Parser() {

}

/* Accessors */

string Parser::getCurrentLine() { return currentLine; }
string Parser::getStrippedLine() { return strippedLine; }
int Parser::getLineCount() { return lineCount; }
int Parser::getInstructionCount() { return instructionCount; }




/* Methods */

/* 
    advance: moves onto the next line of the file, if EOF then marks the Parser as finished, and the string as 
*/
void Parser::advance() {
    if(this->hasMoreCommands()) {
        getline(*this->ptr_inputFile, this->currentLine);
        this->calculateStrippedLine();
        this->lineCount++;
        if (this->commandType() == CommandType::A_COMMAND || this->commandType() == CommandType::C_COMMAND) this->instructionCount++;
    }
    else {
        this->currentLine = "";
    }
}

/*
    hasMoreCommands: returns true if not at the end of the file, potentially more commands may be present to be processed
*/
bool Parser::hasMoreCommands() {
    if(this->ptr_inputFile->eof()) {
        return false;
    }
    else {
        return true;
    }
}

/*
    calculateStrippedLine: runs through the the characters of the currentline, removing whitespace and comments (comments start with //)
*/
void Parser::calculateStrippedLine() {
    string strippedString = "";
    int stringLength = this->currentLine.length();
    for (int i = 0; i < stringLength; i++) {
        if (this->currentLine[i] != ' ') {              // will skip if whitespace
            if (this->currentLine[i] == '/' && i < stringLength && this->currentLine[i+1] == '/') {           // if / and not the final character, then if the next character is /, then can exit, as the remainder is a comment
                i = stringLength;
            }
            else {                                      // not a comment or whitespace, add to strippedString
                strippedString += currentLine[i];
            }
        }
    }
    this->strippedLine = strippedString;
}

/*
    commandType: based on the current stripped string, returns the Command Type of the line
*/
CommandType Parser::commandType() {

    CommandType returnValue = CommandType::NOT_COMMAND;
   
    if (this->strippedLine.length() == 0) {
        returnValue = CommandType::NOT_COMMAND;
    }
    else {
        switch (this->strippedLine[0]) {
        case '@':                       // A commands start with an @
            returnValue = CommandType::A_COMMAND;
            break;
        case '(':                       // Lable / L Commands start with a (
            returnValue = CommandType::L_COMMAND;
            break;
        default:
            returnValue = CommandType::C_COMMAND;          // other non error lines are C commands
        }
    }

    return returnValue;

}

/*
    symbol: returns the symbol or decimal of A_COMMAND or  L_COMMAND
*/
string Parser::symbol() {
    
    CommandType currentCommandType = this->commandType();

    if (currentCommandType == CommandType::C_COMMAND || currentCommandType == CommandType::NOT_COMMAND) {       // C and NOT command do not have a symbol
        return "";
    }

    string returnString = "";

    if (currentCommandType == CommandType::L_COMMAND) {                                                         // L command returns the lable command between the brakets
        returnString = this->strippedLine.substr(1, this->strippedLine.length() - 2);
    }

    if (currentCommandType == CommandType::A_COMMAND) {                                                         // A command returns the address or lable after the @ symbol
        returnString = this->strippedLine.substr(1, this->strippedLine.length() - 1);
    }

    return returnString;
}

/*
    destination: returns the DestinationCode if the commandType is an C command
*/

DestinationCode Parser::destination() {

    string lookupDestString = "null";
    int delimPos = 0;
    char delim = '=';
    
    if(this->commandType() == CommandType::C_COMMAND) {
        if (delimPos = this->strippedLine.find(delim) != string::npos) {         // no equals sign in an C command mean that there is no distination
            delimPos = this->strippedLine.find(delim);
            lookupDestString = this->strippedLine.substr(0, delimPos);
            
        }
    }

    return this->hackcodeTranslationService->dest(lookupDestString);;

}

/*
    jump: returns the JCode if the commandType is an C command
*/

JumpCode Parser::jump() {

    string lookupJumpString = "null";
    int delimPos = 0;
    char delim = ';';

    if (this->commandType() == CommandType::C_COMMAND) {
        if (delimPos = this->strippedLine.find(delim) != string::npos) {         // no ; sign in an C command mean that there is no jump
            delimPos = this->strippedLine.find(delim);
            lookupJumpString = this->strippedLine.substr(delimPos + 1, this->strippedLine.length() - delimPos);

        }
    }

    return this->hackcodeTranslationService->jump(lookupJumpString);;

}

ComputeCode Parser::compute() {

    string lookupComputeString = "null";
    string sLine = this->strippedLine;
    int startPos = 0;
    int endPos = this->strippedLine.length();
    int jumpLocation = 0;
    int destLocation = 0;
    char delimJump = ';';
    char delimDest = '=';

    if (this->commandType() == CommandType::C_COMMAND) {
        if (jumpLocation = sLine.find(delimJump) != string::npos) {         // if there is a jump (with a ;) then the comp code ends before the delim
            jumpLocation = sLine.find(delimJump);
            endPos = jumpLocation;
        }
        if (destLocation = sLine.find(delimDest) != string::npos) {         // if there is a dest (with a =) then the comp code starts after the equals sign
            destLocation = sLine.find(delimDest);
            startPos = destLocation + 1;
        }
        lookupComputeString = sLine.substr(startPos, endPos - startPos);
    }

    return this->hackcodeTranslationService->comp(lookupComputeString);;

}

void Parser::returnToFirstCommand() {
    this->ptr_inputFile->clear();
    this->ptr_inputFile->seekg(0);
    this->instructionCount = 0;
    this->lineCount = 0;
}


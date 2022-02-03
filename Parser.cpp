#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include "HackCodeTranslation.h"
#include "Parser.h"

using namespace std;
using namespace hackassembler;

/* CONSTRUCTORS */

Parser::Parser(fstream *ptr_inputFile){
    currentLine = "Hello World";
}

Parser::~Parser(){

}

/* Accessors */

string Parser::getCurrentLine(){return currentLine;}
string Parser::getStrippedLine(){return strippedLine;}
int Parser::getLineCount(){return lineCount;}
int Parser::getInstructionCount(){return instructionCount;}




/* Methods */
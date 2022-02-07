/*
    HackAssembler: A programme to take an input a file of Hack assembler code (.asm), with the file address referenced in the commandline, and output
        hack binary code as a 16 character char line of 0 and 1's in a .hack file of the same name.
        Hack Language is documents in https://www.nand2tetris.org/_files/ugd/44046b_d70026d8c1424487a451eaba3e372132.pdf
    Author: williammarkpainter@gmail.com / williammarkpainter.com
    Date:   21st Jan 2022
*/
#include <iostream>
#include <string>
#include <fstream>
//#include "hackHelper.cpp"
#include "FileManager.h"            // Provide the read and write functionlaity for the input (.asm) and output (.hack) files
#include "HackCode.h"            // Provide the class and functionality to store the machine code in vector of vector 16 bools (needs to be optimised to 16 bit) 
#include "HackCodeTranslation.h"    // Return the Op codes for translation of Assembly Code strings value to components of the machine code, operates in structs with arrays of bools (needs to be optimised to bits)
#include "SymbolTable.h"            // Tracks the RAM and ROM locations of pre-define symbols (RAM), user variables (RAM), and Labels (ROM)
#include "Parser.h"             // Parses each line in the input file

using namespace std;
using namespace hackassembler;

bool isNumber(string symbol);
int stringToInt(string symbol);
HackLineOfCode intToHackLineOfCode(int number);
 
void numberTest();
void testFileManager(FileManager* fileManager, bool fullTest);
void testFileParser(Parser* fileParser, bool fullTest);
void testAdd(HackCode* hackCode, bool fullTest);
void helperTest(HackCodeTranslation* translationHelper, bool fullTest);
void helperTestPrint(string testString, HackCodeTranslation* translationHelper, bool fullTest);
void symbolTest(SymbolTable* symbolTable, bool fullTest);
void lineTypeTest(Parser* fileParser, bool fullTest);


int main(int argc, char* argv[]) {

    bool fullTest = false;
    bool testOutput = false;

    // check argument has been passed
    if (argc < 2) {
        cout << "No filename argument provided. Programme will not execute." << endl;
        return -1;
    }

    // create file manager and check if valid file name
    FileManager fileManager(argv[1]);

    if (fileManager.isValidFileName() != true) {
        cout << "Filename : " << argv[1] << " was not valid. Programme will not execute." << endl;
        return -2;
    }

    // check if file can be open

    fileManager.openInputFile();
    if (!fileManager.isInputFileOpen()) {
        cout << "Filename : " << argv[1] << " could not be opened. Programme did not execute." << endl;
        return -3;
    }

    // check output file can be created and open it

    fileManager.openOutputFile();
    if (!fileManager.isOutputFileOpen()) {
        cout << "Unable to open file for output with the name : " << fileManager.getOutputFileName() << ". Programme did not execute." << endl;
        return -4;
    }

    if(testOutput) testFileManager(&fileManager, fullTest);
    

    // Build Symbol Table and Initiate Code Module

    HackCodeTranslation translationHelper;
    if(testOutput) helperTest(&translationHelper, fullTest);


    // Initiate Parser

    Parser fileParser(fileManager.refInputFile());
    if (testOutput)
    {
        cout << "Testing File Parser:" << endl;
        testFileParser(&fileParser, fullTest);
        lineTypeTest(&fileParser, fullTest);
        fileParser.returnToFirstCommand();
    }
    

    // First Pass of File to build Symbol Table for labels
    SymbolTable symbolTable; 
    if (testOutput) {
        cout << "Testing Symbol Table:" << endl;
        symbolTest(&symbolTable, fullTest);
    }

    cout << "Reading " << fileManager.getSourceFileName() << "for input." << endl;
    
    while (fileParser.hasMoreCommands()) {                          
        
        fileParser.advance();
        string symbol = fileParser.symbol();

        if (fileParser.commandType() == CommandType::L_COMMAND) {       // Where there are L commands, these need adding to the symbol table with a reference to the next line
            if (symbolTable.contains(symbol) == false) {
                symbolTable.addEntry(symbol, fileParser.getInstructionCount(), false);
            }
        }
    }


    // Second Pass to write program to file, adding A_COMMAND labels to the mapping table
    HackCode hackCode;

    if (testOutput)
    {
        cout << "Testing HackCode:" << endl;
        testAdd(&hackCode, fullTest);
    }

    cout << "Processing file content and generating code." << endl;

    fileParser.returnToFirstCommand();
    while (fileParser.hasMoreCommands()) {
        fileParser.advance();
        HackLineOfCode newline;

        if (fileParser.commandType() == CommandType::A_COMMAND) {       // Where A command, 
            int number = 0;
            string symbol = fileParser.symbol();

            

            if (isNumber(symbol)) {                             // if symbol is a number, then we have a simple address       
                number = stoi(symbol);
            }
            else {                                              // else we need to look up the address in the Symbol table, adding a new address if this is the first instance of the variable
                if (symbolTable.contains(symbol) == false) {
                    symbolTable.addEntry(symbol, symbolTable.getFreeMemoryLocation(), true);
                }
                number = symbolTable.getAddress(symbol);
            }
            newline = intToHackLineOfCode(number);
            hackCode.addLine(newline);
        }
        else if (fileParser.commandType() == CommandType::C_COMMAND) {  // where C command, build up code
            // first 3 items are 1's
            for (int i = 0; i < 3; i++) {
                newline.arr[i] = true;
            }
            // add the 7 comp
            ComputeCode tempCompCode = fileParser.compute();
            for (int i = 0; i < 7; i++) {  
                newline.arr[i + 3] = tempCompCode.value[i];
            }
            // add the 3 dest
            DestinationCode tempDestCode = fileParser.destination();
            for (int i = 0; i < 3; i++) {
                newline.arr[i + 10] = tempDestCode.value[i];
            }
            // add the 3 jump
            JumpCode tempJumpCode = fileParser.jump();
            for (int i = 0; i < 3; i++) {
                newline.arr[i + 13] = tempJumpCode.value[i];
            }
            hackCode.addLine(newline);
        }
        
    }

    fileManager.closeInputFile();

    // write hack Programme to file
    fileManager.openOutputFile();

    if (testOutput) cout << "Testing - Programme Content" << endl;

    cout << "Writing " << fileManager.getOutputFileName() << " file." << endl;

    for (int i = 0; i < hackCode.getLineCount(); i++) {
        string lineToOutput = hackCode.popLineToString();
        fileManager.addLineToOutputFile(lineToOutput);
        if (testOutput) cout << "Line " << i + 1 << " : " << lineToOutput << endl;
    }

    fileManager.closeOutputFile();

    cout << "Great Success!!! About to exit..." << endl;

    return 0;
}


/* Supporting Code */

/*
    isNumber: Checks a String to see if is a number
*/
bool isNumber(string symbol) {
    bool result = true;
    for (int i = 0; i < symbol.length(); i++) {
        if (isdigit(symbol[i]) == false) result = false;
    }
    return result;
}

int stringToInt(string symbol){
    return stoi(symbol);
}

HackLineOfCode intToHackLineOfCode(int number) {

    HackLineOfCode newCodeLine;
    for (int i = 0; i < 16; i++) {
        newCodeLine.arr[i] = false;
    }

    int counter = 15;

    while (number > 0) {
        if (number % 2 == 0) newCodeLine.arr[counter] = false;
        else newCodeLine.arr[counter] = true;

        number >>= 1;
        counter--;
    }    

    return newCodeLine;

}


/* TEST CODE*/

void numberTest() {
    cout << "## Number Test ##" << endl;
    string test1 = "15289";
    string test2 = "A452";

    cout << "Test 1: " << test1 << " is number = " << isNumber(test1) << " which is an integer " << stringToInt(test1) << endl;
    HackLineOfCode test1AsCode = intToHackLineOfCode(stringToInt(test1));
    cout << "{";
    for (int i = 0; i < 16; i++) {
        cout << test1AsCode.arr[i] << ",";
    }
    cout << endl;
    cout << "Test 2: " << test2 << " is number = " << isNumber(test2) << endl;

}

void testFileManager(FileManager* fileManager, bool fullTest) {
    cout << "Testing File Manager:" << endl;
    cout << "Input filename: " << fileManager->getBaseFileName() << endl;
    cout << "Output filename: " << fileManager->getOutputFileName() << endl;
}


void testFileParser(Parser* fileParser, bool fullTest) {
    cout << fileParser->getCurrentLine() << endl;
}

void testAdd(HackCode* hackCode, bool fullTest) {
    HackLineOfCode temp;
    int lineCount = 1;
    if (fullTest) lineCount = 100;
    //temp1.arr[] = {1,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1};
    for (int i = 0; i < lineCount; i++) {
        HackLineOfCode temp;
        for (int x = 0; x < 16; x++) {
            if (x > 0 && i % x == 0) {
                temp.arr[x] = 1;
            }
            else {
                temp.arr[x] = 0;
            }
        }

        hackCode->addLine(temp);
    }

    cout << "Printing testAdd output:" << endl;

    for (int i = 0; i < lineCount; i++) {
        cout << i << ":" << hackCode->popLineToString() << endl;
    }

    // reset hackCode
    hackCode->clearContents();
}


void helperTest(HackCodeTranslation* translationHelper, bool fullTest) {
    helperTestPrint("MD", translationHelper, fullTest);
    if (fullTest) {
        helperTestPrint("D", translationHelper, fullTest);
        helperTestPrint("null", translationHelper, fullTest);
        helperTestPrint("JGT", translationHelper, fullTest);
        helperTestPrint("JMP", translationHelper, fullTest);
        helperTestPrint("A-1", translationHelper, fullTest);
        helperTestPrint("D-A", translationHelper, fullTest);
    }
}

void helperTestPrint(string testString, HackCodeTranslation* translationHelper, bool fullTest) {
    cout << "Test: " << testString << endl;
    cout << "dest: " << translationHelper->dest(testString).value[0] << translationHelper->dest(testString).value[1] << translationHelper->dest(testString).value[2] << endl;
    if (fullTest) {
        cout << "jump: " << translationHelper->jump(testString).value[0] << translationHelper->jump(testString).value[1] << translationHelper->jump(testString).value[2] << endl;
        cout << "comp: " << translationHelper->comp(testString).value[0] << translationHelper->comp(testString).value[1] << translationHelper->comp(testString).value[2] << endl;
    }
    
}

void symbolTest(SymbolTable* symbolTable, bool fullTest) {
    cout << "## Symbol Table Test ## " << endl;
    if (fullTest){
        cout << symbolTable->getAddress("R4") << endl;
        cout << symbolTable->contains("KBD") << endl;
        cout << symbolTable->contains("Hello!") << endl;
        cout << symbolTable->addEntry("Var1", symbolTable->getFreeMemoryLocation(), true) << endl;
        cout << symbolTable->getAddress("Var1") << endl;
        cout << symbolTable->addEntry("Var2", symbolTable->getFreeMemoryLocation(), true) << endl;
        cout << symbolTable->getAddress("Var2") << endl;
        cout << symbolTable->addEntry("Lab1", 79, false) << endl;
        cout << symbolTable->getAddress("Lab1") << endl;
        cout << symbolTable->getFreeMemoryLocation() << endl;
    }
    else {
        cout << symbolTable->addEntry("Var1", symbolTable->getFreeMemoryLocation(), true) << endl;
        cout << symbolTable->getAddress("Var1") << endl;
    }
    
}

/*
    lineTypeTest: test to run through file and print out the type for each line
*/
void lineTypeTest(Parser* fileParser, bool fullTest) {

    string typeValue[] = { "A_COMMAND", "C_COMMAND", "L_COMMAND", "NOT_COMMAND" };

    
    while (fileParser->hasMoreCommands()) {
        fileParser->advance();
        CommandType lineType = fileParser->commandType();
        
        string type = "";
        if (lineType == CommandType::A_COMMAND) {
            type = "A Command";
        }
        else if (lineType == CommandType::C_COMMAND) {
            type = "C Command";
        }
        else if (lineType == CommandType::L_COMMAND) {
            type = "L Command";
        }
        else if (lineType == CommandType::NOT_COMMAND) {
            type = "Not Command";
        }

        string dest = "";
        for (int i = 0; i < 3; i++) {
            if (fileParser->destination().value[i]) {
                dest += '1';
            }
            else {
                dest += '0';
            }
        }

        string jump = "";
        for (int i = 0; i < 3; i++) {
            if (fileParser->jump().value[i]) {
                jump += '1';
            }
            else {
                jump += '0';
            }
        }

        string comp = "";
        for (int i = 0; i < 7; i++) {
            if (fileParser->compute().value[i]) {
                comp += '1';
            }
            else {
                comp += '0';
            }
        }
  
        cout << "Line " << fileParser->getLineCount() << " is of type " << type << " and has a symbol of " << fileParser->symbol() << " and destination of " << dest << ", jump of " << jump << " and comp of " << comp << endl;

    }

}
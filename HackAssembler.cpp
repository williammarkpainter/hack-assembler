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
#include "HackProgram.h"            // Provide the class and functionality to store the machine code in vector of vector 16 bools (needs to be optimised to 16 bit) 
#include "HackCodeTranslation.h"    // Return the Op codes for translation of Assembly Code strings value to components of the machine code, operates in structs with arrays of bools (needs to be optimised to bits)
#include "SymbolTable.h"            // Tracks the RAM and ROM locations of pre-define symbols (RAM), user variables (RAM), and Labels (ROM)
#include "HackParser.h"             // Parses each line in the input file

using namespace std;
using namespace hackassembler;

void testAdd(HackCode *hackCode);
void helperTest(HackCodeTranslation *translationHelper);
void helperTestPrint(string testString, HackCodeTranslation *translationHelper);
void symbolTest(SymbolTable *symbolTable);


int main(int argc, char *argv[]){

    // check argument has been passed
    if(argc < 2){
        cout << "No filename argument provided. Programme will not execute." << endl;
        return -1;
    }

    // create file manager and check if valid file name
    FileManager fileManager(argv[1]);

    if(fileManager.isValidFileName() != true){
        cout << "Filename : " << argv[1] << " was not valid. Programme will not execute." << endl;
        return -2;
    }

    // check if file can be open

    fileManager.openInputFile();
    if(!fileManager.isInputFileOpen()){
        cout << "Filename : " << argv[1] << " could not be opened. Programme did not execute." << endl;
        return -3;
    }

    // check output file can be created and open it

    fileManager.openOutputFile();
    if(!fileManager.isOutputFileOpen()){
        cout << "Unable to open file for output with the name : " << fileManager.getOutputFileName() << ". Programme did not execute." << endl;
        return -4;
    }

    // Build Symbol Table and Initiate Code Module

    HackCodeTranslation translationHelper;
    helperTest(&translationHelper);

    
    // Initiate Parser

    Parser fileParser(fileManager.refInputFile());
    cout << "Testing File Parser:" << endl;
    cout << fileParser.getCurrentLine() << endl;


    // First Pass of File to build Symbol Table for labels

    SymbolTable symbolTable;
    symbolTest(&symbolTable);


    // Second Pass to wirte program to file
    HackCode hackCode;
  
    cout << "Great Success!!! About to exit..." << endl;

    return 0;
}

void testAdd(HackCode *hackCode){
    HackLineOfCode temp1;
    //temp1.arr[] = {1,0,1,0,0,0,1,0,0,1,1,0,0,1,0,1};
    for(int i = 0; i < 100; i++){
        HackLineOfCode temp;
        for(int x = 0; x <16; x++){
            if(i % 16 == 0){
                temp.arr[x] = 1;
            } else {
                temp.arr[x] = 0;
            }
        }
    
        hackCode->addLine(temp);
    }

    for(int i = 0; i<hackCode->getLineCount(); i++){
        cout << i << ":" << hackCode->popLineToString() << endl;
    }
}


void helperTest(HackCodeTranslation *translationHelper){
    helperTestPrint("MD", translationHelper);
    helperTestPrint("D", translationHelper);
    helperTestPrint("null", translationHelper);
    helperTestPrint("JGT", translationHelper);
    helperTestPrint("JMP", translationHelper);
    helperTestPrint("A-1", translationHelper);
    helperTestPrint("D-A", translationHelper);
}

void helperTestPrint(string testString, HackCodeTranslation *translationHelper){
    cout << "Test: " << testString << endl;
    cout << "dest: " << translationHelper->dest(testString).value[0] << translationHelper->dest(testString).value[1] << translationHelper->dest(testString).value[2] << endl;
    cout << "jump: " << translationHelper->jump(testString).value[0] << translationHelper->jump(testString).value[1] << translationHelper->jump(testString).value[2] << endl;
    cout << "comp: " << translationHelper->comp(testString).value[0] << translationHelper->comp(testString).value[1] << translationHelper->comp(testString).value[2] << endl;
}

void symbolTest(SymbolTable *symbolTable){
    cout << "## Symbol Table Test ## " << endl;
    cout << symbolTable->getAddress("R4") << endl;
    cout << symbolTable->contains("KBD") << endl;
    cout << symbolTable->contains("Hello!") << endl;
    cout << symbolTable->addEntry("Var1",symbolTable->getFreeMemoryLocation(), true) << endl;
    cout << symbolTable->getAddress("Var1") << endl;
    cout << symbolTable->addEntry("Var2",symbolTable->getFreeMemoryLocation(), true) << endl;
    cout << symbolTable->getAddress("Var2") << endl;
    cout << symbolTable->addEntry("Lab1",79, false) << endl;
    cout << symbolTable->getAddress("Lab1") << endl;
    cout << symbolTable->getFreeMemoryLocation() << endl;
}
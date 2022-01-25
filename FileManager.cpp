/*
    FileManager for the Hack Assesmbler
        This file provides the functions for opening, reading, and writing the files needed for the Assesmbler
    Author  :   williammarkpainter@gmail.com
    Date    :   17th Jan 2022
*/

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include "FileManager.h"

using namespace std;
using namespace hackassembler;

/* CONSTRUCTORS */

FileManager::FileManager(string fileName){
        setFileNames(fileName);
}
FileManager::~FileManager(){
    if(inputFileOpen){
        closeInputFile();
    }
}

/* METHODS */

/* Simple GET methods*/
string FileManager::getSourceFileName(){return sourceFileName;}
string FileManager::getBaseFileName(){return baseFileName;}
string FileManager::getOutputFileName(){return outputFileName;}
bool FileManager::isInputFileOpen(){return inputFileOpen;}
bool FileManager::isOutputFileOpen(){return outputFileOpen;}
bool FileManager::isValidFileName(){return validFileName;}
fstream FileManager::*refInputFile(){return ptr_inputFile;}
/*
    setFileNames: check if the input name is valid, then creates outout file names
*/
void FileManager::setFileNames(string fileName){
    sourceFileName = fileName;
    validFileName = checkValidFileName();
    if(validFileName){
        int strLength = sourceFileName.length();
        baseFileName = sourceFileName.substr(0,strLength-4);
        outputFileName = baseFileName + ".hack";
    } 
}
/*
    checkValidFileName: checks the input file name is valid and ending in .asm
*/
bool FileManager::checkValidFileName(){
    int strLength = sourceFileName.length();
    if(strLength > 4 && sourceFileName.substr(strLength-4) == ".asm"){
        return true;
    } else {
        return false;
    }
}
/*
    openInputFile: opens the input file
*/
bool FileManager::openInputFile(){
    
    // if fileOpen do nothing
    if(inputFileOpen) return true;

    // open file for input
    inputFile.open(sourceFileName, ios::in);
    if(!inputFile){
        inputFileOpen = false;
        return false;
    } else {
        inputFileOpen = true;
        ptr_inputFile = &inputFile;
        return true;
    }
}
/*
    closeInputFile: closes the input file
*/
bool FileManager::closeInputFile(){
    
    // if inputFileOpen is not true do nothing
    if(!inputFileOpen) return true;

    // open file for input
    inputFile.close();
    inputFileOpen = false;
    return true;
}
/*
    openInputFile: opens the input file
*/
bool FileManager::openOutputFile(){
    
    // if fileOpen do nothing
    if(outputFileOpen) return true;

    // open file for input
    outputFile.open(outputFileName, ios::out);
    if(!outputFile){
        outputFileOpen = false;
        return false;
    } else {
        outputFileOpen = true;
        return true;
    }
}
/*
    closeInputFile: closes the input file
*/
bool FileManager::closeOutputFile(){
    
    // if inputFileOpen is not true do nothing
    if(!outputFileOpen) return true;

    // open file for input
    outputFile.close();
    outputFileOpen = false;
    return true;
}
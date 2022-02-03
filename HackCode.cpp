/*
    HackCode is part of the HackAssembler, is a basic structure for 
        hollding the binary code once translated.
        This binary code is then outputted as a string of '0' and '1' chars to a text file
*/

#include <string>
#include <iostream>
#include "HackCode.h"
#include <queue>


using namespace std;
using namespace hackassembler;

HackCode::HackCode(){
    lineCount = 0;
    writeCount = 0;
}

HackCode::~HackCode(){
}

int HackCode::getLineCount(){return lineCount;};
int HackCode::getWriteCount(){return writeCount;};

/*
    addLine: add a new HackLineOfCode to the end of the queue
*/
int HackCode::addLine(HackLineOfCode newLine){
    hackLinesOfCode.push(newLine);
    lineCount++;
    return lineCount;
}
/*
    pushLineToString: pop the top line in the queue and returns a string representation
*/
string HackCode::popLineToString(){
    
    HackLineOfCode temp = hackLinesOfCode.front();
    hackLinesOfCode.pop();
    
    writeCount++;
    string strBools="";

    for(int i = 0; i < 16; i++){
        strBools+= (temp.arr[i]==1)?'1':'0';
    }
    return strBools;
}

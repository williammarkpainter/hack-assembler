#include <string>
#include <iostream>
#include <queue>

using namespace std;

#ifndef HACK_PROGRAM_FILE_H_
#define HACK_PROGRAM_FILE_H_

namespace hackassembler{

struct HackLineOfCode
{
    bool arr[16];
};


class HackCode{
private:
    queue<HackLineOfCode> hackLinesOfCode;
    int lineCount;
    int writeCount;
public:
    HackCode();
    ~HackCode();
    int addLine(HackLineOfCode newLine);
    int getLineCount();
    int getWriteCount();
    string popLineToString();
};



} /* hackassembler namespace */

#endif /*  */
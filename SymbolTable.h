#include <iostream>
#include <string>
#include <map>

using namespace std;

#ifndef _SYMBOL_TABLE_
#define _SYMBOL_TABLE_

namespace hackassembler{

class SymbolTable{
private:
    map<string, int> mappingTable;
    int freeMemoryLocation;
public:
    SymbolTable();
    ~SymbolTable();
    bool addEntry(string symbol, int address, bool isMemoryLocation);
    bool contains(string symbol);
    int getAddress(string symbol);
    int getFreeMemoryLocation(){return freeMemoryLocation;}    
};

}       /* hackassembler */
#endif    /* _SYMBOL_TABLE_ */
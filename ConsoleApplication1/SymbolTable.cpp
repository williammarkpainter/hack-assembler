/*
    SymbolTable: Contains the HashTable to lookup RAM and ROM memory locations for Symbols used in the Assembly Code
                    Users can add their own symbols and lables in the Assembly Code, this table tracks program lines for labels and memory address for variables
*/
#include <string>
#include <map>
#include "SymbolTable.h"

using namespace std;
using namespace hackassembler;

/* CONSTRUCTORS */
SymbolTable::SymbolTable() {

    // intiate map with initial values
    mappingTable.insert(make_pair("SP", 0));
    mappingTable.insert(make_pair("LCL", 1));
    mappingTable.insert(make_pair("ARG", 2));
    mappingTable.insert(make_pair("THIS", 3));
    mappingTable.insert(make_pair("THAT", 4));
    mappingTable.insert(make_pair("R0", 0));
    mappingTable.insert(make_pair("R1", 1));
    mappingTable.insert(make_pair("R2", 2));
    mappingTable.insert(make_pair("R3", 3));
    mappingTable.insert(make_pair("R4", 4));
    mappingTable.insert(make_pair("R5", 5));
    mappingTable.insert(make_pair("R6", 6));
    mappingTable.insert(make_pair("R7", 7));
    mappingTable.insert(make_pair("R8", 8));
    mappingTable.insert(make_pair("R9", 9));
    mappingTable.insert(make_pair("R10", 10));
    mappingTable.insert(make_pair("R11", 11));
    mappingTable.insert(make_pair("R12", 12));
    mappingTable.insert(make_pair("R13", 13));
    mappingTable.insert(make_pair("R14", 14));
    mappingTable.insert(make_pair("R15", 15));
    mappingTable.insert(make_pair("SCREEN", 16384));
    mappingTable.insert(make_pair("KBD", 24576));
    // first free memory location is 16 for first user varible
    freeMemoryLocation = 16;
}

SymbolTable::~SymbolTable() {

}

/* METHODS */
bool SymbolTable::addEntry(string symbol, int address, bool isMemoryLocation) {
    if (contains(symbol)) {
        return false;
    }
    mappingTable.insert(make_pair(symbol, address));
    if (isMemoryLocation) freeMemoryLocation++;
    return true;
}
bool SymbolTable::contains(string symbol) {
    return(mappingTable.count(symbol));
}
int SymbolTable::getAddress(string symbol) {
    return(mappingTable[symbol]);
}
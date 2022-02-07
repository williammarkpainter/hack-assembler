/*
    HackCodeTransalation is a module in the Hack Assembler that converts the
        string values in a C command to their binary value in the hack machine code.
        The C command has 3 parts, a compute, a desitnation, and a jump
*/


#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "HackCodeTranslation.h"

using namespace std;
using namespace hackassembler;

HackCodeTranslation::HackCodeTranslation()
{
    this->populateDestinationLookup();
    this->populateComputeMap();
    this->populateJumpMap();
}

HackCodeTranslation::~HackCodeTranslation()
{
}
/*
    Populate Destination Map
*/
DestinationMap HackCodeTranslation::destinationLookup = HackCodeTranslation::populateDestinationLookup();
DestinationMap HackCodeTranslation::populateDestinationLookup() {
    DestinationMap destLookup = {
        {"null", {0,0,0}},
        {"M", {0,0,1}},
        {"D", {0,1,0}},
        {"MD", {0,1,1}},
        {"A", {1,0,0}},
        {"AM", {1,0,1}},
        {"AD", {1,1,0}},
        {"AMD", {1,1,1}}
    };
    return destLookup;
}
/*
    Populate Compute Map
*/
ComputeMap HackCodeTranslation::computeLookup = HackCodeTranslation::populateComputeMap();
ComputeMap HackCodeTranslation::populateComputeMap() {
    ComputeMap computeLookup = {
        {"0", {0,1,0,1,0,1,0}},
        {"1", {0,1,1,1,1,1,1}},
        {"-1", {0,1,1,1,0,1,0}},
        {"D", {0,0,0,1,1,0,0}},
        {"A", {0,1,1,0,0,0,0}},
        {"!D", {0,0,0,1,1,0,1}},
        {"!A", {0,1,1,0,0,0,1}},
        {"-D", {0,0,0,1,1,1,1}},
        {"-A", {0,1,1,0,0,1,1}},
        {"D+1", {0,0,1,1,1,1,1}},
        {"A+1", {0,1,1,0,1,1,1}},
        {"D-1", {0,0,0,1,1,1,0}},
        {"A-1", {0,1,1,0,0,1,0}},
        {"D+A", {0,0,0,0,0,1,0}},
        {"D-A", {0,0,1,0,0,1,1}},
        {"A-D", {0,0,0,0,1,1,1}},
        {"D&A", {0,0,0,0,0,0,0}},
        {"D|A", {0,0,1,0,1,0,1}},
        {"M", {1,1,1,0,0,0,0}},
        {"!M", {1,1,1,0,0,0,1}},
        {"-M", {1,1,1,0,0,1,1}},
        {"M+1", {1,1,1,0,1,1,1}},
        {"M-1", {1,1,1,0,0,1,0}},
        {"D+M", {1,0,0,0,0,1,0}},
        {"D-M", {1,0,1,0,0,1,1}},
        {"M-D", {1,0,0,0,1,1,1}},
        {"D&M", {1,0,0,0,0,0,0}},
        {"D|M", {1,0,1,0,1,0,1}}
    };
    return computeLookup;
}
/*
    Populate Jump Map
*/
JumpMap HackCodeTranslation::jumpLookup = HackCodeTranslation::populateJumpMap();
JumpMap HackCodeTranslation::populateJumpMap() {
    JumpMap jumpLookup = {
        {"null", {0,0,0}},
        {"JGT", {0,0,1}},
        {"JEQ", {0,1,0}},
        {"JGE", {0,1,1}},
        {"JLT", {1,0,0}},
        {"JNE", {1,0,1}},
        {"JLE", {1,1,0}},
        {"JMP", {1,1,1}}
    };
    return jumpLookup;
}
/*
    destination, computer, and jump functions.
*/
DestinationCode HackCodeTranslation::dest(string lookupText) {

    DestinationCode result = this->destinationLookup[lookupText];

    return result;
}
ComputeCode HackCodeTranslation::comp(string lookupText) {

    ComputeCode result = this->computeLookup[lookupText];

    return result;
}
JumpCode HackCodeTranslation::jump(string lookupText) {

    JumpCode result = this->jumpLookup[lookupText];

    return result;
}


#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <utility>

using namespace std;

#ifndef HACK_CODE_TRANSLATION_H_
#define HACK_CODE_TRANSLATION_H_

namespace hackassembler{

struct DestinationCode{
    bool value[3];
};
struct ComputeCode{
    bool value[7];
};
struct JumpCode{
    bool value[3];
};

typedef map<string, DestinationCode> DestinationMap;
typedef map<string, ComputeCode> ComputeMap;
typedef map<string, JumpCode> JumpMap;

class HackCodeTranslation
{
private:
    static DestinationMap destinationLookup;
    static ComputeMap computeLookup;
    static JumpMap jumpLookup;
    static DestinationMap populateDestinationLookup();
    static ComputeMap populateComputeMap();
    static JumpMap populateJumpMap();
public:
    HackCodeTranslation();
    ~HackCodeTranslation();
    DestinationCode dest(string codeText);
    ComputeCode comp(string codeText);
    JumpCode jump(string codeText);
};

} /* hackassembler */

#endif /* HACK_CODE_TRANSLATION_H_ */
#include "LoveDataBase.hpp"

using namespace std;

int main(){
    LoveDataBase baza;
    baza.addCandidate();
    baza.addCandidate();
    baza.addCandidate();
    loveDataBase2file(baza);

    //LoveDataBase baza = loveDataBasefromfile("LoveDataBase.xml");
    //baza.candidates["brtek"].print_all();
    //pair<string,string> tag1{"favAnimal","dog"};
    //pair<string,string> tag2{"sex","male"};
    //vector<pair<string,string>> tags{tag1,tag2};
    //auto match = baza.findCandidateByTag(tags);
    //baza.candidates[match[0]].print_all();
};


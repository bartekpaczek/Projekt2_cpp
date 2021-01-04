#include "Person.hpp"
#include <vector>
#include <fstream>
#include <utility>

using namespace std;

class LoveDataBase{
    private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(candidates);        
    }
    
    public:
    string storeFileName = "LoveDataBase.xml";
    map<string, Candidate> candidates;
    map<string, map<string, vector<string>>> mapedCandidates;
    


    LoveDataBase(){
        mapedCandidates["sex"];
        mapedCandidates["age"];
        mapedCandidates["hight"];
        mapedCandidates["eyeColor"];
        mapedCandidates["hair"];
        mapedCandidates["favAnimal"];
    };
 
    void addCandidate(){
        Candidate newCandidate;
        newCandidate.provide_data();
        candidates[newCandidate.name] = newCandidate;
        mapCandidate(newCandidate);
    };

    void mapCandidate(Candidate candidate){        
        mapedCandidates["sex"][candidate.sex].push_back(candidate.name);
        mapedCandidates["age"][candidate.age].push_back(candidate.name);
        mapedCandidates["hight"][candidate.hight].push_back(candidate.name);
        mapedCandidates["eyeColor"][candidate.eyeColor].push_back(candidate.name);
        mapedCandidates["hair"][candidate.hair].push_back(candidate.name);
        mapedCandidates["favAnimal"][candidate.favAnimal].push_back(candidate.name);
    };

    vector<string> findCandidateByTag(vector<pair<string, string>> tagList){
        vector<vector<string>> midSearchResult;
        vector<string> finalSearchResult;
        for(auto it=tagList.begin();it!=tagList.end(); ++it){
            midSearchResult.push_back(mapedCandidates[it->first][it->second]);
        };

        if(!midSearchResult.empty() & midSearchResult.size()!=1){
            for(auto it=midSearchResult.begin(); it!=midSearchResult.end(); ++it){
                if(it==midSearchResult.begin()){
                    finalSearchResult = *it;
                }
                else{
                    sort(finalSearchResult.begin(), finalSearchResult.end());
                    sort(it->begin()+1, it->end());

                    set_intersection(finalSearchResult.begin(), finalSearchResult.end(),
                                    it->begin()+1, it->end(),
                                    finalSearchResult.begin());
                }
                if(finalSearchResult.empty()){
                    break;
                };
            };
        }else if(midSearchResult.size()==1){
            finalSearchResult = midSearchResult[0];
        };
        return finalSearchResult;
    }
};

LoveDataBase loveDataBasefromfile(string fileName){
    LoveDataBase database;
    {
       // create and open an archive for input
       std::ifstream ifs(fileName);
       boost::archive::xml_iarchive ia(ifs);
       // read class state from archive
       ia >> BOOST_SERIALIZATION_NVP(database);
       // archive and stream closed when destructors are called
    }
    //mapping all the candidates
    for(auto it = database.candidates.begin(); it!=database.candidates.end(); ++it){
        database.mapCandidate(it->second);
    }
    return database;
};

void  loveDataBase2file(const LoveDataBase database){
        std::ofstream ofs(database.storeFileName);
            // save data to archive
        {
            boost::archive::xml_oarchive oa(ofs);
            // write class instance to archive
            oa << BOOST_SERIALIZATION_NVP(database);
            // archive and stream closed when destructors are called
        }
    };

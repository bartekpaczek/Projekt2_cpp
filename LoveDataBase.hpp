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
            midSearchResult.push_back((mapedCandidates.at(it->first)).at(it->second));
        };

        if(!midSearchResult.empty() & midSearchResult.size()!=1){
            for(auto it=midSearchResult.begin(); it!=midSearchResult.end(); ++it){
                if(it==midSearchResult.begin()){
                    finalSearchResult = *it;
                }
                else{
                    vector<string> tempResult(1);
                    sort(finalSearchResult.begin(), finalSearchResult.end());
                    sort(it->begin(), it->end());

                    set_intersection(finalSearchResult.begin(), finalSearchResult.end(),
                                    it->begin(), it->end(),
                                    tempResult.begin());
                    finalSearchResult = tempResult;
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



class LDBOperator{
    LoveDataBase dataBase;
    const map<string, int> startCmdList{
        {"new", 0},
        {"file", 1},
        {"quit", 2}
    };

    const map<string, int> mainCmdList{
        {"list", 0},
        {"name", 1},
        {"find", 2},
        {"add", 3},
        //{"auto", 4},
        {"quit", 5}
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

    public:
    LDBOperator(){};
    bool strartAction(){
        bool continueLDB{1};
        string cmd;
        cout<<"\nnew - data base with no records"
              "\nfile - data base from file"
              "\nquit - exit data base\n$";
        cin>>cmd;
        int cmd_no{startCmdList.at(cmd)};
        switch (cmd_no){
        case 0:
            break;
        case 1:{
            string fileName;
            cout<<"Provide file name (\"filename.xml\"): ";
            cin>>fileName;
            dataBase = loveDataBasefromfile(fileName);
            break;
        }
        case 2:
            continueLDB = 0;
        };
        return continueLDB;        
    };

    bool mainAction(){
        bool continueLDB{1};
        string cmd;
        cout<<"\nlist - list all the Love Data Base members"
              "\nname - find person by name, and print its data"
              "\nfind - find candidates by features"
              "\nadd  - add candidate"
              //"\nauto - find ideal partner for specific candidate"
              "\nquit - save and close Love Data Base\n$";
        cin>>cmd;
        int cmd_no{mainCmdList.at(cmd)};
        switch(cmd_no){
        case 0:
            for(auto it=dataBase.candidates.begin(); it!=dataBase.candidates.end(); ++it){
                it->second.print_all();
            }
            break;
        case 1:{
            string personName;
            cout<<"\nProvide person full name (John Smith): ";
            cin>>personName;
            dataBase.candidates.at(personName).print_all();
            break;
        }    
        case 2:{
            bool moreParameters{1};
            string category;
            string catValue;
            pair<string, string> tag;
            vector<pair<string,string>> tags;
            
            while(moreParameters){
                cout<<"\nAvailable search categories: sex, age, hight, eyeColor, hair, favAnimal";
                cout<<"\nProvide search category: ";
                cin>>category;
                cout<<"\nProvide search value for category e.g: male: ";
                cin>>catValue;
                tag = {category, catValue};
                tags.push_back(tag);
                cout<<"Do you want to add more search requirements? [1/0]: ";
                cin>>moreParameters;
            };
            auto foundCandidates = dataBase.findCandidateByTag(tags);
            if(!foundCandidates.empty()){
                cout<<"\nSuch candidates were found:\n";
                for(auto it : foundCandidates){
                    cout<<it<<"\n";
                };
            }else{
                cout<<"No candidates with such features found :(. Lower the requirements.";
            };
            break;
        };
        case 3:{
            dataBase.addCandidate();
            break;
        };
        //case 4:{
        //        string personName;
        //        cout<<"\nProvide person full name (John Smith): ";
        //        cin>>personName;
        //        dataBase.candidates.at(personName).print_all();
        //        auto perfectCandidates = dataBase.findCandidateByTag(dataBase.candidates.at(personName).ideal);
        //        break;
        //    };
        case 5:{
                loveDataBase2file(dataBase);
                continueLDB = 0;
                break;
            }
        };
        return continueLDB;
    };
    
    void start(){
        bool LDBcontinue{1};
        bool startFinished{0};
        while(LDBcontinue){
            try{
                if(!startFinished){                
                    LDBcontinue = this->strartAction();
                    startFinished = 1;
                }else{
                    LDBcontinue = this->mainAction();
                };
            }catch(const boost::archive::archive_exception& e){
                cout<<"\nNo such file exist or the file structure is wrong.\n";
            }catch(const out_of_range& e){
                cout<<"\nInvalid command. Follow the instructions.\n";
            }
        }
    }
};
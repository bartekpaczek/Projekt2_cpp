#include <string>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>

// include headers that implement a archive in simple xml
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/map.hpp>

using namespace std;

vector<string> sexTypeList = {"female","male"};
vector<string> eyeColorsList = {"blue", "green", "brown", "black"};
vector<string> hairColorsList = {"bold", "blond", "brunet", "red", "gray"};
vector<string> animalsList = {"none", "cat","dog"};

struct Ideal{
    //Basic data
    string sex; 
    string age;

    //Aperance data
    string hight; //in cm
    string eyeColor;
    string hair;

    //Person data
    string favAnimnal;
    Ideal(){};
};

struct Candidate{
    private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(name);
        ar & BOOST_SERIALIZATION_NVP(sex);
        ar & BOOST_SERIALIZATION_NVP(age);
        ar & BOOST_SERIALIZATION_NVP(hight);
        ar & BOOST_SERIALIZATION_NVP(eyeColor);
        ar & BOOST_SERIALIZATION_NVP(hair);
        ar & BOOST_SERIALIZATION_NVP(favAnimal);
        ar & BOOST_SERIALIZATION_NVP(email);
        ar & BOOST_SERIALIZATION_NVP(phoneNumber);        
    }

    public:
    string name;
    string sex; 
    string age;

    //Aperance data
    string hight; //in cm
    string eyeColor;
    string hair;

    //Person data
    string favAnimal;

    //Contact data
    string email;
    string phoneNumber;

    Candidate(){};
    void provide_data(){
        cout<<"\nProvide full name: "; cin>>name;
        sex = inputDataValidation(sexTypeList, "\nProvide sex (female or male): ");
        cout<<"\nProvide age: "; cin>>age;
        cout<<"\nProvide hight: "; cin>>hight;
        eyeColor = inputDataValidation(eyeColorsList, "\nProvide eye color (blue, green, brown, black): ");
        hair = inputDataValidation(hairColorsList, "\nProvide hair color (bold, blond, brunet, red, gray): ");
        favAnimal = inputDataValidation(animalsList, "\nProvide favourite animal: (none, cat, dog): ");
        cout<<"\nProvide e-mail: "; cin>>email;
        cout<<"\nProvide phone number: ";cin>>phoneNumber;
    };

    void print_all(){
        cout<<"\n"<<name<<
        "\nSex: "<<sex<<
        "\nAge: "<<age<<
        "\nHight: "<<hight<<
        "\nEye color: "<<eyeColor<<
        "\nHair color: "<<hair<<
        "\nFavourite animal: "<<favAnimal<<
        "\n----------------------------------"<<
        "\nPhone number: "<<phoneNumber<<
        "\nE-mail:"<<email;        
    };

    void print_name(){
        cout<<name<<"\n";
    };


    string inputDataValidation(vector<string>& paramTypeList, string queryText){
        bool incorrectInput{1};
        string inputVal;
        string outputVal;
        while(incorrectInput){
            cout<<queryText;cin>>inputVal;
            auto iterator = find(std::begin(paramTypeList), end(paramTypeList), inputVal);
            if(iterator==paramTypeList.end()){
                cout<<"\nInvalid data. Follow directions in brackets. Try again.";
            }else{
                incorrectInput = 0;
                outputVal = inputVal;
            };
        }
        return outputVal;
    };
};
#include <fstream>
#include <string>
#include <map>

// include headers that implement a archive in simple text format
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/map.hpp>
using namespace std;

/////////////////////////////////////////////////////////////
// gps coordinate
//
// illustrates serialization for a simple type
//
class lol{
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(trolo);        
    }
    string trolo;
    int x;
    
    public:
    lol(){};
    lol(string input):trolo(input){};
};
class gps_position
{
private:
    friend class boost::serialization::access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(degrees);
        ar & BOOST_SERIALIZATION_NVP(minutes);
        ar & BOOST_SERIALIZATION_NVP(seconds);
        ar & BOOST_SERIALIZATION_NVP(miejsce);
        ar & BOOST_SERIALIZATION_NVP(a);
        
    }
    int degrees;
    int minutes;
    float seconds;
    std::string miejsce;
    lol a;

public:
    gps_position(){};
    gps_position(int d, int m, float s, std::string mi, lol b) :
        degrees(d), minutes(m), seconds(s), miejsce(mi), a(b)
    {}
};

int main() {
    // create and open a character archive for output
    std::ofstream ofs("filename.xml");

    // create class instance
    const gps_position g(35, 59, 24.567f, "dupa", lol("aaa"));
    const map<string, gps_position> a{{"a", g}};

    // save data to archive
    {
        boost::archive::xml_oarchive oa(ofs);
        // write class instance to archive
        oa << BOOST_SERIALIZATION_NVP(a);
    	// archive and stream closed when destructors are called
    }

    // ... some time later restore the class instance to its orginal state
    map<string, gps_position> newg;
    {
        // create and open an archive for input
        std::ifstream ifs("filename.xml");
        boost::archive::xml_iarchive ia(ifs);
        // read class state from archive
        ia >> BOOST_SERIALIZATION_NVP(newg);
        // archive and stream closed when destructors are called
    }
    return 0;
}

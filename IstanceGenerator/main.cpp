#include <iostream>
#include <boost/serialization/serialization.hpp>
#include "problem.h"

#define LINES 4

using namespace std;

int main(int argc, char *argv[])
{
    vector<ComponentModel> comp_models;
    vector<NetworkModel> net_models;
    {
        // create and open an archive for input
        std::ifstream ifs("./Models/component_models.dat");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> comp_models;
        // archive and stream closed when destructors are called
    }
    {
       // create and open an archive for input
       std::ifstream ifs("./Models/network_models.dat");
       boost::archive::text_iarchive ia(ifs);
       // read class state from archive
       ia >> net_models;
       // archive and stream closed when destructors are called
    }

    for(int i=0; i<LINES; i++)
    {
        stringstream ss;
        ss << "N" << i;
        SystemNode sn
        ProblemNode pn(ss.str());
    }


    return 0;
}

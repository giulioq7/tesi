#include "satellitedata.h"

using namespace std;

SatelliteData::SatelliteData()
{
}


SatelliteData::SatelliteData(string str)
{
    name = str;
}


std::ostream& operator<<(std::ostream& out, const SatelliteData& s)
{
    out << s.name;

    return out;
}

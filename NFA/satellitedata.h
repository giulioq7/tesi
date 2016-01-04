#ifndef SATELLITEDATA_H
#define SATELLITEDATA_H

#include <iostream>
#include <set>

class SatelliteData
{
public:
    std::string name;
    std::set<std::set<std::string> > candidate_diagnosis;
    SatelliteData();
    SatelliteData(std::string str);

    SatelliteData& operator=(SatelliteData s) { name = s.name; return *this;}
};

#endif // SATELLITEDATA_H

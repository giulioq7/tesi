#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

using namespace std;

class Component
{
    public:
        Component(string name);
        string getName();
    private:
        string name;
};

#endif // COMPONENT_H

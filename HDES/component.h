#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>
#include <vector>

using namespace std;

class Component
{
public:
    Component(string name);
    ~Component();
    string get_name();
    vector<Component> *get_children();
    bool is_complex();
    void add_child(Component *child);
    string to_string();

private:
    string name;
    vector<Component> *children;
    bool complex;
};

#endif // COMPONENT_H

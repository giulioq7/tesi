#include <iostream>
#include <string>

#include "component.h"

using namespace std;

int main()
{
    cout << "Creating context hierarchy..." << endl;
    Component *l = new Component("l");

    delete l;

    return 0;
}


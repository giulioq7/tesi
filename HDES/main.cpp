#include "component.h"
#include <iostream>

using namespace std;

int main()
{
    cout << "Starting HDES construction..." << endl;

    Component *L  = new Component("L");
    Component *W1 = new Component("W1");
    Component *W2 = new Component("W2");
    Component *l1 = new Component("l1");
    Component *p1 = new Component("p1");
    Component *r1 = new Component("r1");
    Component *l2 = new Component("l2");
    Component *p2 = new Component("p2");
    Component *r2 = new Component("r2");

    L->add_child(W1);
    L->add_child(W2);

    W1->add_child(l1);
    W1->add_child(p1);
    W1->add_child(r1);

    W2->add_child(l2);
    W2->add_child(p2);
    W2->add_child(r2);

    cout <<  L->to_string();
    cout << W1->to_string();
    cout << W2->to_string();
    cout << l1->to_string();
    cout << p1->to_string();
    cout << r1->to_string();
    cout << l2->to_string();
    cout << p2->to_string();
    cout << r2->to_string();


    //free memory
    delete L;
    delete W1;
    delete W2;
    delete l1;
    delete p1;
    delete r1;
    delete l2;
    delete p2;
    delete r2;

    return 0;
}


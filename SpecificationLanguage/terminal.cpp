#include "terminal.h"

using namespace std;

Terminal::Terminal()
{
}

Terminal::Terminal(string n)
{
    name = n;
    value = "<eps>";
}

Terminal::~Terminal()
{
    value.clear();
}

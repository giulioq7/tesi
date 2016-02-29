#include "terminal.h"

using namespace std;

Terminal::Terminal()
{
    value = "<eps>";
}

Terminal::Terminal(string n)
{
    name = n;
    value = "<eps>";
}

std::string Terminal::get_name() const
{
    return name;
}

std::string Terminal::get_value() const
{
    return value;
}

std::vector<Terminal*> Terminal::get_linked_terminals() const
{
    return linked_terminals;
}

void Terminal::set_value(std::string v)
{
    value = v;
}

void Terminal::add_linked_terminal(Terminal* t)
{
    linked_terminals.push_back(t);
}

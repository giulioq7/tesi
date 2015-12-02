#include "pattern.h"

Pattern::Pattern()
{
}


Pattern::Pattern(std::string str)
{
    name = str;
    max_language = false;
}


std::string Pattern::get_name() const
{
    return name;
}

std::string Pattern::get_terminal_id() const
{
    return terminal_id;
}

std::string Pattern::get_expr() const
{
    return expr;
}

bool Pattern::is_max_language() const
{
    return max_language;
}

vector<int> Pattern::get_language() const
{
    return language;
}

void Pattern::set_terminal_id(std::string id)
{
    terminal_id = id;
}

void Pattern::set_expr(std::string str)
{
    expr = str;
}

void Pattern::choose_max_language()
{
    max_language = true;
}

void Pattern::add_trans(int t)
{
    language.push_back(t);
}

void Pattern::assign_language(vector<int> l)
{
    language = l;
}


void Pattern::not_substitution()
{
    while(true)
    {
        int tilde_pos = expr.find("~");
        if(tilde_pos == -1)
            break;
        int bracket_pos = expr.find(")",tilde_pos);
        int len = bracket_pos - tilde_pos - 1;
        string str;
        for(vector<int>::iterator it = language.begin(); it != language.end(); it++)
        {
            if(*it != atoi(expr.substr(tilde_pos+1, len).c_str()))
            {
                stringstream ss;
                ss << *it;
                str.append(ss.str());
                str.append("|");
            }
        }
        //removes last pipe "|"
        str.resize(str.size()-1);
        expr.replace(tilde_pos, len+1, str);
    }
}

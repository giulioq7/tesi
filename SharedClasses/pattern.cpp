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

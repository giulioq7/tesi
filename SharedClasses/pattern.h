#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>

class Pattern
{
    std::string name;
    std::string terminal_id;
    std::string expr;
    bool max_language;

public:
    Pattern();
    Pattern(std::string str);

    std::string get_name();
    std::string get_terminal_id();
    std::string get_expr();
    bool is_max_language();

    void set_terminal_id(std::string id);
    void set_expr(std::string str);
    void chose_max_language();
};

#endif // PATTERN_H

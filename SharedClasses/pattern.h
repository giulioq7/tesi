#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <astl.h>
#include "nettransition.h"

class Pattern
{
    std::string name;
    std::string terminal_id;
    std::string expr;
    bool max_language;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & terminal_id;
        ar & expr;
        ar & max_language;
    }

public:
    Pattern();
    Pattern(std::string str);

    std::string get_name() const;
    std::string get_terminal_id() const;
    std::string get_expr() const;
    bool is_max_language() const;

    void set_terminal_id(std::string id);
    void set_expr(std::string str);
    void choose_max_language();
};

#endif // PATTERN_H

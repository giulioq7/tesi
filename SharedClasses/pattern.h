#ifndef PATTERN_H
#define PATTERN_H

#include <iostream>
#include <algorithm>
#include <sstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <astl.h>
#include "nettransition.h"

/**
 * @brief The Pattern class represents a pattern declaration
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class Pattern
{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief terminal_id name of network output terminal where the pattern event is generated
     */
    std::string terminal_id;
    /**
     * @brief expr regular expression
     */
    std::string expr;
    /**
     * @brief max_language true if the language is maximum, false otherwise
     */
    bool max_language;
    /**
     * @brief language vectors of language transitions (integer mapped values)
     */
    vector<int> language;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & terminal_id;
        ar & expr;
        ar & max_language;
        ar & language;
    }

public:
    /**
     * @brief Pattern default constructor
     */
    Pattern();
    /**
     * @brief Pattern constructor from id
     * @param str name
     */
    Pattern(std::string str);

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_terminal_id
     * @return
     */
    std::string get_terminal_id() const;
    /**
     * @brief get_expr
     * @return
     */
    std::string get_expr() const;
    /**
     * @brief is_max_language
     * @return
     */
    bool is_max_language() const;
    /**
     * @brief get_language
     * @return
     */
    vector<int> get_language() const;

    /**
     * @brief set_terminal_id
     * @param id
     */
    void set_terminal_id(std::string id);
    /**
     * @brief set_expr
     * @param str
     */
    void set_expr(std::string str);
    /**
     * @brief choose_max_language set max_language to true
     */
    void choose_max_language();
    /**
     * @brief add_trans
     * @param t
     */
    void add_trans(int t);
    /**
     * @brief assign_language
     * @param l
     */
    void assign_language(vector<int> l);
    /**
     * @brief not_substitution substitutes not operator with result after language determination
     */
    void not_substitution();
};

#endif // PATTERN_H

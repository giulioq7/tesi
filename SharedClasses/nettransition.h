#ifndef NETTRANSITION_H
#define NETTRANSITION_H

#include "transition.h"
#include "netcomponent.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The NetTransition class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class NetTransition : public CHAR_TRAITS<NetTransition>
{
protected:
    /**
     * @brief trans transition of relative component model
     */
    Transition* trans;
    /**
     * @brief component network component involved
     */
    NetComponent* component;
    /**
     * @brief name
     */
    std::string name;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & trans;
        ar & component;
        ar & name;
    }

public:
    /**
     * @brief NetTransition default constructor
     */
    NetTransition();
    /**
     * @brief NetTransition constructor from pointers to transition and network component
     * @param t
     * @param c
     */
    NetTransition(Transition* t, NetComponent* c);

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_trans
     * @return
     */
    Transition* get_trans() const;
    /**
     * @brief get_component
     * @return
     */
    NetComponent* get_component() const;


    //required definitions to use a NetTransition as automata alphabet for astl lib
    /**
     * @brief char_type
     */
    typedef NetTransition char_type;
    /**
     * @brief int_type
     */
    typedef long        int_type;
    /**
     * @brief size assign value in order to use matrix representation (not specified in this project)
     */
    static const size_t size;
    /**
     * @brief eq
     * @param x
     * @param y
     * @return
     */
    static bool eq(const char_type &x, const char_type &y) { return x == y; }
    /**
     * @brief lt
     * @param x
     * @param y
     * @return
     */
    static bool lt(const char_type &x, const char_type &y) { return x < y; }
    /**
     * @brief operator <
     * @param t
     * @return
     */
    bool operator<(const NetTransition t) const
    {
       return (name < t.get_name());
    }
    /**
     * @brief operator ==
     * @param t
     * @return
     */
    bool operator==(const NetTransition t) const
    {
        return (name == t.get_name());
    }

};

#endif // NETTRANSITION_H

#ifndef NETCOMPONENT_H
#define NETCOMPONENT_H

#include <iostream>
#include "componentmodel.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The NetComponent class represents a component in a network model
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class NetComponent
{
protected:
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief model component model
     */
    ComponentModel* model;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & model;
    }
public:
    /**
     * @brief NetComponent default constructor
     */
    NetComponent();
    /**
     * @brief NetComponent constructor from id
     * @param str name
     */
    NetComponent(std::string str);

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_model
     * @return pointer to ComponentModel
     */
    ComponentModel* get_model() const;

    /**
     * @brief set_model assigns model
     * @param cm
     */
    void set_model(ComponentModel* cm);

    /**
     * @brief operator == overload of equality operator
     * @param c
     * @return
     */
    bool operator==(const NetComponent c) const {return name == c.name;}
};

#endif // NETCOMPONENT_H

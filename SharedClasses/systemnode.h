#ifndef NODE_H
#define NODE_H

#include <iostream>
#include "networkmodel.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The SystemNode class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class SystemNode
{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief net_model pointer to network model
     */
    NetworkModel* net_model;
    /**
     * @brief initials
     */
    vector<pair<std::string,std::string> > initials;
    /**
     * @brief viewer
     */
    map<pair<string,string>,string>   viewer;
    /**
     * @brief ruler
     */
    map<pair<string,string>,string>   ruler;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & net_model;
        ar & initials;
        ar & viewer;
        ar & ruler;
    }

public:
    /**
     * @brief SystemNode
     */
    SystemNode();
    /**
     * @brief SystemNode
     * @param str
     */
    SystemNode(std::string str) { name = str;}

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_net_model
     * @return
     */
    NetworkModel* get_net_model() const;
    /**
     * @brief get_initials
     * @return
     */
    vector<pair<std::string,std::string> > get_initials() const;
    /**
     * @brief get_viewer
     * @return
     */
    map<pair<string,string>,string>   get_viewer() const;
    /**
     * @brief get_ruler
     * @return
     */
    map<pair<string,string>,string>   get_ruler() const;

    /**
     * @brief set_net_model
     * @param nm
     */
    void set_net_model(NetworkModel* nm);
    /**
     * @brief set_initials
     * @param ins
     */
    void set_initials(vector<pair<std::string,std::string> > ins);
    /**
     * @brief set_viewer
     * @param vwr
     */
    void set_viewer(map<pair<string,string>,string> vwr);
    /**
     * @brief set_ruler
     * @param rlr
     */
    void set_ruler(map<pair<string,string>,string> rlr);
};

#endif // NODE_H

#ifndef NETWORKMODEL_H
#define NETWORKMODEL_H

#include <map>
#include <sstream>
#include "utils.h"
#include "netcomponent.h"
#include "nettransition.h"
#include "pattern.h"
#include "statedata_strlist.h"

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

/**
 * @brief The NetworkModel class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
class NetworkModel
{
    /**
     * @brief name
     */
    std::string name;
    /**
     * @brief components
     */
    vector<NetComponent> components;
    /**
     * @brief inputs
     */
    vector<std::string> inputs;
    /**
     * @brief outputs
     */
    vector<std::string> outputs;
    /**
     * @brief links between internal componets and from network input terminals and components input terminal (to manage pattern events)
     */
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > links;

    /**
     * @brief patterns vector of pattern declarations
     */
    vector<Pattern> patterns;
    /**
     * @brief initials initial states of components involved
     */
    vector<pair<std::string,std::string> > initials;
    /**
     * @brief viewer maps observable transitions in observable labels
     */
    map<pair<string,string>,string>   viewer;
    /**
     * @brief ruler maps fault transitions in fault labels
     */
    map<pair<string,string>,string>   ruler;

    /**
     * @brief pattern_space vector of pattern spaces, one foreach language
     */
    vector<astl::DFA_map<NetTransition,StateData_strList> *> pattern_space;
    /**
     * @brief pattern_languages
     */
    vector<set<string> > pattern_languages;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & name;
        ar & components;
        ar & inputs;
        ar & outputs;
        ar & links;
        ar & patterns;
        ar & initials;
        ar & viewer;
        ar & ruler;
        ar & pattern_space;
        ar & pattern_languages;
        ar & count;
        ar & conv_int_str;
        ar & conv_str_int;
    }

public:
    int count;
    /**
     * @brief NetworkModel default constructor
     */
    NetworkModel();
    /**
     * @brief NetworkModel constructor from id
     * @param str name
     */
    NetworkModel(std::string str);

    /**
     * @brief conv_str_int maps transition into number
     */
    std::map<std::pair<std::string,std::string>,int> conv_str_int;
    /**
     * @brief conv_int_str maps number into transition
     */
    std::map<int,std::pair<std::string,std::string> > conv_int_str;

    /**
     * @brief get_name
     * @return
     */
    std::string get_name() const;
    /**
     * @brief get_components
     * @return
     */
    vector<NetComponent> get_components() const;
    /**
     * @brief get_inputs
     * @return
     */
    vector<std::string> get_inputs() const;
    /**
     * @brief get_outputs
     * @return
     */
    vector<std::string> get_outputs() const;
    /**
     * @brief get_links
     * @return
     */
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  get_links() const;
    /**
     * @brief get_patterns
     * @return
     */
    vector<Pattern> get_patterns() const;
    /**
     * @brief get_initials
     * @return
     */
    vector<pair<std::string,std::string> > get_initials() const;
    /**
     * @brief get_viewer
     * @return
     */
    map<pair<string,string>,string>  get_viewer() const;
    /**
     * @brief get_ruler
     * @return
     */
    map<pair<string,string>,string>  get_ruler() const;
    /**
     * @brief get_pattern_space
     * @return
     */
    vector<astl::DFA_map<NetTransition,StateData_strList> *> get_pattern_space() const;
    /**
     * @brief get_pattern_languages
     * @return
     */
    vector<set<string> > get_pattern_languages() const;

    /**
     * @brief add_comp
     * @param nc
     */
    void add_comp(NetComponent nc);
    /**
     * @brief add_input
     * @param in
     */
    void add_input(std::string in);
    /**
     * @brief add_output
     * @param out
     */
    void add_output(std::string out);
    /**
     * @brief add_link
     * @param t1
     * @param c1
     * @param t2
     * @param c2
     */
    void add_link(std::string t1,std::string c1,std::string t2, std::string c2);
    /**
     * @brief add_pattern
     * @param p
     */
    void add_pattern(Pattern p);
    /**
     * @brief add_initial
     * @param state
     * @param c
     */
    void add_initial(std::string state, std::string c);
    /**
     * @brief add_label
     * @param t
     * @param l
     */
    void add_label(pair<string,string> t,std::string l);
    /**
     * @brief add_fault
     * @param t
     * @param f
     */
    void add_fault(pair<string,string> t, std::string f);
    /**
     * @brief add_pattern_space
     * @param pts
     */
    void add_pattern_space(astl::DFA_map<NetTransition,StateData_strList> * pts);
    /**
     * @brief add_language
     * @param l
     */
    void add_language(set<std::string> l);

    /**
     * @brief set_components
     * @param vc
     */
    void set_components(vector<NetComponent> vc);
    /**
     * @brief set_inputs
     * @param vi
     */
    void set_inputs(vector<std::string> vi);
    /**
     * @brief set_outputs
     * @param vo
     */
    void set_outputs(vector<std::string> vo);
    /**
     * @brief set_links
     * @param vl
     */
    void set_links(vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > vl);
    /**
     * @brief set_patterns
     * @param vp
     */
    void set_patterns(vector<Pattern> vp);
    /**
     * @brief set_initials
     * @param vi
     */
    void set_initials(vector<pair<std::string,std::string> > vi);
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

    /**
     * @brief not_trans computes not operation on regular expressions
     * @param operand
     * @return
     */
    std::string not_trans(std::string operand);
    /**
     * @brief find_component from id
     * @param id
     * @return pointer to NetComponent
     */
    NetComponent* find_component(std::string id);
};

#endif // NETWORKMODEL_H

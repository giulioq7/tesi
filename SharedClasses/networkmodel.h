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

class NetworkModel
{
    std::string name;
    vector<NetComponent> components;
    vector<std::string> inputs;
    vector<std::string> outputs;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > links;

    vector<Pattern> patterns;
    vector<pair<std::string,std::string> > initials;
    map<pair<string,string>,string>   viewer;
    map<pair<string,string>,string>   ruler;

    vector<astl::DFA_map<NetTransition,StateData_strList> *> pattern_space;
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
    NetworkModel();
    NetworkModel(std::string str);

    std::map<std::pair<std::string,std::string>,int> conv_str_int;
    std::map<int,std::pair<std::string,std::string> > conv_int_str;

    std::string get_name() const;
    vector<NetComponent> get_components() const;
    vector<std::string> get_inputs() const;
    vector<std::string> get_outputs() const;
    vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  get_links() const;
    vector<Pattern> get_patterns() const;
    vector<pair<std::string,std::string> > get_initials() const;
    map<pair<string,string>,string>  get_viewer() const;
    map<pair<string,string>,string>  get_ruler() const;
    vector<astl::DFA_map<NetTransition,StateData_strList> *> get_pattern_space() const;
    vector<set<string> > get_pattern_languages() const;

    void add_comp(NetComponent nc);
    void add_input(std::string in);
    void add_output(std::string out);
    void add_link(std::string t1,std::string c1,std::string t2, std::string c2);
    void add_pattern(Pattern p);
    void add_initial(std::string state, std::string c);
    void add_label(pair<string,string> t,std::string l);
    void add_fault(pair<string,string> t, std::string f);
    void add_pattern_space(astl::DFA_map<NetTransition,StateData_strList> * pts);
    void add_language(set<std::string> l);

    void set_components(vector<NetComponent> vc);
    void set_inputs(vector<std::string> vi);
    void set_outputs(vector<std::string> vo);
    void set_links(vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > vl);
    void set_patterns(vector<Pattern> vp);
    void set_initials(vector<pair<std::string,std::string> > vi);
    void set_viewer(map<pair<string,string>,string> vwr);
    void set_ruler(map<pair<string,string>,string> rlr);

    std::string not_trans(std::string operand);
    NetComponent* find_component(std::string id);
};

#endif // NETWORKMODEL_H

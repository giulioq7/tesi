#include "networkmodel.h"

NetworkModel::NetworkModel()
{
     count = 0;
}

NetworkModel::NetworkModel(std::string str)
{
     count = 0;
     name = str;
}


std::string NetworkModel::get_name() const
{
    return name;
}

vector<NetComponent> NetworkModel::get_components() const
{
    return components;
}

vector<std::string> NetworkModel::get_inputs() const
{
    return inputs;
}

vector<std::string> NetworkModel::get_outputs() const
{
    return outputs;
}

vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > >  NetworkModel::get_links() const
{
    return links;
}

vector<Pattern> NetworkModel::get_patterns() const
{
    return patterns;
}

vector<pair<std::string,std::string> > NetworkModel::get_initials() const
{
    return initials;
}

map<pair<string,string>,string>  NetworkModel::get_viewer() const
{
    return viewer;
}

map<pair<string,string>,string>  NetworkModel::get_ruler() const
{
    return ruler;
}

vector<astl::DFA_map<NetTransition,StateData_strList> *> NetworkModel::get_pattern_space() const
{
    return pattern_space;
}

vector<set<string> > NetworkModel::get_pattern_languages() const
{
    return pattern_languages;
}

void NetworkModel::add_comp(NetComponent nc)
{
    components.push_back(nc);
}

void NetworkModel::add_input(std::string in)
{
    inputs.push_back(in);
}

void NetworkModel::add_output(std::string out)
{
    outputs.push_back(out);
}

void NetworkModel::add_link(std::string t1,std::string c1,std::string t2, std::string c2)
{
    links.push_back(make_pair(make_pair(t1,c1),make_pair(t2,c2)));
}

void NetworkModel::add_pattern(Pattern p)
{
    patterns.push_back(p);
}

void NetworkModel::add_initial(std::string state, std::string c)
{
    initials.push_back(make_pair(state,c));
}

void NetworkModel::add_label(pair<string,string> t,std::string l)
{
    viewer[t] = l;
}

void NetworkModel::add_fault(pair<string,string> t, std::string f)
{
    ruler[t] = f;
}

void NetworkModel::add_pattern_space(astl::DFA_map<NetTransition,StateData_strList> * pts)
{
    pattern_space.push_back(pts);
}

void NetworkModel::add_language(set<string> l)
{
    pattern_languages.push_back(l);
}


void NetworkModel::set_components(vector<NetComponent> vc)
{
    components = vc;
}

void NetworkModel::set_inputs(vector<std::string> vi)
{
    inputs = vi;
}

void NetworkModel::set_outputs(vector<std::string> vo)
{
    outputs = vo;
}

void NetworkModel::set_links(vector<pair<pair<std::string,std::string>,pair<std::string,std::string> > > vl)
{
    links = vl;
}

void NetworkModel::set_patterns(vector<Pattern> vp)
{
    patterns = vp;
}

void NetworkModel::set_initials(vector<pair<std::string,std::string> > vi)
{
    initials = vi;
}

void NetworkModel::set_viewer(map<pair<string,string>,string> vwr)
{
    viewer = vwr;
}

void NetworkModel::set_ruler(map<pair<string,string>,string> rlr)
{
    ruler = rlr;
}



std::string NetworkModel::not_trans(std::string operand)
{
    string res_expr = "(";
    for(unsigned int i=0; i<components.size();i++)
    {
        vector<Transition> trans = components.at(i).get_model()->get_trans();
        for(unsigned int j=0; j<trans.size() ; j++)
        {
            string t = trans.at(j).get_name();
            string c = components.at(i).get_name();
            string str(t); str.append("("); str.append(c); str.append(")");
            if(str != operand)
            {
                pair<string,string> p = make_pair(t,c);
                if(conv_str_int.find(p) == conv_str_int.end())
                {
                    conv_str_int[p] = count;
                    conv_int_str[count] = p;
                    count++;
                }
                stringstream ss;
                ss << conv_str_int[p];
                res_expr.append(ss.str());
                res_expr.append("|");
            }

        }
    }

    //removes last pipe "|"
    res_expr.resize(res_expr.size()-1);
    res_expr.append(")");

    return res_expr;
}


NetComponent *NetworkModel::find_component(std::string id)
{
    NetComponent* ref = NULL;
    for(vector<NetComponent>::iterator it = components.begin(); it != components.end(); it++)
    {
        if((*it).get_name() == id)
        {
            ref = &(*it);
            break;
        }
    }
    return ref;
}

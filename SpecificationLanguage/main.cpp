/**
 * @mainpage
 *
 * @brief Offline compiler (specification and preprocessing)
 *
 * @author Giulio Quarenghi
 *
 * @date  2016 February
 *
 */

#include <iostream>
#include <utility>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include "spec_driver.h"
#include "ostream_util.h"
#include "utils.h"
#include <stream.h>
#include "astl.h"
#include "determinize.h"
#include "dot.h"
#include "grail_lib.h"
#include "nettransition.h"
#include "minimize.h"

#define TYPE int
#define GRAPHS_DIR "../Graphs/SpecificationLanguage/"



using namespace std;
using namespace grail;
using namespace astl;
using namespace boost;

//PUBLIC
fm<int> start_build(vector<Pattern> patterns);
DFA_map<NetTransition, StateData_strList>* from_grail_to_astl(fm<TYPE>* dfa, NetworkModel* net);

//PRIVATE
fm<int> patodfa(std::string regex);
void mark_finals(fm<int> &A, std::string patt);
grail::fm<TYPE>* merge_dfa(std::vector<grail::fm<TYPE>* > *vec);
void add_eps_to_init(fm<int> &nfa);
fm<int> mysubset(grail::fm<TYPE>* nfa,grail::list<grail::set<grail::state> >&	sub);
void minimize(grail::fm<TYPE>* fm);
void dot_draw(std::ostream &strm, grail::fm<TYPE>* Pts);


int main(int argc, char** argv)
{
    spec_driver driver;
    for (int i = 1; i < argc; ++i)
    {
        if (argv[i] == std::string("-p"))
            driver.trace_parsing = true;
        else if (argv[i] == std::string("-s"))
            driver.trace_scanning = true;
        else if (!driver.parse(argv[i]))
            cout << "Parsing ended." << endl;
    }

    //adjust inherited attributes of system nodes and problem nodes (initial states, viewer, ruler)
    driver.adjust_inherited();


    for(vector<ProblemNode>::iterator it = driver.problem.nodes.begin(); it != driver.problem.nodes.end(); it++)
    {
        if(it->get_initials().size() < it->get_ref_node()->get_net_model()->get_components().size())
        {
            driver.error("Initial state missed");
            exit(1);
        }
    }

    std::string str(GRAPHS_DIR);
    chdir(str.c_str());

    //dependency graph of system nodes
    driver.build_dependency_graph();

    if(debug > 0)
    {
        ofstream file;
        file.open("dependency_graph");
        Utils::my_dot(file, *driver.system.get_dependency_graph());
        //full_dot(file, dfirst_markc(driver.system.dependency_graph));
    }

    //build components models automata (initial state missed because it will be certainly defined only in problem node)
    driver.build_automata_comp();

    for(vector<ProblemNode>::iterator it = driver.problem.nodes.begin(); it != driver.problem.nodes.end(); it++)
    {
        vector<NetComponent> comps = it->get_ref_node()->get_net_model()->get_components();
        for(vector<NetComponent>::iterator it2 = comps.begin();
            it2 != comps.end(); it2++)
        {
            Component c(it2->get_name());
            c.set_model(it2->get_model());

            it->add_concrete_component(c);
        }
    }

    for(vector<NetworkModel>::iterator it = driver.networks.begin(); it != driver.networks.end(); it++)
    {
        vector<Pattern> patts = it->get_patterns();
        for(vector<Pattern>::iterator it2 = patts.begin(); it2 != patts.end(); it2++)
        {
            if(!it2->is_max_language())
                it2->not_substitution();
        }
    }
    //print all info
    //cout << driver;

    //xdot file should be blank because initial state is missed
    /*ofstream file("comp_model_bhv.xdot");
    full_dot(file, dfirst_markc(*driver.components[0].automaton)); // write tags
    file.close();*/
//    ofstream file("comp_model_bhv.xdot");
//    Utils::my_dot(file,*driver.components[2].automaton);
//    file.close();

//    ofstream file2("comp_model_bhv2.xdot");
//    Utils::my_dot(file2,*driver.components[3].automaton);
//    file2.close();

    for(vector<NetworkModel>::iterator it = driver.networks.begin(); it != driver.networks.end(); it++)
    {
        int num_pts = 0;
        vector<Pattern> remaining = it->get_patterns();
        while(!remaining.empty())
        {
            vector<Pattern> same_language;
            same_language.push_back(remaining.at(0));
            remaining.erase(remaining.begin());
            vector<Pattern>::iterator it2;
            for(it2 = remaining.begin(); it2 != remaining.end();it2++)
            {
                if(same_language.at(0).get_language() == it2->get_language())
                {
                    same_language.push_back(*it2);
                    remaining.erase(it2);
                    it2--;
                }
            }
            fm<TYPE> gr = start_build(same_language);
            DFA_map<NetTransition,StateData_strList>* ptspace = from_grail_to_astl(&gr,&(*it));
            //Utils::minimize_by_partition(*ptspace);
            it->add_pattern_space(ptspace);

            vector<int> lang = same_language.at(0).get_language();
            std::set<std::string> res_lang;
            for(vector<int>::iterator it_l = lang.begin(); it_l != lang.end(); it_l++)
            {
                std::pair<std::string,std::string> conv = it->conv_int_str[*it_l];
                std::string s = conv.first;
                s.append("("); s.append(conv.second); s.append(")");
                res_lang.insert(s);
            }
            it->add_language(res_lang);

            if(debug > 0)
            {
               ofstream file;
               stringstream ss;
               ss << "pts_" << it->get_name() << num_pts;
               file.open (ss.str().c_str());
               Utils::my_dot(file,*ptspace);
               //full_dot(file, dfirst_markc(*ptspace));
               file.close();
               //full_dump(cout, dfirst_markc(*ptspace));
            }
            num_pts++;
        }
    }

    driver.build_Isp();
    /*ofstream fi("Isp");
    full_dot(fi,dfirst_markc(*driver.problem.nodes[0].index_space));
    fi.close();*/

    for(vector<ProblemNode>::iterator it = driver.problem.nodes.begin(); it != driver.problem.nodes.end(); it++)
    {
        vector<std::string> inputs = it->get_ref_node()->get_net_model()->get_inputs();
        for(vector<std::string>::iterator it2 = inputs.begin(); it2 != inputs.end(); it2++)
        {
            Terminal *t = new Terminal(*it2);
            it->add_input_terminal(t);
        }
        vector<std::string> outputs = it->get_ref_node()->get_net_model()->get_outputs();
        for(vector<std::string>::iterator it2 = outputs.begin(); it2 != outputs.end(); it2++)
        {
            Terminal *t = new Terminal(*it2);
            it->add_output_terminal(t);
        }
    }
    for(vector<ProblemNode>::iterator it = driver.problem.nodes.begin(); it != driver.problem.nodes.end(); it++)
           it->make_terminals();

    vector<std::pair<std::pair<std::string,std::string>,std::pair<std::string,std::string> > > emer = driver.system.get_emergence();
    for(vector<std::pair<std::pair<std::string,std::string>,std::pair<std::string,std::string> > >::iterator it = emer.begin(); it != emer.end(); it++)
    {
        Terminal *t1, *t2;
        ProblemNode *n1, *n2;
        n1 = Utils::find_from_id(driver.problem.nodes, it->first.second);
        vector<Terminal*> outs = n1->get_output_terminals();
        t1 = Utils::findptr_from_id(outs, it->first.first);
        n2 = Utils::find_from_id(driver.problem.nodes, it->second.second);
        vector<Terminal*> ins = n2->get_input_terminals();
        t2 = Utils::findptr_from_id(ins, it->second.first);
        t1->add_linked_terminal(t2);
    }

    vector<Terminal*> input_terminals;
    multimap<Terminal*,int> lazy;
    for(vector<ProblemNode>::iterator it = driver.problem.nodes.begin(); it != driver.problem.nodes.end(); it++)
    {
        int i=0;
        for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
        {
            vector<Terminal*> terms = it2->get_input_terminals();
            for(vector<Terminal*>::iterator it3 = terms.begin(); it3 != terms.end(); it3++)
            {
                input_terminals.push_back(*it3);
                lazy.insert(make_pair(*it3,i));
                i++;
            }
        }
    }
    map<Terminal*,int> term_map;
    int m = 0;
    for(vector<Terminal*>::iterator it = input_terminals.begin(); it != input_terminals.end(); it++)
    {
        term_map[*it] = m;
        m++;
    }

    for(vector<ProblemNode>::iterator it = driver.problem.nodes.begin(); it != driver.problem.nodes.end(); it++)
    {
         vector<Terminal*> lazy_input_terminals;
         for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
         {
             vector<Terminal*> terms = it2->get_input_terminals();
             for(vector<Terminal*>::iterator it3 = terms.begin(); it3 != terms.end(); it3++)
                 lazy_input_terminals.push_back(*it3);
         }
         map<Terminal*,int> lazy_term_map;
         int m = 0;
         for(vector<Terminal*>::iterator i = lazy_input_terminals.begin(); i != lazy_input_terminals.end(); i++)
         {
             lazy_term_map[*i] = m;
             m++;
         }
        for(vector<Component>::iterator it2 = it->concrete_components.begin(); it2 != it->concrete_components.end(); it2++)
        {
            it2->set_automaton(new DFA_map<SysTransition,StateData_str>());
            DFA_map<Transition,StateData_str> *current = it2->get_model()->get_automaton();
            DFA_map<Transition,StateData_str>::const_iterator c_it;
            for(c_it = current->begin(); c_it != current->end(); c_it++)
            {
                DFA_map<SysTransition,StateData_str>::state_type s;
                s = it2->get_automaton()->new_state();
                current->tag(s) = current->tag(*c_it);
            }
            it2->get_automaton()->initial(it->find_initial_state(it2->get_name()));
            current->initial(*current->begin());
            typename bfirst_mark_cursor_type<DFA_map<Transition,StateData_str>, DFA_concept, set_marker
                    <unsigned int, std::less<unsigned int> > >::model bfc = bfirst_markc(*it2->get_model()->get_automaton()), bfc_end;

            while(bfc != bfc_end)
            {
                do
                {
                    NetComponent *nc = it->get_ref_node()->get_net_model()->find_component(it2->get_name());
                    Transition *simple = nc->get_model()->find_trans(bfc.letter().get_name());
                    SysTransition t(simple,nc,it->get_ref_node());
                    t.set_input_event(t.get_trans()->get_input_event().first, term_map[it2->find_input_terminal(t.get_trans()->get_input_event().second)]);
                    t.set_lazy_input_event(t.get_trans()->get_input_event().first, lazy_term_map[it2->find_input_terminal(t.get_trans()->get_input_event().second)]);
                    std::vector<std::pair<std::string,std::string> > out_evs = t.get_trans()->get_out_events();
                    for(vector<std::pair<std::string,std::string> >::iterator i = out_evs.begin(); i!= out_evs.end(); i++)
                    {
                        Terminal *out_term = it2->find_output_terminal(i->second);
                        vector<int> list,lazy_list;
                        vector<Terminal*> terms = out_term->get_linked_terminals();
                        for(vector<Terminal*>::iterator j = terms.begin(); j != terms.end(); j++)
                        {
                            list.push_back(term_map[*j]);
                            lazy_list.push_back(lazy_term_map[*j]);
                        }
                        t.add_output_event(i->first,list);
                        t.add_lazy_output_event(i->first,lazy_list);
                    }
                    it2->get_automaton()->set_trans(bfc.src(),t,bfc.aim());
                }while(bfc.next());
            }
            current->initial(current->null_state);
        }
        vector<Pattern> patterns = it->get_ref_node()->get_net_model()->get_patterns();
        for(vector<Pattern>::iterator it2 = patterns.begin(); it2 != patterns.end(); it2++)
        {
            vector<Terminal*> outs = it->get_output_terminals();
            Terminal *out = Utils::findptr_from_id(outs,it2->get_terminal_id());
            it->add_patt_map(it2->get_name(), out);
            vector<int> list, lazy_list;
            //searches all network input terminals linked to out terminal
            vector<Terminal*> terms = out->get_linked_terminals();
            for(vector<Terminal*>::iterator it3 = terms.begin(); it3 != terms.end(); it3++)
            {
                //searches all component input terminals linked to network input terminal
                vector<Terminal*> terms2 = (*it3)->get_linked_terminals();
                for(vector<Terminal*>::iterator it4 = terms2.begin(); it4 != terms2.end(); it4++)
                {
                    list.push_back(term_map[*it4]);
                    //lazy_list.push_back(lazy_term_map[*it4]);
                    lazy_list.push_back(lazy.find(*it4)->second);
                }
            }
            it->add_patt_indexes_map(it2->get_name(),list);
            it->add_lazy_patt_indexes_map(it2->get_name(), lazy_list);
        }
    }




    // save data to archive
    {
        std::ofstream ofs("../../CompiledData/component_models.dat");
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << driver.components;
        // archive and stream closed when destructors are called
    }

    // save data to archive
    {
        std::ofstream ofs("../../CompiledData/network_models.dat");
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << driver.networks;
        // archive and stream closed when destructors are called
    }

    // save data to archive
    {
        std::ofstream ofs("../../CompiledData/system.dat");
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << driver.system;
        // archive and stream closed when destructors are called
    }

    // save data to archive
    {
        std::ofstream ofs("../../CompiledData/problem.dat");
        boost::archive::text_oarchive oa(ofs);
        // write class instance to archive
        oa << driver.problem;
        // archive and stream closed when destructors are called
    }


    // ... some time later restore the class instance to its orginal state
   /*vector<ComponentModel> comps;
   vector<NetworkModel> nets;
   System system;
   Problem problem;
   {
        // create and open an archive for input
        std::ifstream ifs("../../CompiledData/component_models.dat");
        boost::archive::text_iarchive ia(ifs);
        // read class state from archive
        ia >> comps;
        // archive and stream closed when destructors are called
   }
   {
       // create and open an archive for input
       std::ifstream ifs("../../CompiledData/network_models.dat");
       boost::archive::text_iarchive ia(ifs);
       // read class state from archive
       ia >> nets;
       // archive and stream closed when destructors are called
   }
   {
       // create and open an archive for input
       std::ifstream ifs("../../CompiledData/system.dat");
       boost::archive::text_iarchive ia(ifs);
       // read class state from archive
       ia >> system;
       // archive and stream closed when destructors are called
   }
   {
       // create and open an archive for input
       std::ifstream ifs("../../CompiledData/problem.dat");
       boost::archive::text_iarchive ia(ifs);
       // read class state from archive
       ia >> problem;
       // archive and stream closed when destructors are called
   }*/

    /*ofstream f2("comp_concrete_bhv.xdot");
    full_dot(f2,dfirst_markc(*problem.nodes[0].concrete_components[2].automaton));
    f2.close();*/

}


fm<TYPE> start_build(vector<Pattern > patterns)
{
    vector<fm<TYPE>* >* v = new vector<fm<TYPE>* >;

    vector<Pattern>::iterator it;
    for(it = patterns.begin(); it != patterns.end(); it++)
    {
        fm<TYPE> *A = new fm<TYPE>;
        *A = patodfa(it->get_expr());
        mark_finals(*A, it->get_name());
        v->push_back(A);

        if(debug > 0)
        {
           ofstream f;
           stringstream ss;
           ss << "A_";
           ss << it->get_name();
           f.open(ss.str().c_str());
           dot_draw(f, A);
           f.close();
        }
    }

   fm<TYPE> *merge = merge_dfa(v);

   if(debug > 1)
   {
       ofstream f;
       f.open("merge_before_eps");
       dot_draw(f, merge);
       f.close();
   }

   add_eps_to_init(*merge);

   if(debug > 1)
   {
       ofstream f;
       f.open("merge_after_eps");
       dot_draw(f, merge);
       f.close();
   }

   //merge->subset(); //this function has been modified (just a bit) in order to mantain subsets information
                      //to mark final states
   grail::list<grail::set<grail::state> > sub;
   fm<TYPE> merge2;
   merge2 = mysubset(merge,sub);
   //merge2.min_by_partition(); //with this we'll lose final tags information

   //retrieves final states labels
   grail::set<grail::state> fin;
   merge2.finals(fin);
   grail::set<grail::state> fin_merge;
   merge->finals(fin_merge);
   for(int a=0; a<fin.size(); a++)
   {
       std::string str = "{";
       for(int b=0; b<sub[fin[a].value()].size(); b++)
       {
           int index = fin_merge.member(sub[fin[a].value()][b]);
           if(index != -1)
           {
               str.append(merge->final_states[index].get_tag());
               str.append(",");
           }
       }
       str.resize(str.size()-1); //removes last comma (useless)
       str.append("}");
       merge2.final_states[a].set_tag(str);
   }

   //this minimization seems to have some problems too (probably because it does not take into account transitions of the merged final states)
   //minimize(&merge2);

   /*
   if(debug > 0)
   {
       //prints some nodes and arcs
       grail::set<grail::state>  nodes;
       merge2->states(nodes);
       cout << "All: "<< nodes;
       grail::set<grail::state>  in;
       merge2->starts(in);
       cout << "Initials: "<< in;
       grail::set<grail::state> finals;
       merge2->finals(finals);
       cout << "Finals: "<< finals;
       grail::set<grail::inst<TYPE> > arcs;
       merge2->get_arcs(arcs);
       cout << "Arcs: " << arcs;

       //prints final states tags
       cout << "Final states Tags:" <<endl;
       for(int i=0; i<merge2->final_states.size();i++)
           cout << merge2->final_states[i].get_tag() << endl;
   }*/

   //draw merge2
   if(debug > 1)
   {
       std::string name = "pattern_space";
       ofstream f;
       f.open(name.c_str());
       dot_draw(f, &merge2);
       f.close();
   }

   vector<fm<TYPE>* >::iterator it_v;
   for(it_v = v->begin(); it_v != v->end(); it_v++)
       delete *it_v;
   //v->erase(v->begin(),v->end());
   delete v;
   //delete merge;

   return merge2;
}


fm<TYPE> patodfa(std::string regex)
{
    re<TYPE> r;
    istringstream str(regex);
    str >> r;
    fm<TYPE> *dfa = new fm<TYPE>;
    r.retofm(*dfa);
    grail::list<grail::set<grail::state> > sub;
    mysubset(dfa,sub);
    //dfa->subset();

    return *dfa;
}


void mark_finals(fm<TYPE>& A, std::string patt)
{
    grail::set<grail::state> finals;
    A.finals(finals);
    for(int i=0; i<finals.size();++i)
    {
        finals[i].set_tag(patt);
    }
    A.set_finals(finals);
}


fm<TYPE>* merge_dfa(vector<fm<TYPE> *> *vec)
{
    fm<TYPE>* nfa = new fm<TYPE>;

    vector<fm<TYPE> *>::iterator it;

    for(it = vec->begin(); it != vec->end(); it++)
        *nfa += **it;

     return nfa;
}


void add_eps_to_init(fm<TYPE>& nfa)
{
    grail::set<grail::state>  nodes;
    nfa.states(nodes);
    grail::set<grail::state> initials;
    nfa.starts(initials);
    //grail::set<grail::state> finals;
    //nfa.finals(finals);
    //finals = nfa.final_states;

    grail::set<grail::state> non_initials(nodes);
    non_initials -= initials;

    grail::set<grail::inst<TYPE> > arcs;
    nfa.get_arcs(arcs);

    //cout << nodes;
    //cout << initials;
    //cout << non_initials;
    //cout << finals;
    //cout << arcs;

    for(int i=0; i<arcs.size();++i)
    {
        grail::state current = arcs[i].get_sink();
        if(non_initials.member(current)!=-1)
        {
            for(int j=0;j<nfa.number_of_start_states(); ++j)
            {
                TYPE label = arcs[i].get_label();
                inst<TYPE> trans;
                trans.assign(arcs[i].get_source(),label,initials[j]);
                nfa.add_instruction(trans);
                if(nfa.final_states.member(initials[j]) != -1)
                {
                    grail::set<grail::state> fin;
                    nfa.finals(fin);
                    fin += current;
                    nfa.set_finals(fin);
                }
            }
        }
    }
}


fm<TYPE> mysubset(fm<TYPE>* nfa,grail::list<grail::set<grail::state> >&	sub)
{
    fm<TYPE>		m;
    int			i;
    int			j;
    int			k;
    int			no_states;
    grail::state			s1;
    grail::state			s2;
    grail::set<grail::state>		target;
    grail::set<grail::state>		temp2;
    grail::set<TYPE>		alphabet;
    inst<TYPE>		t;
    fm<TYPE>		temp;

    // if already deterministic, do nothing
    if (nfa->is_deterministic())
        return *nfa;

    // sort if necessary
    nfa->arcs.sort();

    // remove unreachable states
    //nfa->reachable_fm();
    //nfa->reverse();
    //nfa->reachable_fm();
    //nfa->reverse();

    // collect the alphabet
    nfa->labels(alphabet);

    // add start subset to the list


    sub += nfa->start_states;

    s1 = 0;
    m.start_states += s1;
    if (debug>8)
    {std::cout<<"in fm:subset(), added start states: 0"<<std::endl;}


        // if some start state is also a final state, add
        // the new start state to the list of final states
        temp2.intersect(nfa->final_states, nfa->start_states);
        if (temp2.size() > 0)
            m.final_states += s1;
    if (debug>8)
    {std::cout<<"in fm:subset(), intesection of Start states and final states has "<<temp2.size()<<" elements"<<std::endl;}

        // for each existing set in the subset list,
        // find a new reachable subset
        no_states = 1;

        for (i=0; i<no_states; ++i)
        {
            for (j=0; j<alphabet.size(); ++j)
            {
                target.clear();
                for (k=0; k<sub[i].size(); ++k)
                {
                    nfa->select(alphabet[j], sub[i][k], SOURCE, temp);
                    temp.sinks(temp2);
                    target += temp2;
                }

                if (target.size() == 0)
                    continue;

                // create states for an instruction
                s1 = i;

                if ((k = sub.member(target)) < 0)
                {
                    sub += target;
                    s2 = no_states++;
                    temp2.intersect(nfa->final_states, target);
                    if (temp2.size() > 0)
                        {m.final_states += s2;
    if (debug>8)
    {std::cout<<"in fm:subset(), added final states: 0"<<s2.value()<<std::endl;}}
                }
                else
                    s2 = k;

                t.assign(s1, alphabet[j], s2);
                m.arcs += t;
    if (debug>8)
    {std::cout<<"in fm:subset(), added arcs states: 0"<<t.get_source().value()<<t.get_label()<<t.get_sink().value()<<std::endl;}
            }
        }
    if (debug>0)
    {std::cout<<"in fm:subset(), sets of states:" <<std::endl;
    for (int a=0;a<sub.size();a++)
        {std::cout<<"set: "<<a<<" { ";
        for(int b=0;b<sub[a].size();b++)
            std::cout<<sub[a][b].value()<<" ";
        std::cout<<" }"<<std::endl;
        }
    }

    return m;

}


void minimize(fm<TYPE>* fm)
{
    std::string tags[fm->number_of_final_states()];
    for(int i=0; i<fm->number_of_final_states(); i++)
    {
        for(int j=i+1; j<fm->number_of_final_states(); j++)
        {
            if(fm->final_states[i].get_tag() == fm->final_states[j].get_tag())
            {
                for(int k=0; k<fm->arcs.size();k++)
                {
                    if(fm->arcs[k].get_sink().value() == fm->final_states[j].value())
                    {
                        inst<TYPE> tr;
                        tr.assign(fm->arcs[k].get_source(),fm->arcs[k].get_label(),fm->final_states[i]);
                        //cout << tr;
                        fm->add_instruction(tr);
                        //cout << fm->arcs;
                        fm->arcs.sort();
                    }
                }
                fm->remove(fm->final_states[j]);
                //cout << fm->arcs;
                j--;
             }
        }
        tags[i] = fm->final_states[i].get_tag();
    }
    fm->canonical_numbering(); //looses tags
    for(int i=0; i<fm->number_of_final_states(); i++)
        fm->final_states[i].set_tag(tags[i]);
}



void dot_draw(ostream& strm, fm<TYPE>* Pts)
{
    strm << "digraph G \n { ratio=0.5;\n rankdir=TB;\n ranksep=0.4;" << endl;
    grail::set<grail::state> nodes;
    Pts->states(nodes);
    grail::set<grail::state> fin;
    Pts->finals(fin);
    grail::set<grail::state> non_fin;
    Pts->states(non_fin);
    non_fin -= fin;
    for(int i=0; i<non_fin.size(); i++)
        strm << non_fin[i].value() << " [shape=circle,fontsize=40,label=\"" << non_fin[i].value() << "\"]" << endl;
    for(int i=0; i<fin.size(); i++)
        strm << fin[i].value() << " [shape=doublecircle,fontsize=40,label=\"" << fin[i].value() << "\n" << Pts->final_states[i].get_tag() << "\"]" << endl;
    grail::set<inst<TYPE> > arcs;
    Pts->get_arcs(arcs);
    for(int i=0; i< arcs.size(); i++)
        strm << arcs[i].get_source().value() << " -> " << arcs[i].get_sink().value() << " [label=\"" << arcs[i].get_label() << "\",fontsize=40];" << endl;

    for(int i=0; i<Pts->start_states.size(); i++)
    {
        strm << "node [shape = point, color=white, fontcolor=white]; start" << i << ";" << endl;
        strm << "start" << i << " -> " << Pts->start_states[i].value() << ";" << endl;
    }
    strm << "}";
}


DFA_map<NetTransition, StateData_strList> *from_grail_to_astl(fm<TYPE> *dfa, NetworkModel* net)
{
    grail::set<grail::state> init;
    dfa->starts(init);

    map<int,std::string> tag_map;
    for(int i=0; i<dfa->final_states.size(); i++)
    {
       tag_map[dfa->final_states[i].value()] = dfa->final_states[i].get_tag();
    }
    grail::set<grail::state> fin;
    dfa->finals(fin);

    grail::set<inst<TYPE> > trans;
    dfa->get_arcs(trans);

    DFA_map<NetTransition,StateData_strList> *target = new DFA_map<NetTransition,StateData_strList>;

    int n = dfa->number_of_states();
    DFA_map<NetTransition,StateData_strList>::state_type s[n];
    target->new_state(n, s);

    for(int i=0; i<trans.size();i++)
    {
        int index_src = trans[i].get_source().value();
        int index_aim = trans[i].get_sink().value();
        std::pair<std::string,std::string> p = net->conv_int_str[trans[i].get_label()];
        //cout << p.first << "(" << p.second << ")" << endl;
        NetComponent *comp = net->find_component(p.second);
        Transition *tr = comp->get_model()->find_trans(p.first);
        NetTransition t(tr,comp);
        target->set_trans(s[index_src],t,s[index_aim]);

        if(init.member(trans[i].get_source()) != -1)
            target->initial(s[index_src]);
        if(init.member(trans[i].get_sink()) != -1)
            target->initial(s[index_aim]);

        if(fin.member(trans[i].get_source()) != -1)
        {
            std::string str = tag_map[index_src];
            trim_left_if(str,is_any_of("{")); trim_right_if(str,is_any_of("}"));
            typedef std::set<std::string> split_vector_type;
            split_vector_type set_str; // #2: Search for tokens
            split(set_str, str, is_any_of(","),token_compress_on);
            for(split_vector_type::iterator it = set_str.begin(); it != set_str.end(); it++)
            {
                if(*it == "")
                    set_str.erase(it);
            }
            target->tag(s[index_src]) = StateData_strList(set_str);
            target->final(s[index_src]) = true;
        }
        if(fin.member(trans[i].get_sink()) != -1)
        {
            std::string str = tag_map[index_aim];
            trim_left_if(str,is_any_of("{")); trim_right_if(str,is_any_of("}"));
            typedef std::set<std::string> split_vector_type;
            split_vector_type set_str; // #2: Search for tokens
            split(set_str, str, is_any_of(","),token_compress_on);
            for(split_vector_type::iterator it = set_str.begin(); it != set_str.end(); it++)
            {
                if(*it == "")
                    set_str.erase(it);
            }
            target->tag(s[index_aim]) = StateData_strList(set_str);
            target->final(s[index_aim]) = true;
        }
    }

    return target;
}

#include <iostream>
#include <utility>
#include <unistd.h>
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
#include "serialize.h"

#define TYPE int
#define GRAPHS_DIR "./Pts/"



using namespace std;
using namespace grail;
using namespace astl;

//PUBLIC
fm<TYPE>* start_build(std::vector<std::pair<std::string,std::string> > patterns);
DFA_map<NetTransition,StateData>* from_grail_to_astl(fm<TYPE>* dfa, NetworkModel* net);

//PRIVATE
grail::fm<TYPE>* patodfa(std::string regex);
void mark_finals(grail::fm<TYPE>* A, std::string patt);
grail::fm<TYPE>* merge_dfa(std::vector<grail::fm<TYPE>* > *vec);
void add_eps_to_init(grail::fm<TYPE>* nfa);
grail::fm<TYPE>* mysubset(grail::fm<TYPE>* nfa,grail::list<grail::set<grail::state> >&	sub);
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
        if(it->initials.size() < it->ref_node->net_model->components.size())
        {
            driver.error("Initial state missed");
            exit(1);
        }
    }

    //entails system root node
    if(driver.system.id_root == "")
        driver.system.root = &driver.system.node_list.at(0);
    else
        driver.system.root = driver.find_node(driver.system.id_root);

    //dependency graph of system nodes
    driver.build_dependency_graph();

    if(debug > 0)
    {
        ofstream file;
        file.open("dependency_graph");
        full_dot(file, dfirst_markc(driver.system.dependency_graph));
    }

    //build components models automata (initial state missed because it will be certainly defined only in problem node)
    driver.build_automata_comp();

    //print all info
    cout << driver;

    //ofstream file("prova.xdot");
    //full_dot(file, dfirst_markc(driver.components[1].automaton)); // write tags
    //file.close();

    vector<NetworkModel>::iterator it;
    for(it = driver.networks.begin(); it != driver.networks.end(); it++)
    {
       DFA_map<NetTransition,StateData>* ptspace = from_grail_to_astl(start_build((*it).patterns),&(*it));
       (*it).pattern_space = *ptspace;

       if(debug > 0)
       {
           ofstream file;
           std::string str = "pts_";
           str.append((*it).name);
           file.open (str.c_str());
           full_dot(file, dfirst_markc((*it).pattern_space));
           file.close();
           //full_dump(cout, dfirst_markc(*ptspace));
       }
    }

    // create and open a character archive for output
        std::ofstream ofs("filename");

    // save data to archive
        {
            //boost::archive::text_oarchive oa(ofs);
            // write class instance to archive
            //oa << driver.components.at(0);
            // archive and stream closed when destructors are called
        }


}


fm<TYPE>* start_build(vector<std::pair<std::string,std::string> > patterns)
{
    chdir(GRAPHS_DIR);

    vector<fm<TYPE>* >* v = new vector<fm<TYPE>* >;

    vector<std::pair<std::string,std::string> >::iterator it;
    for(it = patterns.begin(); it != patterns.end(); it++)
    {
        fm<TYPE>* A = patodfa((*it).second);
        mark_finals(A,(*it).first);
        v->push_back(A);

        if(debug > 0)
        {
           ofstream f;
           stringstream ss;
           ss << "A_";
           ss << (*it).first;
           f.open(ss.str().c_str());
           dot_draw(f, A);
           f.close();
        }
    }

   fm<TYPE> *merge = merge_dfa(v);

   if(debug > 0)
   {
       ofstream f;
       f.open("merge_before_eps");
       dot_draw(f, merge);
       f.close();
   }

   add_eps_to_init(merge);

   if(debug > 0)
   {
       ofstream f;
       f.open("merge_after_eps");
       dot_draw(f, merge);
       f.close();
   }

   //merge->subset(); //this function has been modified (just a bit) in order to mantain subsets information
                      //to mark final states
   grail::list<grail::set<grail::state> > sub;
   fm<TYPE> *merge2;
   merge2 = mysubset(merge,sub);
   //merge2->min_by_partition(); //with this we'll lose final tags information

   //retrieves final states labels
   grail::set<grail::state> fin;
   merge2->finals(fin);
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
       merge2->final_states[a].set_tag(str);
   }

   //this minimization seems to have some problems too (probably because it does not take into account transitions of the merged final states)
   //minimize(merge2);

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
   if(debug > 0)
   {
       std::string name = "pattern_space";
       ofstream f;
       f.open(name.c_str());
       dot_draw(f, merge2);
       f.close();
   }

   v->erase(v->begin(),v->end());
   delete v;
   delete merge;

   return merge2;
}


fm<TYPE>* patodfa(std::string regex)
{
    re<TYPE> r;
    istringstream str(regex);
    str >> r;
    fm<TYPE>* dfa = new fm<TYPE>;
    r.retofm(*dfa);
    dfa->subset();

    return dfa;
}


void mark_finals(fm<TYPE>* A, std::string patt)
{
    grail::set<grail::state> finals;
    A->finals(finals);
    for(int i=0; i<finals.size();++i)
    {
        finals[i].set_tag(patt);
    }
    A->set_finals(finals);
}


fm<TYPE>* merge_dfa(vector<fm<TYPE> *> *vec)
{
    fm<TYPE>* nfa = new fm<TYPE>;

    for(unsigned int i=0; i<vec->size(); ++i)
        *nfa += *(vec->at(i));

     return nfa;
}


void add_eps_to_init(fm<TYPE>* nfa)
{
    grail::set<grail::state>  nodes;
    nfa->states(nodes);
    grail::set<grail::state> initials;
    nfa->starts(initials);
    grail::set<grail::state> finals;
    nfa->finals(finals);

    grail::set<grail::state> non_initials(nodes);
    non_initials -= initials;

    grail::set<grail::inst<TYPE> > arcs;
    nfa->get_arcs(arcs);

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
            for(int j=0;j<nfa->number_of_start_states(); ++j)
            {
                TYPE label = arcs[i].get_label();
                inst<TYPE> trans;
                trans.assign(arcs[i].get_source(),label,initials[j]);
                nfa->add_instruction(trans);
                if(finals.member(initials[j]) != -1)
                {
                    grail::set<grail::state> fin;
                    nfa->finals(fin);
                    fin += current;
                    nfa->set_finals(fin);
                }
            }
        }
    }
}


fm<TYPE>* mysubset(fm<TYPE>* nfa,grail::list<grail::set<grail::state> >&	sub)
{

    fm<TYPE>		*m = new fm<TYPE>;
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
        return nfa;

    // sort if necessary
    nfa->arcs.sort();

    // remove unreachable states
    nfa->reachable_fm();
    nfa->reverse();
    nfa->reachable_fm();
    nfa->reverse();

    // collect the alphabet
    nfa->labels(alphabet);

    // add start subset to the list


    sub += nfa->start_states;

    s1 = 0;
    m->start_states += s1;
    if (debug>8)
    {std::cout<<"in fm:subset(), added start states: 0"<<std::endl;}


        // if some start state is also a final state, add
        // the new start state to the list of final states
        temp2.intersect(nfa->final_states, nfa->start_states);
        if (temp2.size() > 0)
            m->final_states += s1;
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
                    target += temp.sinks(temp2);
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
                        {m->final_states += s2;
    if (debug>8)
    {std::cout<<"in fm:subset(), added final states: 0"<<s2.value()<<std::endl;}}
                }
                else
                    s2 = k;

                t.assign(s1, alphabet[j], s2);
                m->arcs += t;
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


DFA_map<NetTransition,StateData>* from_grail_to_astl(fm<TYPE> *dfa, NetworkModel* net)
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

    DFA_map<NetTransition,StateData> *target = new DFA_map<NetTransition,StateData>;

    int n = dfa->number_of_states();
    DFA_map<NetTransition,StateData>::state_type s[n];
    target->new_state(n, s);

    for(int i=0; i<trans.size();i++)
    {
        int index_src = trans[i].get_source().value();
        int index_aim = trans[i].get_sink().value();
        std::pair<std::string,std::string> p = net->conv_int_str[trans[i].get_label()];
        //cout << p.first << "(" << p.second << ")" << endl;
        NetTransition t;
        t.component = net->find_component(p.second);
        t.trans = t.component->model->find_trans(p.first);
        target->set_trans(s[index_src],t,s[index_aim]);

        if(init.member(trans[i].get_source()) != -1)
            target->initial(s[index_src]);
        if(init.member(trans[i].get_sink()) != -1)
            target->initial(s[index_aim]);

        if(fin.member(trans[i].get_source()) != -1)
        {
            target->tag(s[index_src]) = StateData(tag_map[index_src]);
            target->final(s[index_src]) = true;
        }
        if(fin.member(trans[i].get_sink()) != -1)
        {
            target->tag(s[index_aim]) = StateData(tag_map[index_aim]);
            target->final(s[index_aim]) = true;
        }
    }

    return target;
}
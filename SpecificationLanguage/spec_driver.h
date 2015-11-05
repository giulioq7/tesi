#ifndef SPEC_DRIVER_H
#define SPEC_DRIVER_H
# include "specparser.hpp"
#include "componentmodel.h"
#include "networkmodel.h"
#include "system.h"
#include <vector>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

// Tell Flex the lexer's prototype ...
# define YY_DECL \
    yy::spec_parser::symbol_type yylex(spec_driver&)
// ... and declare it for the parser's sake.
YY_DECL;


class spec_driver
{
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & components;
        ar & networks;
        ar & system;
        ar & problem;
    }

public:
    spec_driver();
    spec_driver(int n_comp, int n_net)
    {
        components = vector<ComponentModel>(n_comp);
        networks = vector<NetworkModel>(n_net);
    }
    virtual ~spec_driver();

    vector<ComponentModel> components;
    vector<NetworkModel> networks;
    NetworkModel current_net_model;
    System system;
    Problem problem;


    // Handling the scanner.
    void scan_begin ();
    void scan_end ();
    bool trace_scanning;

    // Run the parser on file F.
    // Return 0 on success.
    int parse (const std::string& f);
    // The name of the file being parsed.
    // Used later to pass the file name to the location tracker.
    std::string file;
    // Whether parser traces should be generated.
    bool trace_parsing;

    // Error handling.
    void error (const yy::location& l, const std::string& m);
    void error (const std::string& m);

    //utils
    bool duplicate_component_model_id(std::string id);
    bool duplicate_network_model_id(std::string id);
    vector<Component> build_components(vector<std::string> ids, std::string name);
    NetworkModel* find_netmodel(std::string id_model);
    SystemNode* find_node(std::string id);
    void adjust_inherited();
    void build_automata_comp();
    void build_dependency_graph();

    //sem checks
    void semantic_checks(ComponentModel cm);
    void semantic_checks(NetworkModel nm);
    void semantic_checks(System sys);
    void semantic_checks(SystemNode node);
    void semantic_checks(Problem pb);
    void semantic_checks(ProblemNode pbn);
};

#endif // SPEC_DRIVER_H

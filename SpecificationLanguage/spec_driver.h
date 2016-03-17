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

/**
 * @brief The spec_driver class: sintax analysis and semantics driver class
 * @date Febbraio 2016
 * @author Giulio Quarenghi
 */
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

    /**
     * @brief components: vector of component models
     */
    vector<ComponentModel> components;
    /**
     * @brief networks: vector ok network models
     */
    vector<NetworkModel> networks;
    /**
     * @brief current_net_model: current network model
     */
    NetworkModel current_net_model;
    /**
     * @brief current_patt_maxl: true if the language of current pattern is max
     */
    bool current_patt_maxl;
    /**
     * @brief current_pattern
     */
    Pattern current_pattern;
    /**
     * @brief system
     */
    System system;
    /**
     * @brief problem
     */
    Problem problem;


    // Handling the scanner.
    /**
     * @brief scan_begin
     */
    void scan_begin ();
    /**
     * @brief scan_end
     */
    void scan_end ();
    /**
     * @brief trace_scanning: keeps track while scanning (verbose)
     */
    bool trace_scanning;

    // Run the parser on file F.
    // Return 0 on success.
    /**
     * @brief parse
     * @param f: input file to be parsed
     * @return 0 if success
     */
    int parse (const std::string& f);
    // The name of the file being parsed.
    // Used later to pass the file name to the location tracker.
    /**
     * @brief file
     */
    std::string file;
    // Whether parser traces should be generated.
    /**
     * @brief trace_parsing
     */
    bool trace_parsing;

    // Error handling.
    /**
     * @brief error
     * @param l
     * @param m
     */
    void error (const yy::location& l, const std::string& m);
    /**
     * @brief error
     * @param m
     */
    void error (const std::string& m);

    //utils
    /**
     * @brief duplicate_component_model_id: checks if current ID is already used
     * @param id
     * @return true if duplicated, false otherwise
     */
    bool duplicate_component_model_id(std::string id);
    /**
     * @brief duplicate_network_model_id: checks if current ID is already used
     * @param id
     * @return true if duplicated, false otherwise
     */
    bool duplicate_network_model_id(std::string id);
    /**
     * @brief build_components: build network components from a list of ids
     * @param ids list of names
     * @param name name of component model
     * @return vector of NetComponent
     */
    vector<NetComponent> build_components(vector<std::string> ids, std::string name);
    /**
     * @brief find_netmodel: find a network model from id
     * @param id_model name
     * @return pointer to NetworkModel
     */
    NetworkModel* find_netmodel(std::string id_model);
    /**
     * @brief find_node: find a system node from id
     * @param id name
     * @return pointer to SystemNode
     */
    SystemNode* find_node(std::string id);
    /**
     * @brief adjust_inherited: adjusts viewer, ruler and initial states of all nodes
     */
    void adjust_inherited();
    /**
     * @brief build_automata_comp: builds component models automata (initial state missed)
     */
    void build_automata_comp();
    /**
     * @brief build_dependency_graph: builds an automata representing dependencies beetwen system nodes (pattern events links)
     */
    void build_dependency_graph();
    /**
     * @brief build_Isp: builds index spaces from local observations
     */
    void build_Isp();

    //sem checks
    /**
     * @brief semantic_checks
     * @param cm
     */
    void semantic_checks(ComponentModel cm);
    /**
     * @brief semantic_checks
     * @param nm
     */
    void semantic_checks(NetworkModel nm);
    /**
     * @brief semantic_checks
     * @param sys
     */
    void semantic_checks(System sys);
    /**
     * @brief semantic_checks
     * @param node
     */
    void semantic_checks(SystemNode node);
    /**
     * @brief semantic_checks
     * @param pb
     */
    void semantic_checks(Problem pb);
    /**
     * @brief semantic_checks
     * @param pbn
     */
    void semantic_checks(ProblemNode pbn);
};

/**
 * @brief operator << overload of stream operator to print all data info
 * @param out
 * @param d driver
 * @return
 */
std::ostream& operator<<(std::ostream& out, const spec_driver& d);

#endif // SPEC_DRIVER_H

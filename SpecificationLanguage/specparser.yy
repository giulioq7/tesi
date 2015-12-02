%skeleton "lalr1.cc"
%defines
%define parser_class_name {spec_parser}
%define api.value.type variant
%define api.token.constructor
%define parse.assert

%code requires
{
# include <string>
# include <list>
# include "system.h"
# include "problem.h"
class spec_driver;
}
// The parsing context.
%param { spec_driver& driver}
%locations
%initial-action
{
  // Initialize the initial location.
  @$.begin.filename = @$.end.filename = &driver.file;
};
%define parse.trace
%define parse.error verbose
%code
{
#include <cstdlib>
#include <sstream>
# include "utils.h"
#include "ostream_util.h"
# include "spec_driver.h"

using namespace std;

extern yy::location loc;
}

//%define api.token.prefix {TOK_}
%token
COMPONENT
MODEL
IS
END
EVENT
INPUT
OUTPUT
STATE
TRANSITION
NETWORK
LINK
PATTERN
INITIAL
VIEWER
RULER
SYSTEM
NODE
ROOT
EMERGENCE
PROBLEM
OBS
END_OF_FILE  0  "end of file"
COMMA ","
COLON ":"
SEMI_COLON ";"
DOT "."
EQUALS "="
DOUBLE_EQUALS "=="
ARROW "->"
L_BRACKET "("
R_BRACKET ")"
L_SQUARE_BRACKET "["
R_SQUARE_BRACKET "]"
L_CURLY_BRACKET "{"
R_CURLY_BRACKET "}"
AND "&"
PIPE "|"
STAR "*"
PLUS "+"
OPT "?"
TILDE "~"
;

%token <string> ID "id"

%type <ComponentModel>  comp_model_decl
%type <vector<string> > event_decl
%type <vector<string> > input_decl
%type <vector<string> > output_decl
%type <vector<string> > state_decl
%type <vector<string> > id_list
%type <vector<Transition> > transition_decl
%type <vector<Transition> > trans_decl_list
%type <Transition> trans_decl
%type <pair<string,string> > event
%type <vector<pair<string,string> > > opt_ref_list
%type <vector<pair<string,string> > > ref_list
%type <pair<string,string> > ref
%type <NetworkModel>  net_model_decl
%type <vector<Component> > component_section
%type <vector<pair<pair<string,string>,pair<string,string> > > > link_section
%type <vector<Component> > decl_list
%type <vector<Component> > decl
%type <vector<pair<pair<string,string>,pair<string,string> > > > link_list
%type <pair<pair<string,string>,pair<string,string> > > link_decl
%type <vector<Pattern> > pattern_section
%type <vector<pair<string,string> > > initial_section
%type <map<pair<string,string>,string>  > viewer_section
%type <map<pair<string,string>,string>  > ruler_section
%type <vector<Pattern> > pattern_list
%type <Pattern>  pattern_decl
%type <bool> pattern_op
%type <string> expr
%type <string> term
%type <string> factor
%type <map<pair<string,string>,string>  > map_list
%type <pair<pair<string,string>,string> >   map_decl
%type <System> system_decl
%type <vector<SystemNode> > system_node_list
%type <string> root_section
%type <vector<pair<pair<string,string>,pair<string,string> > > > emergence_section
%type <SystemNode> system_node
%type <Problem> problem_decl
%type <vector<ProblemNode> > problem_node_list
%type <ProblemNode> problem_node
%type <vector<string> > obs_section;
%type <vector<string> > opt_id_list;

//%printer { yyoutput << $$; } <*>;
%printer { yyoutput << $$; } <string>;
%printer { yyoutput << $$; } <ComponentModel>;
%printer { yyoutput << $$; } <Transition>;
%printer { yyoutput << $$; } <NetworkModel>;



%%

specification   : spec_list{  }
                ;

spec_list       : spec DOT spec_list {  }
                | spec DOT { }
                ;

spec            : model_decl { }
                | system_decl { driver.system = System($1); }
                | problem_decl { driver.problem = Problem($1); }
                ;

model_decl      : comp_model_decl { ComponentModel($1); driver.components.push_back($1); }
                | net_model_decl
                {
                    NetworkModel($1); driver.networks.push_back($1);
                    vector<NetworkModel>::reverse_iterator it = driver.networks.rbegin();
                    if(it != driver.networks.rend())
                    {
                        driver.networks.back().conv_str_int = driver.current_net_model.conv_str_int;
                        driver.networks.back().conv_int_str = driver.current_net_model.conv_int_str;
                    }
                    driver.semantic_checks(driver.current_net_model);
                }
                ;

comp_model_decl : COMPONENT MODEL ID IS
                    event_decl
                    input_decl
                    output_decl
                    state_decl
                    transition_decl
                  END ID
                  {
                    if(driver.duplicate_component_model_id($3))
                    {
                        string msg = "Semantic error: duplicate component model ID ";
                        msg.append($3);
                        driver.error(loc,msg);
                    }
                    if($3 != $11)
                    {
                        string msg = "Semantic error: wrong end ID ";
                        msg.append($11);
                        driver.error(loc, msg);
                    }
                    $$.name = $3;
                    $$.events = $5;
                    $$.inputs = $6;
                    $$.outputs = $7;
                    $$.states = $8;
                    $$.trans = $9;

                    driver.semantic_checks($$);
                  }
                ;

event_decl      : EVENT id_list SEMI_COLON { $$ = $2; }
                ;

id_list         : ID COMMA id_list
                  { $$.push_back($1); $$ = Utils::merge($$,$3); }
                | ID { $$.push_back($1); }
                ;

input_decl      : INPUT id_list SEMI_COLON { $$ = $2; }
                | { }
                ;

output_decl     : OUTPUT id_list SEMI_COLON { $$ = $2; }
                | { }
                ;

state_decl      : STATE id_list SEMI_COLON { $$ = $2; }
                ;

transition_decl : TRANSITION trans_decl_list SEMI_COLON { $$ = $2; }
                ;

trans_decl_list : trans_decl COMMA trans_decl_list { $$.push_back($1); $$ = Utils::merge($$,$3); }
                | trans_decl  { $$.push_back($1); }
                ;

trans_decl      : ID EQUALS event COMMA ID ARROW ID COMMA L_CURLY_BRACKET opt_ref_list R_CURLY_BRACKET
                  {
                    $$.name = $1; $$.input_event = $3;
                    string s1 = $5; string s2 = $7; $$.s1_s2 = make_pair(s1,s2);
                    $$.out_events = $10;
                  }
                ;

event           : ref { $$ = $1; }
                | L_BRACKET R_BRACKET {}
                ;

ref             : ID L_BRACKET ID R_BRACKET { string s1 = $1; string s2 = $3; $$ = make_pair(s1,s2); }
                ;

opt_ref_list    : ref_list { $$ = $1; }
                | {}
                ;

ref_list        : ref COMMA ref_list { $$.push_back($1); $$ = Utils::merge($$,$3); }
                | ref { $$.push_back($1); }
                ;

net_model_decl  : NETWORK MODEL ID IS
                    component_section
                    {
                        driver.current_net_model = NetworkModel();
                        driver.current_net_model.components = $5;
                    }
                    input_decl
                    output_decl
                    link_section
                    pattern_section
                    initial_section
                    viewer_section
                    ruler_section
                  END ID
                  {
                    if(driver.duplicate_network_model_id($3))
                    {
                        string msg = "Semantic error: duplicate network model ID ";
                        msg.append($3);
                        driver.error(loc,msg);
                    }
                    if($3 != $15)
                    {
                        string msg = "Semantic error: wrong end ID ";
                        msg.append($15);
                        driver.error(loc, msg);
                    }
                    $$.name = $3;
                    $$.components = $5;
                    $$.inputs = $7;
                    $$.outputs = $8;
                    $$.links = $9;
                    $$.patterns = $10;
                    $$.initials = $11;
                    $$.viewer = $12;
                    $$.ruler = $13;

                    driver.semantic_checks($$);
                  }
                ;

component_section : COMPONENT decl_list SEMI_COLON { $$ = $2; }
                  ;

decl_list         : decl COMMA decl_list { $$ = Utils::merge($$,$1); $$ = Utils::merge($$,$3); }
                  | decl { $$ = Utils::merge($$,$1); }
                  ;

decl              : id_list COLON ID { $$ = driver.build_components($1,$3);  }
                  ;

link_section      : LINK link_list SEMI_COLON { $$ = $2; }
                  | { }
                  ;

link_list         : link_decl COMMA link_list { $$.push_back($1); $$ = Utils::merge($$,$3); }
                  | link_decl { $$.push_back($1); }
                  ;

link_decl         : ref ARROW ref {  $$ = make_pair($1,$3); }
                  ;

pattern_section   : PATTERN pattern_list SEMI_COLON { $$ = $2; }
                  | { }
                  ;

pattern_list      : pattern_decl COMMA pattern_list { driver.current_pattern = Pattern(); $$.push_back($1); $$ = Utils::merge($$,$3); }
                  | pattern_decl { driver.current_pattern = Pattern(); $$.push_back($1); }
                  ;

pattern_decl      : ref pattern_op expr
                    {
                        $$ = Pattern($1.first);
                        $$.set_terminal_id($1.second);
                        if($2)
                            $$.choose_max_language();
                        else
                            $$.assign_language(driver.current_pattern.get_language());
                        $$.set_expr($3);
                        driver.current_pattern.assign_language(vector<int>());
                    }
                  ;

pattern_op        : EQUALS { $$ = false; driver.current_patt_maxl = false; }
                  | DOUBLE_EQUALS { $$ = true; driver.current_patt_maxl = true; }
                  ;

expr              : expr PIPE term { $$.append($1); $$.append("|"); $$.append($3); }
                  | term { $$.append($1); }
                  ;

term              : term factor { $$.append($1); $$.append($2); }
                  | term AND factor { $$.append("("); $$.append($1); $$.append($3); $$.append("|"); $$.append($3); $$.append($1); $$.append(")"); }
                  | factor { $$.append($1); }
                  ;

factor            : factor STAR { $$.append($1); $$.append("*"); }
                  | factor PLUS { $$.append($1); $$.append($1); $$.append("*");}
                  | factor OPT { $$.append("("); $$.append($1); $$.append("|"); $$.append("\"\""); $$.append(")"); }
                  | L_BRACKET expr R_BRACKET { $$.append("("); $$.append($2); $$.append(")"); }
                  | TILDE ref
                    { string str = $2.first; str.append("("); str.append($2.second); str.append(")");
                      if(driver.current_patt_maxl)
                        $$.append(driver.current_net_model.not_trans(str));
                      else
                      {
                          pair<string,string> p = make_pair($2.first, $2.second);
                          if(driver.current_net_model.conv_str_int.find(p) == driver.current_net_model.conv_str_int.end())
                          {
                              driver.current_net_model.conv_str_int[p] = driver.current_net_model.count;
                              driver.current_net_model.conv_int_str[driver.current_net_model.count] = p;
                              driver.current_net_model.count++;
                              //driver.current_pattern.add_trans(driver.current_net_model.conv_str_int[p]);
                          }
                          int val = driver.current_net_model.conv_str_int[p];
                          vector<int> l = driver.current_pattern.get_language();
                          if(!Utils::contain(l,val))
                              driver.current_pattern.add_trans(val);
                          $$.append("(");
                          $$.append("~");
                          stringstream ss;
                          ss << driver.current_net_model.conv_str_int[p];
                          $$.append(ss.str());
                          $$.append(")");
                      }
                    }
                  | ref
                  { //$$.append($1.first); $$.append("("); $$.append($1.second); $$.append(")");
                    //string str = $1.first; str.append("("); str.append($1.second); str.append(")");
                    pair<string,string> p = make_pair($1.first, $1.second);
                    if(driver.current_net_model.conv_str_int.find(p) == driver.current_net_model.conv_str_int.end())
                    {
                        driver.current_net_model.conv_str_int[p] = driver.current_net_model.count;
                        driver.current_net_model.conv_int_str[driver.current_net_model.count] = p;
                        driver.current_net_model.count++;
                        //driver.current_pattern.add_trans(driver.current_net_model.conv_str_int[p]);
                    }
                    int val = driver.current_net_model.conv_str_int[p];
                    vector<int> l = driver.current_pattern.get_language();
                    if(!Utils::contain(l,val))
                        driver.current_pattern.add_trans(val);
                    $$.append("(");
                    stringstream ss;
                    ss << driver.current_net_model.conv_str_int[p];
                    $$.append(ss.str());
                    $$.append(")");
                  }
                  ;

initial_section   : INITIAL ref_list SEMI_COLON { $$ = $2; }
                  | { }
                  ;

viewer_section    : VIEWER map_list SEMI_COLON { $$ = $2; }
                  | { }
                  ;

map_list          : map_decl COMMA map_list
                    {
                        $$.insert($1);

                        for(map<pair<string,string>,string>::iterator it = $3.begin(); it != $3.end(); it++)
                        {
                            pair<map<pair<string,string>,string>::iterator,bool> res;
                            res = $$.insert(*it);
                            //verifies existance of the same key in the map
                            if(res.second == false)
                            {
                                string msg = "duplicate network transition map ";
                                msg.append($1.first.first); msg.append("("); msg.append($1.first.second); msg.append(")");
                                driver.error(loc,msg);
                            }
                        }
                    }
                  | map_decl
                    { $$.insert($1); }
                  ;

map_decl          : ref ARROW ID { $$.first = $1; $$.second = $3; }
                  ;

ruler_section     : RULER map_list SEMI_COLON { $$ = $2; }
                  | { }
                  ;

system_decl       : SYSTEM ID IS
                        system_node_list
                        root_section
                        emergence_section
                    END ID
                    {
                        if(driver.system.name != "")
                            driver.error(loc,"Multiple system declaration (system must be unique)");
                        if($2 != $8)
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append($8);
                            driver.error(loc, msg);
                        }
                        $$.name = $2;
                        $$.node_list = $4;
                        $$.id_root = $5;
                        $$.emergence = $6;

                        driver.semantic_checks($$);
                    }
                  ;

system_node_list  : system_node COMMA system_node_list { $$.push_back($1); $$ = Utils::merge($$,$3); }
                  | system_node SEMI_COLON { $$.push_back($1); }
                  ;

system_node       : NODE ID COLON ID IS
                        initial_section
                        viewer_section
                        ruler_section
                    END ID
                    {
                        if($2 != $10)
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append($10);
                            driver.error(loc, msg);
                        }
                        $$ = SystemNode($2);
                        $$.net_model = driver.find_netmodel($4);
                        $$.initials = $6;
                        $$.viewer = $7;
                        $$.ruler = $8;
                    }
                   ;

root_section      : ROOT ID SEMI_COLON { $$ = $2; }
                  | { }
                  ;

emergence_section : EMERGENCE link_list SEMI_COLON { $$ = $2; }
                  | { }
                  ;

problem_decl      : PROBLEM ID IS
                        problem_node_list
                    END ID
                    {
                        if(driver.problem.name != "")
                            driver.error(loc,"Multiple problem declaration (problem must be unique)");
                        if($2 != $6)
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append($6);
                            driver.error(loc, msg);
                        }
                        $$.name = $2;
                        $$.nodes = $4;

                        driver.semantic_checks($$);
                    }
                  ;

problem_node_list : problem_node COMMA problem_node_list { $$.push_back($1); $$ = Utils::merge($$,$3); }
                  | problem_node SEMI_COLON { $$.push_back($1); }
                  ;

problem_node      : NODE ID IS
                        initial_section
                        viewer_section
                        obs_section
                        ruler_section
                    END ID
                    {
                        if($2 != $9)
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append($9);
                            driver.error(loc, msg);
                        }
                        $$ = ProblemNode($2);
                        $$.ref_node = driver.find_node($2);
                        $$.initials = $4;
                        $$.viewer = $5;
                        $$.observation = $6;
                        $$.ruler = $7;
                    }
                  ;

obs_section       : OBS L_SQUARE_BRACKET opt_id_list R_SQUARE_BRACKET SEMI_COLON
                    { $$ = $3; }
                  ;

opt_id_list       : id_list { $$ = $1; }
                  | { }
                  ;


%%

void yy::spec_parser::error (const location_type& l, const std::string& m)
{
    driver.error (l, m);
}


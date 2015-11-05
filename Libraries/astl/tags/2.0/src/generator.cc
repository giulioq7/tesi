#include <astl.h>
#include <random.h>
#include <functional>
#include <iostream>
#include <ctime>
#include <cstdlib>
#ifndef _MSC_VER
#include <unistd.h>
#endif
#include <stream.h>
#include <ccopy.h>

using namespace std;
using namespace astl;


void usage(const string& exe)
{
  cerr << "Usage: " << exe 
       << " [-vm] [-t x] [-f x] [-d depth] [-s seed] {tree|dag|cyclic}" << endl;
  cerr << "       -v\tsend comments to standard error output" << endl;
  cerr << "       -m\tdo not mark states during traversal (dag)" << endl;
  cerr << "       -t\tprobability for a transition to be defined (default 0.5)" << endl;
  cerr << "       -f\tprobability for a state to be final (default 0.1)" << endl;
  cerr << "       -d\tmax depth (default 3)" << endl;
  cerr << "       -s\trandom number generator seed (default time)" << endl;
  exit(1);
}

int main(int argc, char** argv)
{
  bool verbose = false;
  int depth = 3;
  int seed = time(0);
  double t_prob = .5;
  double f_prob = .1;
  const char* exe = argv[0];
  bool mark = true;
  for(int c; (c = getopt(argc, argv, "vmf:t:d:s:")) != EOF;)
    switch(c) {
    case 't' : 
      if (optarg) t_prob = atof(optarg); else usage(exe);
      break;
    case 'f' :
      if (optarg) f_prob = atof(optarg); else usage(exe);
      break;
    case 'd' :
      if (optarg) depth = atoi(optarg); else usage(exe);
      break;
    case 's' :
      if (optarg) seed = atoi(optarg); else usage(exe);
      break;
    case 'v' :
      verbose = true;
      break;
    case 'm' :
      mark = false;
      break;
    default :
      usage(exe);
    }

  string structure;
  if (optind < argc) structure = argv[optind];
  else usage(exe);
  if (structure == "tree") {
    if (verbose) cerr << "random tree automaton [depth " << depth << ", tprob " << t_prob
		      << ", fprob " << f_prob << ", seed " << seed << "]" << endl;
    dump(cout, dfirstc(tree_generator<range<char, 'a', 'z'> >(depth, t_prob, f_prob, seed)));
  }
  else
    if (structure == "dag") {
      if (verbose) cerr << "random DAG automaton [depth " << depth << ", tprob " << t_prob
			<< ", fprob " << f_prob << ", seed " << seed << "]" << endl;
      if (mark) 
	dump(cout, dfirst_markc(dag_generator<range<char, 'a', 'z'> >(depth, t_prob, f_prob, seed)));
      else
	dump(cout, dfirstc(dag_generator<range<char, 'a', 'z'> >(depth, t_prob, f_prob, seed)));
    }
    else
      if (structure == "cyclic") {
	if (verbose) cerr << "random cyclic automaton [depth " << depth << ", tprob " << t_prob
			  << ", fprob " << f_prob << ", seed " << seed << "]" << endl;
	dump(cout, dfirst_markc(cyclic_generator<range<char, 'a', 'z'> >(depth, t_prob, f_prob, seed)));
      }
      else usage(exe);

  return 0;
}

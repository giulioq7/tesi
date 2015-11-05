#include <astl.h>
#include <combinatory.h>
#include <stream.h>
#include <iostream>

using namespace std;
using namespace astl;


void usage(const char* exe)
{
  cerr << "Usage: " << exe << " [-v] [-m] n" << endl;
  exit(1);
}

int main(int argc, char** argv)
{
  config c(argc, argv, 0);

  int n;
  if (c.arg_pos < argc) n = atoi(argv[c.arg_pos]);
  else usage(argv[0]);
  if (n < 1) usage(argv[0]);

  if (c.state_mark)
    dump(cout, dfirst_markc(permutationc(n)));
  else
    dump(cout, dfirstc(permutationc(n)));
  
  return 0;
}

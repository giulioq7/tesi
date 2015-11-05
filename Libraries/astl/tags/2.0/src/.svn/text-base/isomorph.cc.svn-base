#include <astl.h>
#include <set_operation.h>
#include <stream.h>
#include <string>
#include <iostream>

using namespace std;
using namespace astl;


const string USAGE = "";
const string DESCRIPTION = "return 0 if the DFA read from stdin is \
the same as the one read from file";

int main(int argc, char **argv)
{
  config c(argc, argv, 2, "", USAGE, DESCRIPTION);
  
  if (isomorph(istream_cursor<DFA_map<plain> >(cin), 
	       istream_cursor<DFA_map<plain> >(c.aux_input)))
    return 0;
  cerr << "DFAs differ" << endl;
  return 1;
}


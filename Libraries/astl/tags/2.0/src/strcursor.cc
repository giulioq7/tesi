#include <astl.h>
#include <stream.h>
#include <ccopy.h>
#include <language.h>
#include <string>
#include <iostream>

using namespace std;
using namespace astl;


const string USAGE = "word";
const string DESCRIPTION = "output the DFA for word";

int main(int argc, char** argv)
{
  config c(argc, argv, 0, "", USAGE, DESCRIPTION);
  dump(cout, dfirstc(stringc(argv[1])));
  return 0;
}  

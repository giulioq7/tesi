#include <astl.h>
#include <set_operation.h>
#include <tools.h>
#include <ccopy.h>
#include <cursor.h>
#include <stream.h>

using namespace astl;

void usage(const char* exe)
{
  cerr << "Usage: " << exe << " [-v]" << endl;
  exit(1);
}

int main(int argc, char** argv)
{
  config c(argc, argv, 0);
  dump(cout, dfirst_markc(sigma_starc<plain>()));
  return 0;
}
  

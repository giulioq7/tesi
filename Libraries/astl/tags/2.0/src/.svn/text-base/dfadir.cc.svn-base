
// not implemented for Visual C++
#ifndef _MSC_VER

#include <astl.h>
#include <dir_cursor.h>
#include <stream.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <cstdio>         // perror
#include <sys/types.h>    // opendir
#include <dirent.h>

using namespace std;
using namespace astl;


const string USAGE = "[dir]";
const string DESCRIPTION = "build a DFA from a directory tree\n   \
dir\troot directory (initial state)";

#endif

int main(int argc, char **argv)
{
#ifndef _MSC_VER

  config c(argc, argv, 0, "", USAGE, DESCRIPTION);
  if (find(c.begin(), c.end(), "-h") != c.end())
    c.usage();

  string root = ".";
  if (c.arg_pos < argc) root = argv[c.arg_pos];

  DIR *just_testing = opendir(root.c_str());
  if (just_testing == NULL) {
    string error_msg = string(argv[0]) + ": " + root;
    perror(error_msg.c_str());
    exit(2);
  }
  closedir(just_testing);
  if (c.state_mark)
    dump(cout, dfirst_markc(directoryc(root)));
  else
    dump(cout, dfirstc(directoryc(root)));

#endif

  return 0;
}
  
    

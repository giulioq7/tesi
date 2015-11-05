/* File grail.h
 Licence: LGPL
 Added in this format by CC to the Grail project on February 1, 2014

*/
/* namespace grail: added by Brandon Stewart, July 2015 */
using namespace grail;

#define MAX_PATH 512

char* Version=(char*)"Grail Version 3.4.4";

void get_one(fm<int>& a, int argc, char** argv, char* my_name);
void get_two(fm<int>& a, fm<int>& b, int argc, char** argv, char* my_name);
void get_one(re<int>& a, int argc, char** argv, char* my_name);
void get_two(re<int>& a, re<int>& b, int argc, char** argv, char* my_name);
void get_one(fl<int>& a, int argc, char** argv, char* my_name);
void get_two(fl<int>& a, fl<int>& b, int argc, char** argv, char* my_name);
void get_two(fl<int>& a, string<int>& b, int argc, char** argv, char* my_name);
void get_one(afa<int>& a, int argc, char** argv, char* my_name);
void get_two(afa<int>& a, afa<int>& b, int argc, char** argv, char* my_name);
void get_afa(afa<int>& a, int argc, char** argv, char* my_name);

// The following two functions were added for cover automata in 3.1
void get_one(fcm<int>& a, int argc, char** argv, char* my_name);
void get_two(fcm<int>& a, fcm<int>& b, int argc, char** argv, char* my_name);

//Displays a short help note if the first argument is '--help'
void print_help(const char* filter_name);

//Displays the version of grail that we're using, if the first argument is '--version'
void print_version(char*);

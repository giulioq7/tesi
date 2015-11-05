#include <astl.h>
#include <stream.h>

using namespace astl;

const string USAGE = "{-d | -b [-o]} file";
const string DESCRIPTION = "dump/build a compact DFA\n   \
-d\tdump to stdout the DFA read from compact file\n \
-b\twrite a compact DFA built from stdin to file\n \
-o\ttry to optimize space";
ofstream output;
ifstream input;
int optimize;
string exe;

template <typename DFA>
void run(DFA& dfa, const config &c, string file)
{
  dfa.initial(clone(dfa, istream_cursor<DFA>(cin)));
  DFA_compact<DFA> cmp(dfa, optimize);
  if (c.verbose_mode) 
    cmp.stats(cerr);

  output.open(file.c_str());
  if (!output) {
    string msg = exe + ": ";
    perror(msg.c_str());
    exit(2);
  }
  cmp.write(output);
  output.close();
  if (c.verbose_mode)
    cerr << c.argv[0] << ": compact DFA written to " 
	 << c.argv[c.arg_pos] << endl;
} 

int main(int argc, char** argv)
{
  exe = argv[0];
  bool read_mode = true;
  config c(argc, argv, 1, "map bin", USAGE, DESCRIPTION);
  if (find(c.options.begin(), c.options.end(), string("-d"))
      != c.options.end())
    read_mode = true;
  else
  if (find(c.options.begin(), c.options.end(), string("-b"))
      != c.options.end())
    read_mode = false;
  else c.usage();

  if (find(c.options.begin(), c.options.end(), string("-o")) 
      != c.options.end())
    if (read_mode == true) c.usage();
    else
      optimize = 20;
  else
    optimize = 2;

  if (c.verbose_mode) cerr << c.argv[0] << ": optimize " 
			   << (optimize == 0 ? "yes" : "no") << endl;
  if (c.arg_pos >= argc) c.usage();
  if (read_mode) {
    input.open(argv[c.arg_pos]);
    if (!input) {
      string msg = string(argv[0]) + ": ";
      perror(msg.c_str());
      exit(2);
    }
    DFA_compact<DFA_bin<> > dfa;
    if (!dfa.read(input)) {
      string msg = string(argv[0]) + ": ";
      perror(msg.c_str());
      exit(2);
    }
    if (c.state_mark)
      dump(cout, dfirst_markc(dfa));
    else
      dump(cout, dfirstc(dfa));
  }
  else {
    if (c.representation == "map") {
      DFA_map<> dfa;
      run(dfa, c, argv[c.arg_pos]);
    }
    if (c.representation == "bin") {
      DFA_bin<> dfa;
      run(dfa, c, argv[c.arg_pos]);
    }
  }
  return 0;
}




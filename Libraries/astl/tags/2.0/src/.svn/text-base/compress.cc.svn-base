#include <astl.h>
#include <stream.h>

using namespace astl;

const string USAGE = "{-d | -b} file";
const string DESCRIPTION = "dump/build a compressed DFA\n   \
-d\tdump to stdout the DFA read from compressed file\n \
-b\twrite a compressed DFA built from stdin to file";
ofstream output;
ifstream input;
string exe;

template <typename DFA>
void run(DFA& dfa, const config &c, string file)
{
  dfa.initial(clone(dfa, istream_cursor<DFA>(cin)));
  FA_compress<> cmp;
  if (c.state_mark)
    cmp.compress(bfirst_markc(dfa));
  else
    cmp.compress(bfirstc(dfa));

  output.open(file.c_str());
  if (!output) {
    string msg = exe + ": ";
    perror(msg.c_str());
    exit(2);
  }
  cmp.write(output);
  output.close();
  if (c.verbose_mode)
    cerr << c.argv[0] << ": compressed DFA written to " 
	 << c.argv[c.arg_pos] << endl;
} 

int main(int argc, char** argv)
{
  exe = argv[0];
  bool read_mode = true;
  config c(argc, argv, 1, "all", USAGE, DESCRIPTION);
  if (find(c.options.begin(), c.options.end(), string("-d"))
      != c.options.end())
    read_mode = true;
  else
  if (find(c.options.begin(), c.options.end(), string("-b"))
      != c.options.end())
    read_mode = false;
  else c.usage();

  if (c.arg_pos >= argc) c.usage();
  if (read_mode) {
    input.open(argv[c.arg_pos]);
    if (!input) {
      string msg = string(argv[0]) + ": ";
      perror(msg.c_str());
      exit(2);
    }
    FA_compress<> dfa;
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
    if (c.representation == "matrix") {
      DFA_matrix<> dfa;
      run(dfa, c, argv[c.arg_pos]);
    }
    if (c.representation == "mtf") {
      DFA_mtf<> dfa;
      run(dfa, c, argv[c.arg_pos]);
    }
    if (c.representation == "tr") {
      DFA_tr<> dfa;
      run(dfa, c, argv[c.arg_pos]);
    }
  }
  return 0;
}





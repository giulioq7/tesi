%{
# include <string>
# include <iostream>
# include <unistd.h>
# include "spec_driver.h"
# include "specparser.hpp"

# define INPUT_FILE_DIR "./InputFiles/"

using namespace std;

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// The location of the current token.
yy::location loc;

%}
%option noyywrap nounput
%x incl


delimiter	[ \t]
spacing		{delimiter}+
letter		[A-Za-z]
digit		[0-9]
unscore         _
id		{letter}({letter}|{digit}|{unscore})*
eol		\n
comment		\/\/.*


%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}

%%

%{
  // Code run each time yylex is called.
  loc.step ();
%}
#include          BEGIN(incl);
<incl>[ \t]*      /* eat the whitespace */
<incl>[^ \t\n]+   { /* got the include file name */
                 chdir(INPUT_FILE_DIR);
                 FILE *file = fopen( yytext, "r" );

                 if (file == NULL)
                 {
                     cout << "Input file error: included file does not exist" << endl;
                     exit(1);
                 }
                 yyin = file;

                        yypush_buffer_state(yy_create_buffer( yyin, YY_BUF_SIZE ));

                        chdir("../");
                 BEGIN(INITIAL);
                 }

{spacing}	;
{comment}       ;

{eol}		{loc.lines (yyleng);loc.step();}

<<EOF>>    {yypop_buffer_state();

                 if ( !YY_CURRENT_BUFFER )
                     {
                        return yy::spec_parser::make_END_OF_FILE(loc);;
                     }
                }

component   return yy::spec_parser::make_COMPONENT(loc);
model       return yy::spec_parser::make_MODEL(loc);
is          return yy::spec_parser::make_IS(loc);
end         return yy::spec_parser::make_END(loc);
event       return yy::spec_parser::make_EVENT(loc);
input       return yy::spec_parser::make_INPUT(loc);
output      return yy::spec_parser::make_OUTPUT(loc);
state       return yy::spec_parser::make_STATE(loc);
transition  return yy::spec_parser::make_TRANSITION(loc);
network     return yy::spec_parser::make_NETWORK(loc);
link        return yy::spec_parser::make_LINK(loc);
pattern     return yy::spec_parser::make_PATTERN(loc);
initial     return yy::spec_parser::make_INITIAL(loc);
viewer      return yy::spec_parser::make_VIEWER(loc);
ruler       return yy::spec_parser::make_RULER(loc);
system      return yy::spec_parser::make_SYSTEM(loc);
node        return yy::spec_parser::make_NODE(loc);
root        return yy::spec_parser::make_ROOT(loc);
emergence   return yy::spec_parser::make_EMERGENCE(loc);
problem     return yy::spec_parser::make_PROBLEM(loc);
obs         return yy::spec_parser::make_OBS(loc);

","         return yy::spec_parser::make_COMMA(loc);
":"         return yy::spec_parser::make_COLON(loc);
";"         return yy::spec_parser::make_SEMI_COLON(loc);
"."         return yy::spec_parser::make_DOT(loc);
"="         return yy::spec_parser::make_EQUALS(loc);
"=="        return yy::spec_parser::make_DOUBLE_EQUALS(loc);
"->"        return yy::spec_parser::make_ARROW(loc);
"("         return yy::spec_parser::make_L_BRACKET(loc);
")"         return yy::spec_parser::make_R_BRACKET(loc);
"["         return yy::spec_parser::make_L_SQUARE_BRACKET(loc);
"]"         return yy::spec_parser::make_R_SQUARE_BRACKET(loc);
"{"         return yy::spec_parser::make_L_CURLY_BRACKET(loc);
"}"         return yy::spec_parser::make_R_CURLY_BRACKET(loc);
"&"         return yy::spec_parser::make_AND(loc);
"|"         return yy::spec_parser::make_PIPE(loc);
"*"         return yy::spec_parser::make_STAR(loc);
"+"         return yy::spec_parser::make_PLUS(loc);
"?"         return yy::spec_parser::make_OPT(loc);
"~"         return yy::spec_parser::make_TILDE(loc);


{id}        return yy::spec_parser::make_ID(yytext,loc);
.		{cout << "Lexical error: token \"" << yytext << "\" at " << loc << endl; exit(1);}
%%

void spec_driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty() || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str(), "r")))
    {
      error("cannot open " + file + ": " + strerror(errno));
      exit(EXIT_FAILURE);
    }
}

void spec_driver::scan_end ()
{
  fclose(yyin);
}



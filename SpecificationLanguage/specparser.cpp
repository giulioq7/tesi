// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.


// First part of user declarations.

#line 37 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "specparser.hpp"

// User implementation prologue.

#line 51 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 27 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:413

#include <cstdlib>
#include <sstream>
# include "utils.h"
#include "ostream_util.h"
# include "spec_driver.h"

using namespace std;

extern yy::location loc;

#line 65 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:413


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K].location)
/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

# ifndef YYLLOC_DEFAULT
#  define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).begin  = YYRHSLOC (Rhs, 1).begin;                   \
          (Current).end    = YYRHSLOC (Rhs, N).end;                     \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).begin = (Current).end = YYRHSLOC (Rhs, 0).end;      \
        }                                                               \
    while (/*CONSTCOND*/ false)
# endif


// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << std::endl;                  \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE(Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void>(0)
# define YY_STACK_PRINT()                static_cast<void>(0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 151 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:479

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  spec_parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr = "";
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              // Fall through.
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }


  /// Build a parser object.
  spec_parser::spec_parser (spec_driver& driver_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      driver (driver_yyarg)
  {}

  spec_parser::~spec_parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  inline
  spec_parser::by_state::by_state ()
    : state (empty_state)
  {}

  inline
  spec_parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  inline
  void
  spec_parser::by_state::clear ()
  {
    state = empty_state;
  }

  inline
  void
  spec_parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  inline
  spec_parser::by_state::by_state (state_type s)
    : state (s)
  {}

  inline
  spec_parser::symbol_number_type
  spec_parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  inline
  spec_parser::stack_symbol_type::stack_symbol_type ()
  {}


  inline
  spec_parser::stack_symbol_type::stack_symbol_type (state_type s, symbol_type& that)
    : super_type (s, that.location)
  {
      switch (that.type_get ())
    {
      case 48: // comp_model_decl
        value.move< ComponentModel > (that.value);
        break;

      case 61: // net_model_decl
        value.move< NetworkModel > (that.value);
        break;

      case 71: // pattern_decl
        value.move< Pattern > (that.value);
        break;

      case 85: // problem_decl
        value.move< Problem > (that.value);
        break;

      case 87: // problem_node
        value.move< ProblemNode > (that.value);
        break;

      case 81: // system_decl
        value.move< System > (that.value);
        break;

      case 83: // system_node
        value.move< SystemNode > (that.value);
        break;

      case 56: // trans_decl
        value.move< Transition > (that.value);
        break;

      case 72: // pattern_op
        value.move< bool > (that.value);
        break;

      case 77: // viewer_section
      case 78: // map_list
      case 80: // ruler_section
        value.move< map<pair<string,string>,string>   > (that.value);
        break;

      case 68: // link_decl
        value.move< pair<pair<string,string>,pair<string,string> >  > (that.value);
        break;

      case 79: // map_decl
        value.move< pair<pair<string,string>,string>  > (that.value);
        break;

      case 57: // event
      case 58: // ref
        value.move< pair<string,string>  > (that.value);
        break;

      case 42: // "id"
      case 73: // expr
      case 74: // term
      case 75: // factor
        value.move< string > (that.value);
        break;

      case 63: // component_section
      case 64: // decl_list
      case 65: // decl
        value.move< vector<Component>  > (that.value);
        break;

      case 69: // pattern_section
      case 70: // pattern_list
        value.move< vector<Pattern>  > (that.value);
        break;

      case 86: // problem_node_list
        value.move< vector<ProblemNode>  > (that.value);
        break;

      case 82: // system_node_list
        value.move< vector<SystemNode>  > (that.value);
        break;

      case 54: // transition_decl
      case 55: // trans_decl_list
        value.move< vector<Transition>  > (that.value);
        break;

      case 66: // link_section
      case 67: // link_list
      case 84: // emergence_section
        value.move< vector<pair<pair<string,string>,pair<string,string> > >  > (that.value);
        break;

      case 59: // opt_ref_list
      case 60: // ref_list
      case 76: // initial_section
        value.move< vector<pair<string,string> >  > (that.value);
        break;

      case 49: // event_decl
      case 50: // id_list
      case 51: // input_decl
      case 52: // output_decl
      case 53: // state_decl
      case 88: // obs_section
      case 89: // opt_id_list
        value.move< vector<string>  > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

  inline
  spec_parser::stack_symbol_type&
  spec_parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
      switch (that.type_get ())
    {
      case 48: // comp_model_decl
        value.copy< ComponentModel > (that.value);
        break;

      case 61: // net_model_decl
        value.copy< NetworkModel > (that.value);
        break;

      case 71: // pattern_decl
        value.copy< Pattern > (that.value);
        break;

      case 85: // problem_decl
        value.copy< Problem > (that.value);
        break;

      case 87: // problem_node
        value.copy< ProblemNode > (that.value);
        break;

      case 81: // system_decl
        value.copy< System > (that.value);
        break;

      case 83: // system_node
        value.copy< SystemNode > (that.value);
        break;

      case 56: // trans_decl
        value.copy< Transition > (that.value);
        break;

      case 72: // pattern_op
        value.copy< bool > (that.value);
        break;

      case 77: // viewer_section
      case 78: // map_list
      case 80: // ruler_section
        value.copy< map<pair<string,string>,string>   > (that.value);
        break;

      case 68: // link_decl
        value.copy< pair<pair<string,string>,pair<string,string> >  > (that.value);
        break;

      case 79: // map_decl
        value.copy< pair<pair<string,string>,string>  > (that.value);
        break;

      case 57: // event
      case 58: // ref
        value.copy< pair<string,string>  > (that.value);
        break;

      case 42: // "id"
      case 73: // expr
      case 74: // term
      case 75: // factor
        value.copy< string > (that.value);
        break;

      case 63: // component_section
      case 64: // decl_list
      case 65: // decl
        value.copy< vector<Component>  > (that.value);
        break;

      case 69: // pattern_section
      case 70: // pattern_list
        value.copy< vector<Pattern>  > (that.value);
        break;

      case 86: // problem_node_list
        value.copy< vector<ProblemNode>  > (that.value);
        break;

      case 82: // system_node_list
        value.copy< vector<SystemNode>  > (that.value);
        break;

      case 54: // transition_decl
      case 55: // trans_decl_list
        value.copy< vector<Transition>  > (that.value);
        break;

      case 66: // link_section
      case 67: // link_list
      case 84: // emergence_section
        value.copy< vector<pair<pair<string,string>,pair<string,string> > >  > (that.value);
        break;

      case 59: // opt_ref_list
      case 60: // ref_list
      case 76: // initial_section
        value.copy< vector<pair<string,string> >  > (that.value);
        break;

      case 49: // event_decl
      case 50: // id_list
      case 51: // input_decl
      case 52: // output_decl
      case 53: // state_decl
      case 88: // obs_section
      case 89: // opt_id_list
        value.copy< vector<string>  > (that.value);
        break;

      default:
        break;
    }

    location = that.location;
    return *this;
  }


  template <typename Base>
  inline
  void
  spec_parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  spec_parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " ("
        << yysym.location << ": ";
    switch (yytype)
    {
            case 42: // "id"

#line 128 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 534 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 48: // comp_model_decl

#line 129 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ComponentModel > (); }
#line 541 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 56: // trans_decl

#line 130 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Transition > (); }
#line 548 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 61: // net_model_decl

#line 131 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< NetworkModel > (); }
#line 555 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 73: // expr

#line 128 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 562 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 74: // term

#line 128 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 569 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 75: // factor

#line 128 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 576 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;


      default:
        break;
    }
    yyo << ')';
  }
#endif

  inline
  void
  spec_parser::yypush_ (const char* m, state_type s, symbol_type& sym)
  {
    stack_symbol_type t (s, sym);
    yypush_ (m, t);
  }

  inline
  void
  spec_parser::yypush_ (const char* m, stack_symbol_type& s)
  {
    if (m)
      YY_SYMBOL_PRINT (m, s);
    yystack_.push (s);
  }

  inline
  void
  spec_parser::yypop_ (unsigned int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  spec_parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  spec_parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  spec_parser::debug_level_type
  spec_parser::debug_level () const
  {
    return yydebug_;
  }

  void
  spec_parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  inline spec_parser::state_type
  spec_parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  inline bool
  spec_parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  spec_parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  spec_parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The locations where the error started and ended.
    stack_symbol_type yyerror_range[3];

    /// The return value of parse ().
    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    // User initialization code.
    #line 20 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 695 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:741

    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, yyla);

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << std::endl;

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:

    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
        try
          {
            symbol_type yylookahead (yylex (driver));
            yyla.move (yylookahead);
          }
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, yyla);
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_(yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
        switch (yyr1_[yyn])
    {
      case 48: // comp_model_decl
        yylhs.value.build< ComponentModel > ();
        break;

      case 61: // net_model_decl
        yylhs.value.build< NetworkModel > ();
        break;

      case 71: // pattern_decl
        yylhs.value.build< Pattern > ();
        break;

      case 85: // problem_decl
        yylhs.value.build< Problem > ();
        break;

      case 87: // problem_node
        yylhs.value.build< ProblemNode > ();
        break;

      case 81: // system_decl
        yylhs.value.build< System > ();
        break;

      case 83: // system_node
        yylhs.value.build< SystemNode > ();
        break;

      case 56: // trans_decl
        yylhs.value.build< Transition > ();
        break;

      case 72: // pattern_op
        yylhs.value.build< bool > ();
        break;

      case 77: // viewer_section
      case 78: // map_list
      case 80: // ruler_section
        yylhs.value.build< map<pair<string,string>,string>   > ();
        break;

      case 68: // link_decl
        yylhs.value.build< pair<pair<string,string>,pair<string,string> >  > ();
        break;

      case 79: // map_decl
        yylhs.value.build< pair<pair<string,string>,string>  > ();
        break;

      case 57: // event
      case 58: // ref
        yylhs.value.build< pair<string,string>  > ();
        break;

      case 42: // "id"
      case 73: // expr
      case 74: // term
      case 75: // factor
        yylhs.value.build< string > ();
        break;

      case 63: // component_section
      case 64: // decl_list
      case 65: // decl
        yylhs.value.build< vector<Component>  > ();
        break;

      case 69: // pattern_section
      case 70: // pattern_list
        yylhs.value.build< vector<Pattern>  > ();
        break;

      case 86: // problem_node_list
        yylhs.value.build< vector<ProblemNode>  > ();
        break;

      case 82: // system_node_list
        yylhs.value.build< vector<SystemNode>  > ();
        break;

      case 54: // transition_decl
      case 55: // trans_decl_list
        yylhs.value.build< vector<Transition>  > ();
        break;

      case 66: // link_section
      case 67: // link_list
      case 84: // emergence_section
        yylhs.value.build< vector<pair<pair<string,string>,pair<string,string> > >  > ();
        break;

      case 59: // opt_ref_list
      case 60: // ref_list
      case 76: // initial_section
        yylhs.value.build< vector<pair<string,string> >  > ();
        break;

      case 49: // event_decl
      case 50: // id_list
      case 51: // input_decl
      case 52: // output_decl
      case 53: // state_decl
      case 88: // obs_section
      case 89: // opt_id_list
        yylhs.value.build< vector<string>  > ();
        break;

      default:
        break;
    }


      // Compute the default @$.
      {
        slice<stack_symbol_type, stack_type> slice (yystack_, yylen);
        YYLLOC_DEFAULT (yylhs.location, slice, yylen);
      }

      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
      try
        {
          switch (yyn)
            {
  case 2:
#line 137 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  }
#line 913 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 140 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  }
#line 919 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 141 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 925 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 144 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 931 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 145 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.system = System(yystack_[0].value.as< System > ()); }
#line 937 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 146 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.problem = Problem(yystack_[0].value.as< Problem > ()); }
#line 943 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 149 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { ComponentModel(yystack_[0].value.as< ComponentModel > ()); driver.components.push_back(yystack_[0].value.as< ComponentModel > ()); }
#line 949 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 151 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                    NetworkModel(yystack_[0].value.as< NetworkModel > ()); driver.networks.push_back(yystack_[0].value.as< NetworkModel > ());
                    vector<NetworkModel>::reverse_iterator it = driver.networks.rbegin();
                    if(it != driver.networks.rend())
                    {
                        driver.networks.back().conv_str_int = driver.current_net_model.conv_str_int;
                        driver.networks.back().conv_int_str = driver.current_net_model.conv_int_str;
                    }
                    driver.semantic_checks(driver.current_net_model);
                }
#line 964 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 170 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                    if(driver.duplicate_component_model_id(yystack_[8].value.as< string > ()))
                    {
                        string msg = "Semantic error: duplicate component model ID ";
                        msg.append(yystack_[8].value.as< string > ());
                        driver.error(loc,msg);
                    }
                    if(yystack_[8].value.as< string > () != yystack_[0].value.as< string > ())
                    {
                        string msg = "Semantic error: wrong end ID ";
                        msg.append(yystack_[0].value.as< string > ());
                        driver.error(loc, msg);
                    }
                    yylhs.value.as< ComponentModel > ().name = yystack_[8].value.as< string > ();
                    yylhs.value.as< ComponentModel > ().events = yystack_[6].value.as< vector<string>  > ();
                    yylhs.value.as< ComponentModel > ().inputs = yystack_[5].value.as< vector<string>  > ();
                    yylhs.value.as< ComponentModel > ().outputs = yystack_[4].value.as< vector<string>  > ();
                    yylhs.value.as< ComponentModel > ().states = yystack_[3].value.as< vector<string>  > ();
                    yylhs.value.as< ComponentModel > ().trans = yystack_[2].value.as< vector<Transition>  > ();

                    driver.semantic_checks(yylhs.value.as< ComponentModel > ());
                  }
#line 991 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 194 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 997 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 198 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > ().push_back(yystack_[2].value.as< string > ()); yylhs.value.as< vector<string>  > () = Utils::merge(yylhs.value.as< vector<string>  > (),yystack_[0].value.as< vector<string>  > ()); }
#line 1003 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 199 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > ().push_back(yystack_[0].value.as< string > ()); }
#line 1009 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 202 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 1015 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 203 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1021 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 206 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 1027 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 207 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1033 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 210 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 1039 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 213 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > () = yystack_[1].value.as< vector<Transition>  > (); }
#line 1045 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 216 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > ().push_back(yystack_[2].value.as< Transition > ()); yylhs.value.as< vector<Transition>  > () = Utils::merge(yylhs.value.as< vector<Transition>  > (),yystack_[0].value.as< vector<Transition>  > ()); }
#line 1051 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 217 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > ().push_back(yystack_[0].value.as< Transition > ()); }
#line 1057 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 221 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                    yylhs.value.as< Transition > ().name = yystack_[10].value.as< string > (); yylhs.value.as< Transition > ().input_event = yystack_[8].value.as< pair<string,string>  > ();
                    string s1 = yystack_[6].value.as< string > (); string s2 = yystack_[4].value.as< string > (); yylhs.value.as< Transition > ().s1_s2 = make_pair(s1,s2);
                    yylhs.value.as< Transition > ().out_events = yystack_[1].value.as< vector<pair<string,string> >  > ();
                  }
#line 1067 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 228 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< pair<string,string>  > () = yystack_[0].value.as< pair<string,string>  > (); }
#line 1073 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 229 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {}
#line 1079 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 232 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string s1 = yystack_[3].value.as< string > (); string s2 = yystack_[1].value.as< string > (); yylhs.value.as< pair<string,string>  > () = make_pair(s1,s2); }
#line 1085 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 235 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[0].value.as< vector<pair<string,string> >  > (); }
#line 1091 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 236 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {}
#line 1097 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 239 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[2].value.as< pair<string,string>  > ()); yylhs.value.as< vector<pair<string,string> >  > () = Utils::merge(yylhs.value.as< vector<pair<string,string> >  > (),yystack_[0].value.as< vector<pair<string,string> >  > ()); }
#line 1103 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 240 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[0].value.as< pair<string,string>  > ()); }
#line 1109 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 245 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        driver.current_net_model = NetworkModel();
                        driver.current_net_model.components = yystack_[0].value.as< vector<Component>  > ();
                    }
#line 1118 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 257 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                    if(driver.duplicate_network_model_id(yystack_[12].value.as< string > ()))
                    {
                        string msg = "Semantic error: duplicate network model ID ";
                        msg.append(yystack_[12].value.as< string > ());
                        driver.error(loc,msg);
                    }
                    if(yystack_[12].value.as< string > () != yystack_[0].value.as< string > ())
                    {
                        string msg = "Semantic error: wrong end ID ";
                        msg.append(yystack_[0].value.as< string > ());
                        driver.error(loc, msg);
                    }
                    yylhs.value.as< NetworkModel > ().name = yystack_[12].value.as< string > ();
                    yylhs.value.as< NetworkModel > ().components = yystack_[10].value.as< vector<Component>  > ();
                    yylhs.value.as< NetworkModel > ().inputs = yystack_[8].value.as< vector<string>  > ();
                    yylhs.value.as< NetworkModel > ().outputs = yystack_[7].value.as< vector<string>  > ();
                    yylhs.value.as< NetworkModel > ().links = yystack_[6].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ();
                    yylhs.value.as< NetworkModel > ().patterns = yystack_[5].value.as< vector<Pattern>  > ();
                    yylhs.value.as< NetworkModel > ().initials = yystack_[4].value.as< vector<pair<string,string> >  > ();
                    yylhs.value.as< NetworkModel > ().viewer = yystack_[3].value.as< map<pair<string,string>,string>   > ();
                    yylhs.value.as< NetworkModel > ().ruler = yystack_[2].value.as< map<pair<string,string>,string>   > ();

                    driver.semantic_checks(yylhs.value.as< NetworkModel > ());
                  }
#line 1148 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 284 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = yystack_[1].value.as< vector<Component>  > (); }
#line 1154 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 287 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[2].value.as< vector<Component>  > ()); yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[0].value.as< vector<Component>  > ()); }
#line 1160 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 288 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[0].value.as< vector<Component>  > ()); }
#line 1166 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 291 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = driver.build_components(yystack_[2].value.as< vector<string>  > (),yystack_[0].value.as< string > ());  }
#line 1172 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 294 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > () = yystack_[1].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > (); }
#line 1178 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 295 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1184 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 298 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ().push_back(yystack_[2].value.as< pair<pair<string,string>,pair<string,string> >  > ()); yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > () = Utils::merge(yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > (),yystack_[0].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ()); }
#line 1190 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 299 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ().push_back(yystack_[0].value.as< pair<pair<string,string>,pair<string,string> >  > ()); }
#line 1196 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 302 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  yylhs.value.as< pair<pair<string,string>,pair<string,string> >  > () = make_pair(yystack_[2].value.as< pair<string,string>  > (),yystack_[0].value.as< pair<string,string>  > ()); }
#line 1202 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 305 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Pattern>  > () = yystack_[1].value.as< vector<Pattern>  > (); }
#line 1208 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 306 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1214 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 309 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.current_pattern = Pattern(); yylhs.value.as< vector<Pattern>  > ().push_back(yystack_[2].value.as< Pattern > ()); yylhs.value.as< vector<Pattern>  > () = Utils::merge(yylhs.value.as< vector<Pattern>  > (),yystack_[0].value.as< vector<Pattern>  > ()); }
#line 1220 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 310 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.current_pattern = Pattern(); yylhs.value.as< vector<Pattern>  > ().push_back(yystack_[0].value.as< Pattern > ()); }
#line 1226 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 314 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        yylhs.value.as< Pattern > () = Pattern(yystack_[2].value.as< pair<string,string>  > ().first);
                        yylhs.value.as< Pattern > ().set_terminal_id(yystack_[2].value.as< pair<string,string>  > ().second);
                        if(yystack_[1].value.as< bool > ())
                            yylhs.value.as< Pattern > ().choose_max_language();
                        else
                            yylhs.value.as< Pattern > ().assign_language(driver.current_pattern.get_language());
                        yylhs.value.as< Pattern > ().set_expr(yystack_[0].value.as< string > ());
                        driver.current_pattern.assign_language(vector<int>());
                    }
#line 1241 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 326 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< bool > () = false; driver.current_patt_maxl = false; }
#line 1247 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 327 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< bool > () = true; driver.current_patt_maxl = true; }
#line 1253 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 330 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1259 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 331 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1265 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 334 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1271 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 335 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append(")"); }
#line 1277 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 336 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1283 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 339 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("*"); }
#line 1289 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 340 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("*");}
#line 1295 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 341 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append("\"\""); yylhs.value.as< string > ().append(")"); }
#line 1301 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 342 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(")"); }
#line 1307 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 344 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string str = yystack_[0].value.as< pair<string,string>  > ().first; str.append("("); str.append(yystack_[0].value.as< pair<string,string>  > ().second); str.append(")");
                      if(driver.current_patt_maxl)
                        yylhs.value.as< string > ().append(driver.current_net_model.not_trans(str));
                      else
                      {
                          pair<string,string> p = make_pair(yystack_[0].value.as< pair<string,string>  > ().first, yystack_[0].value.as< pair<string,string>  > ().second);
                          if(driver.current_net_model.conv_str_int.find(p) == driver.current_net_model.conv_str_int.end())
                          {
                              driver.current_net_model.conv_str_int[p] = driver.current_net_model.count;
                              driver.current_net_model.conv_int_str[driver.current_net_model.count] = p;
                              driver.current_net_model.count++;
                              //driver.current_pattern.add_trans(driver.current_net_model.conv_str_int[p]);
                          }
                          int val = driver.current_net_model.conv_str_int[p];
                          vector<int> l = driver.current_pattern.get_language();
                          if(!Utils::contain(l,val))
                              driver.current_pattern.add_trans(val);
                          yylhs.value.as< string > ().append("(");
                          yylhs.value.as< string > ().append("~");
                          stringstream ss;
                          ss << driver.current_net_model.conv_str_int[p];
                          yylhs.value.as< string > ().append(ss.str());
                          yylhs.value.as< string > ().append(")");
                      }
                    }
#line 1337 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 370 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { //$$.append($1.first); $$.append("("); $$.append($1.second); $$.append(")");
                    //string str = $1.first; str.append("("); str.append($1.second); str.append(")");
                    pair<string,string> p = make_pair(yystack_[0].value.as< pair<string,string>  > ().first, yystack_[0].value.as< pair<string,string>  > ().second);
                    if(driver.current_net_model.conv_str_int.find(p) == driver.current_net_model.conv_str_int.end())
                    {
                        driver.current_net_model.conv_str_int[p] = driver.current_net_model.count;
                        driver.current_net_model.conv_int_str[driver.current_net_model.count] = p;
                        driver.current_net_model.count++;
                        //driver.current_pattern.add_trans(driver.current_net_model.conv_str_int[p]);
                    }
                    int val = driver.current_net_model.conv_str_int[p];
                    vector<int> l = driver.current_pattern.get_language();
                    if(!Utils::contain(l,val))
                        driver.current_pattern.add_trans(val);
                    yylhs.value.as< string > ().append("(");
                    stringstream ss;
                    ss << driver.current_net_model.conv_str_int[p];
                    yylhs.value.as< string > ().append(ss.str());
                    yylhs.value.as< string > ().append(")");
                  }
#line 1362 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 392 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[1].value.as< vector<pair<string,string> >  > (); }
#line 1368 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 393 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1374 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 396 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1380 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 397 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1386 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 401 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        yylhs.value.as< map<pair<string,string>,string>   > ().insert(yystack_[2].value.as< pair<pair<string,string>,string>  > ());

                        for(map<pair<string,string>,string>::iterator it = yystack_[0].value.as< map<pair<string,string>,string>   > ().begin(); it != yystack_[0].value.as< map<pair<string,string>,string>   > ().end(); it++)
                        {
                            pair<map<pair<string,string>,string>::iterator,bool> res;
                            res = yylhs.value.as< map<pair<string,string>,string>   > ().insert(*it);
                            //verifies existance of the same key in the map
                            if(res.second == false)
                            {
                                string msg = "duplicate network transition map ";
                                msg.append(yystack_[2].value.as< pair<pair<string,string>,string>  > ().first.first); msg.append("("); msg.append(yystack_[2].value.as< pair<pair<string,string>,string>  > ().first.second); msg.append(")");
                                driver.error(loc,msg);
                            }
                        }
                    }
#line 1407 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 418 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > ().insert(yystack_[0].value.as< pair<pair<string,string>,string>  > ()); }
#line 1413 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 421 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< pair<pair<string,string>,string>  > ().first = yystack_[2].value.as< pair<string,string>  > (); yylhs.value.as< pair<pair<string,string>,string>  > ().second = yystack_[0].value.as< string > (); }
#line 1419 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 424 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1425 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 425 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1431 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 432 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        if(driver.system.name != "")
                            driver.error(loc,"Multiple system declaration (system must be unique)");
                        if(yystack_[5].value.as< string > () != yystack_[0].value.as< string > ())
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append(yystack_[0].value.as< string > ());
                            driver.error(loc, msg);
                        }
                        yylhs.value.as< System > ().name = yystack_[5].value.as< string > ();
                        yylhs.value.as< System > ().node_list = yystack_[3].value.as< vector<SystemNode>  > ();
                        yylhs.value.as< System > ().emergence = yystack_[2].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ();

                        driver.semantic_checks(yylhs.value.as< System > ());
                    }
#line 1451 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 449 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<SystemNode>  > ().push_back(yystack_[2].value.as< SystemNode > ()); yylhs.value.as< vector<SystemNode>  > () = Utils::merge(yylhs.value.as< vector<SystemNode>  > (),yystack_[0].value.as< vector<SystemNode>  > ()); }
#line 1457 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 450 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<SystemNode>  > ().push_back(yystack_[1].value.as< SystemNode > ()); }
#line 1463 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 458 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        if(yystack_[8].value.as< string > () != yystack_[0].value.as< string > ())
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append(yystack_[0].value.as< string > ());
                            driver.error(loc, msg);
                        }
                        yylhs.value.as< SystemNode > () = SystemNode(yystack_[8].value.as< string > ());
                        yylhs.value.as< SystemNode > ().net_model = driver.find_netmodel(yystack_[6].value.as< string > ());
                        yylhs.value.as< SystemNode > ().initials = yystack_[4].value.as< vector<pair<string,string> >  > ();
                        yylhs.value.as< SystemNode > ().viewer = yystack_[3].value.as< map<pair<string,string>,string>   > ();
                        yylhs.value.as< SystemNode > ().ruler = yystack_[2].value.as< map<pair<string,string>,string>   > ();
                    }
#line 1481 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 473 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > () = yystack_[1].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > (); }
#line 1487 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 474 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1493 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 480 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        if(driver.problem.name != "")
                            driver.error(loc,"Multiple problem declaration (problem must be unique)");
                        if(yystack_[4].value.as< string > () != yystack_[0].value.as< string > ())
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append(yystack_[0].value.as< string > ());
                            driver.error(loc, msg);
                        }
                        yylhs.value.as< Problem > ().name = yystack_[4].value.as< string > ();
                        yylhs.value.as< Problem > ().nodes = yystack_[2].value.as< vector<ProblemNode>  > ();

                        driver.semantic_checks(yylhs.value.as< Problem > ());
                    }
#line 1512 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 496 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<ProblemNode>  > ().push_back(yystack_[2].value.as< ProblemNode > ()); yylhs.value.as< vector<ProblemNode>  > () = Utils::merge(yylhs.value.as< vector<ProblemNode>  > (),yystack_[0].value.as< vector<ProblemNode>  > ()); }
#line 1518 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 497 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<ProblemNode>  > ().push_back(yystack_[1].value.as< ProblemNode > ()); }
#line 1524 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 506 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        if(yystack_[7].value.as< string > () != yystack_[0].value.as< string > ())
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append(yystack_[0].value.as< string > ());
                            driver.error(loc, msg);
                        }
                        yylhs.value.as< ProblemNode > () = ProblemNode(yystack_[7].value.as< string > ());
                        yylhs.value.as< ProblemNode > ().ref_node = driver.find_node(yystack_[7].value.as< string > ());
                        yylhs.value.as< ProblemNode > ().initials = yystack_[5].value.as< vector<pair<string,string> >  > ();
                        yylhs.value.as< ProblemNode > ().viewer = yystack_[4].value.as< map<pair<string,string>,string>   > ();
                        yylhs.value.as< ProblemNode > ().observation = yystack_[3].value.as< vector<string>  > ();
                        yylhs.value.as< ProblemNode > ().ruler = yystack_[2].value.as< map<pair<string,string>,string>   > ();
                    }
#line 1543 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 523 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[2].value.as< vector<string>  > (); }
#line 1549 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 526 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[0].value.as< vector<string>  > (); }
#line 1555 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 527 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1561 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;


#line 1565 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
            default:
              break;
            }
        }
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, yylhs);
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yyla.location, yysyntax_error_ (yystack_[0].state, yyla));
      }


    yyerror_range[1].location = yyla.location;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    yyerror_range[1].location = yystack_[yylen - 1].location;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yyerror_range[1].location = yystack_[0].location;
          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }

      yyerror_range[2].location = yyla.location;
      YYLLOC_DEFAULT (error_token.location, yyerror_range, 2);

      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", error_token);
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack"
                 << std::endl;
        // Do not try to display the values of the reclaimed symbols,
        // as their printer might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  void
  spec_parser::error (const syntax_error& yyexc)
  {
    error (yyexc.location, yyexc.what());
  }

  // Generate an error message.
  std::string
  spec_parser::yysyntax_error_ (state_type yystate, const symbol_type& yyla) const
  {
    // Number of reported tokens (one for the "unexpected", one per
    // "expected").
    size_t yycount = 0;
    // Its maximum.
    enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
    // Arguments of yyformat.
    char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];

    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state
         merging (from LALR or IELR) and default reductions corrupt the
         expected token list.  However, the list is correct for
         canonical LR with one exception: it will still contain any
         token that will not be accepted due to an error action in a
         later state.
    */
    if (!yyla.empty ())
      {
        int yytoken = yyla.type_get ();
        yyarg[yycount++] = yytname_[yytoken];
        int yyn = yypact_[yystate];
        if (!yy_pact_value_is_default_ (yyn))
          {
            /* Start YYX at -YYN if negative to avoid negative indexes in
               YYCHECK.  In other words, skip the first -YYN actions for
               this state because they are default actions.  */
            int yyxbegin = yyn < 0 ? -yyn : 0;
            // Stay within bounds of both yycheck and yytname.
            int yychecklim = yylast_ - yyn + 1;
            int yyxend = yychecklim < yyntokens_ ? yychecklim : yyntokens_;
            for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
              if (yycheck_[yyx + yyn] == yyx && yyx != yyterror_
                  && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
                {
                  if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                    {
                      yycount = 1;
                      break;
                    }
                  else
                    yyarg[yycount++] = yytname_[yyx];
                }
          }
      }

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
        YYCASE_(0, YY_("syntax error"));
        YYCASE_(1, YY_("syntax error, unexpected %s"));
        YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    size_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += yytnamerr_ (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const short int spec_parser::yypact_ninf_ = -155;

  const signed char spec_parser::yytable_ninf_ = -1;

  const short int
  spec_parser::yypact_[] =
  {
       5,     7,    21,   -14,    -2,    29,  -155,    17,  -155,  -155,
    -155,  -155,  -155,     9,    13,    45,    51,  -155,     5,    53,
      54,    41,    42,  -155,    55,    61,    23,    46,    -5,    26,
      63,     8,    28,    64,    28,  -155,    47,    31,    68,    41,
    -155,    70,    34,    42,  -155,    56,    52,    28,    69,    57,
      58,    59,    64,    43,    62,    60,    66,    71,    65,  -155,
      72,  -155,  -155,    28,  -155,    73,    28,    85,    67,  -155,
      28,    69,    91,    74,    31,  -155,    31,  -155,    31,    81,
    -155,  -155,    75,    28,    88,  -155,  -155,    89,    72,    77,
    -155,  -155,    78,    79,    31,    83,  -155,    86,    76,    97,
      31,    92,    81,  -155,    31,  -155,    84,    87,    94,    90,
      93,  -155,    98,    95,    96,    82,   101,    31,    72,    93,
    -155,    99,  -155,    31,    28,    31,   109,   -18,  -155,    76,
    -155,  -155,   -13,   102,   105,    81,   115,  -155,  -155,  -155,
     100,   104,   103,   106,   107,  -155,  -155,  -155,  -155,   -20,
    -155,    31,    93,   108,   110,  -155,  -155,  -155,   111,   -20,
      31,  -155,   114,    12,     6,  -155,   125,  -155,  -155,   113,
     -24,  -155,   -20,   -20,     6,  -155,  -155,  -155,   112,   116,
    -155,    12,     6,  -155,   117,   118,    31,   120,  -155,  -155
  };

  const unsigned char
  spec_parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     2,     0,     5,     8,
       9,     6,     7,     0,     0,     0,     0,     1,     4,     0,
       0,     0,     0,     3,     0,     0,     0,    73,     0,     0,
       0,     0,     0,    15,     0,    30,     0,     0,     0,     0,
      70,     0,     0,     0,    76,    13,     0,     0,    17,     0,
       0,    34,    15,     0,     0,     0,     0,    39,     0,    69,
      60,    74,    75,     0,    11,     0,     0,     0,     0,    32,
       0,    17,     0,     0,     0,    72,     0,    68,     0,    62,
      12,    14,     0,     0,     0,    35,    33,    37,    60,     0,
      40,    38,    29,     0,     0,     0,    16,     0,     0,     0,
       0,    42,    62,    25,     0,    59,     0,     0,    64,     0,
      67,    18,     0,     0,    21,     0,     0,     0,    60,    67,
      28,     0,    61,     0,    80,     0,     0,     0,    19,     0,
      10,    36,     0,     0,    44,    62,     0,    65,    63,    79,
       0,     0,     0,     0,     0,    23,    20,    46,    47,     0,
      41,     0,    67,     0,     0,    66,    77,    24,     0,     0,
       0,    58,    45,    49,    52,    43,     0,    71,    78,     0,
       0,    57,     0,     0,    50,    53,    54,    55,     0,     0,
      56,    48,    51,    31,     0,     0,    27,     0,    26,    22
  };

  const short int
  spec_parser::yypgoto_[] =
  {
    -155,  -155,   121,  -155,  -155,  -155,  -155,   -31,    80,   119,
    -155,  -155,    14,  -155,  -155,   -37,  -155,  -102,  -155,  -155,
    -155,   122,  -155,  -155,   -70,  -155,  -155,   -17,  -155,  -155,
     -23,   -34,  -154,   -84,   -99,   -76,  -155,  -114,  -155,   123,
    -155,  -155,  -155,   124,  -155,  -155,  -155
  };

  const short int
  spec_parser::yydefgoto_[] =
  {
      -1,     5,     6,     7,     8,     9,    33,    49,    48,    67,
      84,    99,   113,   114,   144,   161,   187,    93,    10,    52,
      35,    50,    51,   101,    56,    57,   118,   133,   134,   149,
     162,   163,   164,    79,    95,   107,   108,   126,    11,    27,
      28,    38,    12,    30,    31,   110,   140
  };

  const unsigned char
  spec_parser::yytable_[] =
  {
      55,    46,   120,   119,   102,   136,    91,   180,     1,   174,
     159,    13,   143,   172,   147,   148,    65,     2,    39,   182,
      40,   160,    54,     3,    54,    14,     4,   174,    15,    17,
     116,    43,    80,    44,   135,    82,   152,    90,   166,    55,
      16,    92,   159,    18,   175,   176,   177,   138,   173,   141,
      21,    19,    97,   160,    54,    20,    22,   106,    24,    25,
      26,    29,    32,    55,    34,    36,    37,    92,    41,    42,
      45,    53,    47,    54,    58,    60,    61,    64,    66,    63,
     132,    68,    70,    69,   188,    72,   106,    78,   106,    74,
     145,    75,    73,   139,    76,    83,    88,    94,    81,    98,
      96,   104,   100,   115,   105,   109,   117,    77,   103,    85,
     125,   111,   122,   121,   132,   142,    89,   123,   112,   129,
     128,   153,   124,   171,   130,   127,   131,   150,   151,   155,
     158,   178,    71,   154,   165,   168,   170,   157,   181,    23,
     185,   137,   179,   146,     0,   156,     0,     0,     0,    92,
     167,   172,   186,   169,   183,   189,     0,     0,   184,     0,
       0,     0,    59,     0,     0,     0,     0,    62,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      87,     0,    86
  };

  const short int
  spec_parser::yycheck_[] =
  {
      37,    32,   104,   102,    88,   119,    76,    31,     3,   163,
      30,     4,    30,    37,    27,    28,    47,    12,    23,   173,
      25,    41,    42,    18,    42,     4,    21,   181,    42,     0,
     100,    23,    63,    25,   118,    66,   135,    74,   152,    76,
      42,    78,    30,    26,    38,    39,    40,   123,    36,   125,
       5,    42,    83,    41,    42,    42,     5,    94,     5,     5,
      19,    19,     7,   100,     3,    42,    20,   104,    42,     6,
      42,    24,     8,    42,     6,     5,    42,    25,     9,    23,
     117,    24,    23,    25,   186,    42,   123,    15,   125,    29,
     127,    25,    30,   124,    23,    10,     5,    16,    25,    11,
      25,    23,    13,     6,    25,    22,    14,    42,    31,    42,
      17,    25,    25,    29,   151,     6,    42,    23,    42,    23,
      25,     6,    32,   160,    42,    27,    25,    25,    23,    25,
      23,     6,    52,    33,   151,    25,   159,    31,   172,    18,
      23,    42,    29,   129,    -1,    42,    -1,    -1,    -1,   186,
      42,    37,    34,    42,    42,    35,    -1,    -1,    42,    -1,
      -1,    -1,    39,    -1,    -1,    -1,    -1,    43,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      71,    -1,    70
  };

  const unsigned char
  spec_parser::yystos_[] =
  {
       0,     3,    12,    18,    21,    44,    45,    46,    47,    48,
      61,    81,    85,     4,     4,    42,    42,     0,    26,    42,
      42,     5,     5,    45,     5,     5,    19,    82,    83,    19,
      86,    87,     7,    49,     3,    63,    42,    20,    84,    23,
      25,    42,     6,    23,    25,    42,    50,     8,    51,    50,
      64,    65,    62,    24,    42,    58,    67,    68,     6,    82,
       5,    42,    86,    23,    25,    50,     9,    52,    24,    25,
      23,    51,    42,    30,    29,    25,    23,    42,    15,    76,
      50,    25,    50,    10,    53,    42,    64,    52,     5,    42,
      58,    67,    58,    60,    16,    77,    25,    50,    11,    54,
      13,    66,    76,    31,    23,    25,    58,    78,    79,    22,
      88,    25,    42,    55,    56,     6,    67,    14,    69,    77,
      60,    29,    25,    23,    32,    17,    80,    27,    25,    23,
      42,    25,    58,    70,    71,    76,    80,    42,    78,    50,
      89,    78,     6,    30,    57,    58,    55,    27,    28,    72,
      25,    23,    77,     6,    33,    25,    42,    31,    23,    30,
      41,    58,    73,    74,    75,    70,    80,    42,    25,    42,
      73,    58,    37,    36,    75,    38,    39,    40,     6,    29,
      31,    74,    75,    42,    42,    23,    34,    59,    60,    35
  };

  const unsigned char
  spec_parser::yyr1_[] =
  {
       0,    43,    44,    45,    45,    46,    46,    46,    47,    47,
      48,    49,    50,    50,    51,    51,    52,    52,    53,    54,
      55,    55,    56,    57,    57,    58,    59,    59,    60,    60,
      62,    61,    63,    64,    64,    65,    66,    66,    67,    67,
      68,    69,    69,    70,    70,    71,    72,    72,    73,    73,
      74,    74,    74,    75,    75,    75,    75,    75,    75,    76,
      76,    77,    77,    78,    78,    79,    80,    80,    81,    82,
      82,    83,    84,    84,    85,    86,    86,    87,    88,    89,
      89
  };

  const unsigned char
  spec_parser::yyr2_[] =
  {
       0,     2,     1,     3,     2,     1,     1,     1,     1,     1,
      11,     3,     3,     1,     3,     0,     3,     0,     3,     3,
       3,     1,    11,     1,     2,     4,     1,     0,     3,     1,
       0,    15,     3,     3,     1,     3,     3,     0,     3,     1,
       3,     3,     0,     3,     1,     3,     1,     1,     3,     1,
       2,     3,     1,     2,     2,     2,     3,     2,     1,     3,
       0,     3,     0,     3,     1,     3,     3,     0,     7,     3,
       2,    10,     3,     0,     6,     3,     2,     9,     5,     1,
       0
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const spec_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "COMPONENT", "MODEL", "IS",
  "END", "EVENT", "INPUT", "OUTPUT", "STATE", "TRANSITION", "NETWORK",
  "LINK", "PATTERN", "INITIAL", "VIEWER", "RULER", "SYSTEM", "NODE",
  "EMERGENCE", "PROBLEM", "OBS", "\",\"", "\":\"", "\";\"", "\".\"",
  "\"=\"", "\"==\"", "\"->\"", "\"(\"", "\")\"", "\"[\"", "\"]\"", "\"{\"",
  "\"}\"", "\"&\"", "\"|\"", "\"*\"", "\"+\"", "\"?\"", "\"~\"", "\"id\"",
  "$accept", "specification", "spec_list", "spec", "model_decl",
  "comp_model_decl", "event_decl", "id_list", "input_decl", "output_decl",
  "state_decl", "transition_decl", "trans_decl_list", "trans_decl",
  "event", "ref", "opt_ref_list", "ref_list", "net_model_decl", "$@1",
  "component_section", "decl_list", "decl", "link_section", "link_list",
  "link_decl", "pattern_section", "pattern_list", "pattern_decl",
  "pattern_op", "expr", "term", "factor", "initial_section",
  "viewer_section", "map_list", "map_decl", "ruler_section", "system_decl",
  "system_node_list", "system_node", "emergence_section", "problem_decl",
  "problem_node_list", "problem_node", "obs_section", "opt_id_list", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  spec_parser::yyrline_[] =
  {
       0,   137,   137,   140,   141,   144,   145,   146,   149,   150,
     163,   194,   197,   199,   202,   203,   206,   207,   210,   213,
     216,   217,   220,   228,   229,   232,   235,   236,   239,   240,
     245,   243,   284,   287,   288,   291,   294,   295,   298,   299,
     302,   305,   306,   309,   310,   313,   326,   327,   330,   331,
     334,   335,   336,   339,   340,   341,   342,   343,   369,   392,
     393,   396,   397,   400,   417,   421,   424,   425,   428,   449,
     450,   453,   473,   474,   477,   496,   497,   500,   522,   526,
     527
  };

  // Print the state stack on the debug stream.
  void
  spec_parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  spec_parser::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):" << std::endl;
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 2066 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:1167
#line 531 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:1168


void yy::spec_parser::error (const location_type& l, const std::string& m)
{
    driver.error (l, m);
}


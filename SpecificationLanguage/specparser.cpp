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
      case 49: // comp_model_decl
        value.move< ComponentModel > (that.value);
        break;

      case 62: // net_model_decl
        value.move< NetworkModel > (that.value);
        break;

      case 87: // problem_decl
        value.move< Problem > (that.value);
        break;

      case 89: // problem_node
        value.move< ProblemNode > (that.value);
        break;

      case 82: // system_decl
        value.move< System > (that.value);
        break;

      case 84: // system_node
        value.move< SystemNode > (that.value);
        break;

      case 57: // trans_decl
        value.move< Transition > (that.value);
        break;

      case 78: // viewer_section
      case 79: // map_list
      case 81: // ruler_section
      case 86: // emergence_section
        value.move< map<pair<string,string>,string>   > (that.value);
        break;

      case 69: // link_decl
        value.move< pair<pair<string,string>,pair<string,string> >  > (that.value);
        break;

      case 80: // map_decl
        value.move< pair<pair<string,string>,string>  > (that.value);
        break;

      case 58: // event
      case 59: // ref
      case 72: // pattern_decl
        value.move< pair<string,string>  > (that.value);
        break;

      case 43: // "id"
      case 74: // expr
      case 75: // term
      case 76: // factor
      case 85: // root_section
        value.move< string > (that.value);
        break;

      case 64: // component_section
      case 65: // decl_list
      case 66: // decl
        value.move< vector<Component>  > (that.value);
        break;

      case 88: // problem_node_list
        value.move< vector<ProblemNode>  > (that.value);
        break;

      case 83: // system_node_list
        value.move< vector<SystemNode>  > (that.value);
        break;

      case 55: // transition_decl
      case 56: // trans_decl_list
        value.move< vector<Transition>  > (that.value);
        break;

      case 67: // link_section
      case 68: // link_list
        value.move< vector<pair<pair<string,string>,pair<string,string> > >  > (that.value);
        break;

      case 60: // opt_ref_list
      case 61: // ref_list
      case 70: // pattern_section
      case 71: // pattern_list
      case 77: // initial_section
        value.move< vector<pair<string,string> >  > (that.value);
        break;

      case 50: // event_decl
      case 51: // id_list
      case 52: // input_decl
      case 53: // output_decl
      case 54: // state_decl
      case 90: // obs_section
      case 91: // opt_id_list
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
      case 49: // comp_model_decl
        value.copy< ComponentModel > (that.value);
        break;

      case 62: // net_model_decl
        value.copy< NetworkModel > (that.value);
        break;

      case 87: // problem_decl
        value.copy< Problem > (that.value);
        break;

      case 89: // problem_node
        value.copy< ProblemNode > (that.value);
        break;

      case 82: // system_decl
        value.copy< System > (that.value);
        break;

      case 84: // system_node
        value.copy< SystemNode > (that.value);
        break;

      case 57: // trans_decl
        value.copy< Transition > (that.value);
        break;

      case 78: // viewer_section
      case 79: // map_list
      case 81: // ruler_section
      case 86: // emergence_section
        value.copy< map<pair<string,string>,string>   > (that.value);
        break;

      case 69: // link_decl
        value.copy< pair<pair<string,string>,pair<string,string> >  > (that.value);
        break;

      case 80: // map_decl
        value.copy< pair<pair<string,string>,string>  > (that.value);
        break;

      case 58: // event
      case 59: // ref
      case 72: // pattern_decl
        value.copy< pair<string,string>  > (that.value);
        break;

      case 43: // "id"
      case 74: // expr
      case 75: // term
      case 76: // factor
      case 85: // root_section
        value.copy< string > (that.value);
        break;

      case 64: // component_section
      case 65: // decl_list
      case 66: // decl
        value.copy< vector<Component>  > (that.value);
        break;

      case 88: // problem_node_list
        value.copy< vector<ProblemNode>  > (that.value);
        break;

      case 83: // system_node_list
        value.copy< vector<SystemNode>  > (that.value);
        break;

      case 55: // transition_decl
      case 56: // trans_decl_list
        value.copy< vector<Transition>  > (that.value);
        break;

      case 67: // link_section
      case 68: // link_list
        value.copy< vector<pair<pair<string,string>,pair<string,string> > >  > (that.value);
        break;

      case 60: // opt_ref_list
      case 61: // ref_list
      case 70: // pattern_section
      case 71: // pattern_list
      case 77: // initial_section
        value.copy< vector<pair<string,string> >  > (that.value);
        break;

      case 50: // event_decl
      case 51: // id_list
      case 52: // input_decl
      case 53: // output_decl
      case 54: // state_decl
      case 90: // obs_section
      case 91: // opt_id_list
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
            case 43: // "id"

#line 129 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 516 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 49: // comp_model_decl

#line 130 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ComponentModel > (); }
#line 523 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 57: // trans_decl

#line 131 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Transition > (); }
#line 530 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 62: // net_model_decl

#line 132 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< NetworkModel > (); }
#line 537 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 74: // expr

#line 129 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 544 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 75: // term

#line 129 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 551 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 76: // factor

#line 129 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 558 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 85: // root_section

#line 129 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 565 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
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

#line 684 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:741

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
      case 49: // comp_model_decl
        yylhs.value.build< ComponentModel > ();
        break;

      case 62: // net_model_decl
        yylhs.value.build< NetworkModel > ();
        break;

      case 87: // problem_decl
        yylhs.value.build< Problem > ();
        break;

      case 89: // problem_node
        yylhs.value.build< ProblemNode > ();
        break;

      case 82: // system_decl
        yylhs.value.build< System > ();
        break;

      case 84: // system_node
        yylhs.value.build< SystemNode > ();
        break;

      case 57: // trans_decl
        yylhs.value.build< Transition > ();
        break;

      case 78: // viewer_section
      case 79: // map_list
      case 81: // ruler_section
      case 86: // emergence_section
        yylhs.value.build< map<pair<string,string>,string>   > ();
        break;

      case 69: // link_decl
        yylhs.value.build< pair<pair<string,string>,pair<string,string> >  > ();
        break;

      case 80: // map_decl
        yylhs.value.build< pair<pair<string,string>,string>  > ();
        break;

      case 58: // event
      case 59: // ref
      case 72: // pattern_decl
        yylhs.value.build< pair<string,string>  > ();
        break;

      case 43: // "id"
      case 74: // expr
      case 75: // term
      case 76: // factor
      case 85: // root_section
        yylhs.value.build< string > ();
        break;

      case 64: // component_section
      case 65: // decl_list
      case 66: // decl
        yylhs.value.build< vector<Component>  > ();
        break;

      case 88: // problem_node_list
        yylhs.value.build< vector<ProblemNode>  > ();
        break;

      case 83: // system_node_list
        yylhs.value.build< vector<SystemNode>  > ();
        break;

      case 55: // transition_decl
      case 56: // trans_decl_list
        yylhs.value.build< vector<Transition>  > ();
        break;

      case 67: // link_section
      case 68: // link_list
        yylhs.value.build< vector<pair<pair<string,string>,pair<string,string> > >  > ();
        break;

      case 60: // opt_ref_list
      case 61: // ref_list
      case 70: // pattern_section
      case 71: // pattern_list
      case 77: // initial_section
        yylhs.value.build< vector<pair<string,string> >  > ();
        break;

      case 50: // event_decl
      case 51: // id_list
      case 52: // input_decl
      case 53: // output_decl
      case 54: // state_decl
      case 90: // obs_section
      case 91: // opt_id_list
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
#line 138 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  }
#line 893 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 141 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  }
#line 899 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 142 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 905 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 145 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 911 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 146 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.system = System(yystack_[0].value.as< System > ()); }
#line 917 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 147 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.problem = Problem(yystack_[0].value.as< Problem > ()); }
#line 923 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 150 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { ComponentModel(yystack_[0].value.as< ComponentModel > ()); driver.components.push_back(yystack_[0].value.as< ComponentModel > ()); }
#line 929 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 152 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 944 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 171 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 971 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 195 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 977 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 199 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > ().push_back(yystack_[2].value.as< string > ()); yylhs.value.as< vector<string>  > () = Utils::merge(yylhs.value.as< vector<string>  > (),yystack_[0].value.as< vector<string>  > ()); }
#line 983 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 200 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > ().push_back(yystack_[0].value.as< string > ()); }
#line 989 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 203 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 995 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 204 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1001 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 207 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 1007 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 208 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1013 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 211 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 1019 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 214 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > () = yystack_[1].value.as< vector<Transition>  > (); }
#line 1025 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 217 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > ().push_back(yystack_[2].value.as< Transition > ()); yylhs.value.as< vector<Transition>  > () = Utils::merge(yylhs.value.as< vector<Transition>  > (),yystack_[0].value.as< vector<Transition>  > ()); }
#line 1031 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 218 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > ().push_back(yystack_[0].value.as< Transition > ()); }
#line 1037 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 222 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                    yylhs.value.as< Transition > ().name = yystack_[10].value.as< string > (); yylhs.value.as< Transition > ().input_event = yystack_[8].value.as< pair<string,string>  > ();
                    string s1 = yystack_[6].value.as< string > (); string s2 = yystack_[4].value.as< string > (); yylhs.value.as< Transition > ().s1_s2 = make_pair(s1,s2);
                    yylhs.value.as< Transition > ().out_events = yystack_[1].value.as< vector<pair<string,string> >  > ();
                  }
#line 1047 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 229 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< pair<string,string>  > () = yystack_[0].value.as< pair<string,string>  > (); }
#line 1053 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 230 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {}
#line 1059 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 233 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string s1 = yystack_[3].value.as< string > (); string s2 = yystack_[1].value.as< string > (); yylhs.value.as< pair<string,string>  > () = make_pair(s1,s2); }
#line 1065 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 236 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[0].value.as< vector<pair<string,string> >  > (); }
#line 1071 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 237 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {}
#line 1077 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 240 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[2].value.as< pair<string,string>  > ()); yylhs.value.as< vector<pair<string,string> >  > () = Utils::merge(yylhs.value.as< vector<pair<string,string> >  > (),yystack_[0].value.as< vector<pair<string,string> >  > ()); }
#line 1083 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 241 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[0].value.as< pair<string,string>  > ()); }
#line 1089 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 246 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        driver.current_net_model = NetworkModel();
                        driver.current_net_model.components = yystack_[0].value.as< vector<Component>  > ();
                    }
#line 1098 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 258 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
                    yylhs.value.as< NetworkModel > ().patterns = yystack_[5].value.as< vector<pair<string,string> >  > ();
                    yylhs.value.as< NetworkModel > ().initials = yystack_[4].value.as< vector<pair<string,string> >  > ();
                    yylhs.value.as< NetworkModel > ().viewer = yystack_[3].value.as< map<pair<string,string>,string>   > ();
                    yylhs.value.as< NetworkModel > ().ruler = yystack_[2].value.as< map<pair<string,string>,string>   > ();

                    driver.semantic_checks(yylhs.value.as< NetworkModel > ());
                  }
#line 1128 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 285 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = yystack_[1].value.as< vector<Component>  > (); }
#line 1134 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 288 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[2].value.as< vector<Component>  > ()); yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[0].value.as< vector<Component>  > ()); }
#line 1140 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 289 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[0].value.as< vector<Component>  > ()); }
#line 1146 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 292 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = driver.build_components(yystack_[2].value.as< vector<string>  > (),yystack_[0].value.as< string > ());  }
#line 1152 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 295 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > () = yystack_[1].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > (); }
#line 1158 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 296 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1164 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 299 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ().push_back(yystack_[2].value.as< pair<pair<string,string>,pair<string,string> >  > ()); yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > () = Utils::merge(yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > (),yystack_[0].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ()); }
#line 1170 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 300 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ().push_back(yystack_[0].value.as< pair<pair<string,string>,pair<string,string> >  > ()); }
#line 1176 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 303 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  yylhs.value.as< pair<pair<string,string>,pair<string,string> >  > () = make_pair(yystack_[2].value.as< pair<string,string>  > (),yystack_[0].value.as< pair<string,string>  > ()); }
#line 1182 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 306 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[1].value.as< vector<pair<string,string> >  > (); }
#line 1188 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 307 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1194 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 310 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[2].value.as< pair<string,string>  > ()); yylhs.value.as< vector<pair<string,string> >  > () = Utils::merge(yylhs.value.as< vector<pair<string,string> >  > (),yystack_[0].value.as< vector<pair<string,string> >  > ()); }
#line 1200 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 311 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[0].value.as< pair<string,string>  > ()); }
#line 1206 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 314 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string s1 = yystack_[2].value.as< string > (); string s2 = yystack_[0].value.as< string > (); yylhs.value.as< pair<string,string>  > () = make_pair(s1,s2); }
#line 1212 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 321 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1218 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 322 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1224 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 325 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1230 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 326 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append(")"); }
#line 1236 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 327 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1242 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 330 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("*"); }
#line 1248 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 331 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("*");}
#line 1254 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 332 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append("\"\""); yylhs.value.as< string > ().append(")"); }
#line 1260 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 333 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(")"); }
#line 1266 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 335 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string str = yystack_[0].value.as< pair<string,string>  > ().first; str.append("("); str.append(yystack_[0].value.as< pair<string,string>  > ().second); str.append(")");
                      yylhs.value.as< string > ().append(driver.current_net_model.not_trans(str));
                    }
#line 1274 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 339 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { //$$.append($1.first); $$.append("("); $$.append($1.second); $$.append(")");
                    //string str = $1.first; str.append("("); str.append($1.second); str.append(")");
                    pair<string,string> p = make_pair(yystack_[0].value.as< pair<string,string>  > ().first, yystack_[0].value.as< pair<string,string>  > ().second);
                    if(driver.current_net_model.conv_str_int.find(p) == driver.current_net_model.conv_str_int.end())
                    {
                        driver.current_net_model.conv_str_int[p] = driver.current_net_model.count;
                        driver.current_net_model.conv_int_str[driver.current_net_model.count] = p;
                        driver.current_net_model.count++;
                    }
                    yylhs.value.as< string > ().append("(");
                    stringstream ss;
                    ss << driver.current_net_model.conv_str_int[p];
                    yylhs.value.as< string > ().append(ss.str());
                    yylhs.value.as< string > ().append(")");
                  }
#line 1294 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 356 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[1].value.as< vector<pair<string,string> >  > (); }
#line 1300 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 357 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1306 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 360 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1312 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 361 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1318 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 365 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1339 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 382 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > ().insert(yystack_[0].value.as< pair<pair<string,string>,string>  > ()); }
#line 1345 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 385 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< pair<pair<string,string>,string>  > ().first = yystack_[2].value.as< pair<string,string>  > (); yylhs.value.as< pair<pair<string,string>,string>  > ().second = yystack_[0].value.as< string > (); }
#line 1351 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 388 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1357 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 389 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1363 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 397 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        if(driver.system.name != "")
                            driver.error(loc,"Multiple system declaration (system must be unique)");
                        if(yystack_[6].value.as< string > () != yystack_[0].value.as< string > ())
                        {
                            string msg = "Semantic error: wrong end ID ";
                            msg.append(yystack_[0].value.as< string > ());
                            driver.error(loc, msg);
                        }
                        yylhs.value.as< System > ().name = yystack_[6].value.as< string > ();
                        yylhs.value.as< System > ().node_list = yystack_[4].value.as< vector<SystemNode>  > ();
                        yylhs.value.as< System > ().id_root = yystack_[3].value.as< string > ();
                        yylhs.value.as< System > ().emergence = yystack_[2].value.as< map<pair<string,string>,string>   > ();

                        driver.semantic_checks(yylhs.value.as< System > ());
                    }
#line 1384 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 415 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<SystemNode>  > ().push_back(yystack_[2].value.as< SystemNode > ()); yylhs.value.as< vector<SystemNode>  > () = Utils::merge(yylhs.value.as< vector<SystemNode>  > (),yystack_[0].value.as< vector<SystemNode>  > ()); }
#line 1390 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 416 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<SystemNode>  > ().push_back(yystack_[1].value.as< SystemNode > ()); }
#line 1396 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 424 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1414 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 439 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > () = yystack_[1].value.as< string > (); }
#line 1420 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 440 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1426 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 443 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1432 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 444 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1438 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 450 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1457 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 466 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<ProblemNode>  > ().push_back(yystack_[2].value.as< ProblemNode > ()); yylhs.value.as< vector<ProblemNode>  > () = Utils::merge(yylhs.value.as< vector<ProblemNode>  > (),yystack_[0].value.as< vector<ProblemNode>  > ()); }
#line 1463 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 467 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<ProblemNode>  > ().push_back(yystack_[1].value.as< ProblemNode > ()); }
#line 1469 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 476 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1488 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 493 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[2].value.as< vector<string>  > (); }
#line 1494 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 81:
#line 496 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[0].value.as< vector<string>  > (); }
#line 1500 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 82:
#line 497 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1506 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;


#line 1510 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
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


  const short int spec_parser::yypact_ninf_ = -159;

  const signed char spec_parser::yytable_ninf_ = -1;

  const short int
  spec_parser::yypact_[] =
  {
       3,     6,    16,    -8,     7,    49,  -159,    26,  -159,  -159,
    -159,  -159,  -159,    11,    12,    51,    52,  -159,     3,    53,
      54,    41,    42,  -159,    55,    60,    21,    45,   -13,    23,
      61,     8,    25,    62,    25,  -159,    44,    28,    56,    41,
    -159,    68,    32,    42,  -159,    57,    50,    25,    70,    58,
      63,    64,    62,    39,    65,    43,    78,  -159,    72,  -159,
    -159,    25,  -159,    66,    25,    80,    59,  -159,    25,    70,
      88,  -159,    67,    69,    71,    76,    73,    43,    85,  -159,
    -159,    77,    25,    83,  -159,  -159,    91,    72,    74,    75,
    -159,    43,  -159,    81,    82,    43,    84,  -159,    86,    79,
      90,    43,    92,    85,    87,  -159,  -159,    43,  -159,    89,
      93,    94,  -159,    95,    98,    96,    97,    99,   101,   104,
     100,    72,    94,  -159,  -159,  -159,    25,    43,   107,   -17,
    -159,    79,  -159,    43,  -159,    43,    19,   105,   106,    85,
     108,  -159,   102,   111,   103,   109,   110,  -159,  -159,  -159,
    -159,  -159,  -159,   -14,  -159,   100,    94,   112,   113,  -159,
    -159,  -159,   114,   -14,    43,  -159,   115,   -19,     4,  -159,
     119,  -159,  -159,   117,    -1,  -159,   -14,   -14,     4,  -159,
    -159,  -159,   116,   118,  -159,   -19,     4,  -159,   120,   121,
      43,   122,  -159,  -159
  };

  const unsigned char
  spec_parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     2,     0,     5,     8,
       9,     6,     7,     0,     0,     0,     0,     1,     4,     0,
       0,     0,     0,     3,     0,     0,     0,    73,     0,     0,
       0,     0,     0,    15,     0,    30,     0,     0,    75,     0,
      70,     0,     0,     0,    78,    13,     0,     0,    17,     0,
       0,    34,    15,     0,     0,     0,     0,    69,    60,    76,
      77,     0,    11,     0,     0,     0,     0,    32,     0,    17,
       0,    72,     0,     0,     0,    64,     0,     0,    62,    12,
      14,     0,     0,     0,    35,    33,    37,    60,     0,     0,
      74,     0,    68,    29,     0,     0,     0,    16,     0,     0,
       0,     0,    42,    62,     0,    65,    63,     0,    59,     0,
       0,    67,    18,     0,     0,    21,     0,     0,     0,    39,
       0,    60,    67,    25,    28,    61,    82,     0,     0,     0,
      19,     0,    10,     0,    36,     0,     0,     0,    44,    62,
       0,    81,     0,     0,     0,     0,     0,    23,    20,    40,
      38,    46,    47,     0,    41,     0,    67,     0,     0,    66,
      79,    24,     0,     0,     0,    58,    45,    49,    52,    43,
       0,    71,    80,     0,     0,    57,     0,     0,    50,    53,
      54,    55,     0,     0,    56,    48,    51,    31,     0,     0,
      27,     0,    26,    22
  };

  const short int
  spec_parser::yypgoto_[] =
  {
    -159,  -159,   124,  -159,  -159,  -159,  -159,   -31,   123,   125,
    -159,  -159,   -21,  -159,  -159,   -55,  -159,  -105,  -159,  -159,
    -159,   127,  -159,  -159,    -3,  -159,  -159,   -34,  -159,  -159,
     -30,   -38,  -158,   -83,   -98,   -88,  -159,  -114,  -159,   126,
    -159,  -159,  -159,  -159,   128,  -159,  -159,  -159
  };

  const short int
  spec_parser::yydefgoto_[] =
  {
      -1,     5,     6,     7,     8,     9,    33,    49,    48,    65,
      83,   100,   114,   115,   146,   165,   191,    94,    10,    52,
      35,    50,    51,   102,   118,   119,   121,   137,   138,   153,
     166,   167,   168,    78,    96,    74,    75,   128,    11,    27,
      28,    38,    56,    12,    30,    31,   111,   142
  };

  const unsigned char
  spec_parser::yytable_[] =
  {
      73,    46,   124,   106,   103,   122,     1,   109,   140,   178,
      13,    39,   163,    40,   145,     2,    63,   163,   177,   186,
      14,     3,    93,   164,    72,     4,    72,   178,   164,    72,
      79,   184,    43,    81,    44,    15,    73,   176,   139,   143,
      73,   156,   170,   179,   180,   181,   117,   151,   152,    17,
      16,    98,    93,    18,    19,    20,    21,    22,    24,    25,
      26,    29,    32,    34,    36,    37,    41,    42,    45,    53,
      47,    54,    73,    58,   147,    59,    62,    55,   149,    64,
     117,    61,    70,    66,    76,   192,    72,    77,    68,    67,
      82,    71,    80,    87,    99,   141,   116,    90,    88,    89,
      91,    95,    84,    97,   101,   107,   120,   110,   108,   175,
     148,   127,   112,   144,   157,   125,    92,   104,   105,   123,
     131,   169,   113,   129,   130,   182,   126,   134,   135,   133,
     155,   154,   150,   174,   162,    93,   158,   159,   185,   172,
     132,   161,    23,   136,   189,     0,   160,   183,     0,     0,
       0,     0,     0,   176,     0,   171,   190,   173,   193,   187,
       0,   188,     0,     0,     0,    57,     0,     0,     0,     0,
       0,    60,     0,     0,     0,    69,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    86,    85
  };

  const short int
  spec_parser::yycheck_[] =
  {
      55,    32,   107,    91,    87,   103,     3,    95,   122,   167,
       4,    24,    31,    26,    31,    12,    47,    31,    37,   177,
       4,    18,    77,    42,    43,    22,    43,   185,    42,    43,
      61,    32,    24,    64,    26,    43,    91,    38,   121,   127,
      95,   139,   156,    39,    40,    41,   101,    28,    29,     0,
      43,    82,   107,    27,    43,    43,     5,     5,     5,     5,
      19,    19,     7,     3,    43,    20,    43,     6,    43,    25,
       8,    43,   127,     5,   129,    43,    26,    21,   133,     9,
     135,    24,    43,    25,     6,   190,    43,    15,    24,    26,
      10,    26,    26,     5,    11,   126,     6,    26,    31,    30,
      24,    16,    43,    26,    13,    24,    14,    23,    26,   164,
     131,    17,    26,     6,     6,    26,    43,    43,    43,    32,
      24,   155,    43,    28,    26,     6,    33,    26,    24,    30,
      24,    26,   135,   163,    24,   190,    34,    26,   176,    26,
      43,    32,    18,    43,    24,    -1,    43,    30,    -1,    -1,
      -1,    -1,    -1,    38,    -1,    43,    35,    43,    36,    43,
      -1,    43,    -1,    -1,    -1,    39,    -1,    -1,    -1,    -1,
      -1,    43,    -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    69,    68
  };

  const unsigned char
  spec_parser::yystos_[] =
  {
       0,     3,    12,    18,    22,    45,    46,    47,    48,    49,
      62,    82,    87,     4,     4,    43,    43,     0,    27,    43,
      43,     5,     5,    46,     5,     5,    19,    83,    84,    19,
      88,    89,     7,    50,     3,    64,    43,    20,    85,    24,
      26,    43,     6,    24,    26,    43,    51,     8,    52,    51,
      65,    66,    63,    25,    43,    21,    86,    83,     5,    43,
      88,    24,    26,    51,     9,    53,    25,    26,    24,    52,
      43,    26,    43,    59,    79,    80,     6,    15,    77,    51,
      26,    51,    10,    54,    43,    65,    53,     5,    31,    30,
      26,    24,    43,    59,    61,    16,    78,    26,    51,    11,
      55,    13,    67,    77,    43,    43,    79,    24,    26,    79,
      23,    90,    26,    43,    56,    57,     6,    59,    68,    69,
      14,    70,    78,    32,    61,    26,    33,    17,    81,    28,
      26,    24,    43,    30,    26,    24,    43,    71,    72,    77,
      81,    51,    91,    79,     6,    31,    58,    59,    56,    59,
      68,    28,    29,    73,    26,    24,    78,     6,    34,    26,
      43,    32,    24,    31,    42,    59,    74,    75,    76,    71,
      81,    43,    26,    43,    74,    59,    38,    37,    76,    39,
      40,    41,     6,    30,    32,    75,    76,    43,    43,    24,
      35,    60,    61,    36
  };

  const unsigned char
  spec_parser::yyr1_[] =
  {
       0,    44,    45,    46,    46,    47,    47,    47,    48,    48,
      49,    50,    51,    51,    52,    52,    53,    53,    54,    55,
      56,    56,    57,    58,    58,    59,    60,    60,    61,    61,
      63,    62,    64,    65,    65,    66,    67,    67,    68,    68,
      69,    70,    70,    71,    71,    72,    73,    73,    74,    74,
      75,    75,    75,    76,    76,    76,    76,    76,    76,    77,
      77,    78,    78,    79,    79,    80,    81,    81,    82,    83,
      83,    84,    85,    85,    86,    86,    87,    88,    88,    89,
      90,    91,    91
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
       0,     3,     0,     3,     1,     3,     3,     0,     8,     3,
       2,    10,     3,     0,     3,     0,     6,     3,     2,     9,
       5,     1,     0
  };



  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const spec_parser::yytname_[] =
  {
  "\"end of file\"", "error", "$undefined", "COMPONENT", "MODEL", "IS",
  "END", "EVENT", "INPUT", "OUTPUT", "STATE", "TRANSITION", "NETWORK",
  "LINK", "PATTERN", "INITIAL", "VIEWER", "RULER", "SYSTEM", "NODE",
  "ROOT", "EMERGENCE", "PROBLEM", "OBS", "\",\"", "\":\"", "\";\"",
  "\".\"", "\"=\"", "\"==\"", "\"->\"", "\"(\"", "\")\"", "\"[\"", "\"]\"",
  "\"{\"", "\"}\"", "\"&\"", "\"|\"", "\"*\"", "\"+\"", "\"?\"", "\"~\"",
  "\"id\"", "$accept", "specification", "spec_list", "spec", "model_decl",
  "comp_model_decl", "event_decl", "id_list", "input_decl", "output_decl",
  "state_decl", "transition_decl", "trans_decl_list", "trans_decl",
  "event", "ref", "opt_ref_list", "ref_list", "net_model_decl", "$@1",
  "component_section", "decl_list", "decl", "link_section", "link_list",
  "link_decl", "pattern_section", "pattern_list", "pattern_decl",
  "pattern_op", "expr", "term", "factor", "initial_section",
  "viewer_section", "map_list", "map_decl", "ruler_section", "system_decl",
  "system_node_list", "system_node", "root_section", "emergence_section",
  "problem_decl", "problem_node_list", "problem_node", "obs_section",
  "opt_id_list", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  spec_parser::yyrline_[] =
  {
       0,   138,   138,   141,   142,   145,   146,   147,   150,   151,
     164,   195,   198,   200,   203,   204,   207,   208,   211,   214,
     217,   218,   221,   229,   230,   233,   236,   237,   240,   241,
     246,   244,   285,   288,   289,   292,   295,   296,   299,   300,
     303,   306,   307,   310,   311,   314,   317,   318,   321,   322,
     325,   326,   327,   330,   331,   332,   333,   334,   338,   356,
     357,   360,   361,   364,   381,   385,   388,   389,   392,   415,
     416,   419,   439,   440,   443,   444,   447,   466,   467,   470,
     492,   496,   497
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
#line 2015 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.cpp" // lalr1.cc:1167
#line 501 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:1168


void yy::spec_parser::error (const location_type& l, const std::string& m)
{
    driver.error (l, m);
}


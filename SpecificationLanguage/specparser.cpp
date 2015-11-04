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

#line 37 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:404

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#include "specparser.hpp"

// User implementation prologue.

#line 51 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:412
// Unqualified %code blocks.
#line 27 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:413

#include <cstdlib>
#include <sstream>
# include "utils.h"
#include "ostream_util.h"
# include "spec_driver.h"

using namespace std;

extern yy::location loc;

#line 65 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:413


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
#line 151 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:479

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

      case 85: // problem_decl
        value.move< Problem > (that.value);
        break;

      case 87: // problem_node
        value.move< ProblemNode > (that.value);
        break;

      case 80: // system_decl
        value.move< System > (that.value);
        break;

      case 82: // system_node
        value.move< SystemNode > (that.value);
        break;

      case 56: // trans_decl
        value.move< Transition > (that.value);
        break;

      case 76: // viewer_section
      case 77: // map_list
      case 79: // ruler_section
      case 84: // emergence_section
        value.move< map<pair<string,string>,string>   > (that.value);
        break;

      case 68: // link_decl
        value.move< pair<pair<string,string>,pair<string,string> >  > (that.value);
        break;

      case 78: // map_decl
        value.move< pair<pair<string,string>,string>  > (that.value);
        break;

      case 57: // event
      case 58: // ref
      case 71: // pattern_decl
        value.move< pair<string,string>  > (that.value);
        break;

      case 42: // "id"
      case 72: // expr
      case 73: // term
      case 74: // factor
      case 83: // root_section
        value.move< string > (that.value);
        break;

      case 63: // component_section
      case 64: // decl_list
      case 65: // decl
        value.move< vector<Component>  > (that.value);
        break;

      case 86: // problem_node_list
        value.move< vector<ProblemNode>  > (that.value);
        break;

      case 81: // system_node_list
        value.move< vector<SystemNode>  > (that.value);
        break;

      case 54: // transition_decl
      case 55: // trans_decl_list
        value.move< vector<Transition>  > (that.value);
        break;

      case 66: // link_section
      case 67: // link_list
        value.move< vector<pair<pair<string,string>,pair<string,string> > >  > (that.value);
        break;

      case 59: // opt_ref_list
      case 60: // ref_list
      case 69: // pattern_section
      case 70: // pattern_list
      case 75: // initial_section
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

      case 85: // problem_decl
        value.copy< Problem > (that.value);
        break;

      case 87: // problem_node
        value.copy< ProblemNode > (that.value);
        break;

      case 80: // system_decl
        value.copy< System > (that.value);
        break;

      case 82: // system_node
        value.copy< SystemNode > (that.value);
        break;

      case 56: // trans_decl
        value.copy< Transition > (that.value);
        break;

      case 76: // viewer_section
      case 77: // map_list
      case 79: // ruler_section
      case 84: // emergence_section
        value.copy< map<pair<string,string>,string>   > (that.value);
        break;

      case 68: // link_decl
        value.copy< pair<pair<string,string>,pair<string,string> >  > (that.value);
        break;

      case 78: // map_decl
        value.copy< pair<pair<string,string>,string>  > (that.value);
        break;

      case 57: // event
      case 58: // ref
      case 71: // pattern_decl
        value.copy< pair<string,string>  > (that.value);
        break;

      case 42: // "id"
      case 72: // expr
      case 73: // term
      case 74: // factor
      case 83: // root_section
        value.copy< string > (that.value);
        break;

      case 63: // component_section
      case 64: // decl_list
      case 65: // decl
        value.copy< vector<Component>  > (that.value);
        break;

      case 86: // problem_node_list
        value.copy< vector<ProblemNode>  > (that.value);
        break;

      case 81: // system_node_list
        value.copy< vector<SystemNode>  > (that.value);
        break;

      case 54: // transition_decl
      case 55: // trans_decl_list
        value.copy< vector<Transition>  > (that.value);
        break;

      case 66: // link_section
      case 67: // link_list
        value.copy< vector<pair<pair<string,string>,pair<string,string> > >  > (that.value);
        break;

      case 59: // opt_ref_list
      case 60: // ref_list
      case 69: // pattern_section
      case 70: // pattern_list
      case 75: // initial_section
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

#line 128 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 516 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 48: // comp_model_decl

#line 129 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< ComponentModel > (); }
#line 523 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 56: // trans_decl

#line 130 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< Transition > (); }
#line 530 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 61: // net_model_decl

#line 131 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< NetworkModel > (); }
#line 537 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 72: // expr

#line 128 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 544 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 73: // term

#line 128 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 551 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 74: // factor

#line 128 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 558 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
        break;

      case 83: // root_section

#line 128 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:636
        { yyoutput << yysym.value.template as< string > (); }
#line 565 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:636
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
    #line 20 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:741
{
  // Initialize the initial location.
  yyla.location.begin.filename = yyla.location.end.filename = &driver.file;
}

#line 684 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:741

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

      case 85: // problem_decl
        yylhs.value.build< Problem > ();
        break;

      case 87: // problem_node
        yylhs.value.build< ProblemNode > ();
        break;

      case 80: // system_decl
        yylhs.value.build< System > ();
        break;

      case 82: // system_node
        yylhs.value.build< SystemNode > ();
        break;

      case 56: // trans_decl
        yylhs.value.build< Transition > ();
        break;

      case 76: // viewer_section
      case 77: // map_list
      case 79: // ruler_section
      case 84: // emergence_section
        yylhs.value.build< map<pair<string,string>,string>   > ();
        break;

      case 68: // link_decl
        yylhs.value.build< pair<pair<string,string>,pair<string,string> >  > ();
        break;

      case 78: // map_decl
        yylhs.value.build< pair<pair<string,string>,string>  > ();
        break;

      case 57: // event
      case 58: // ref
      case 71: // pattern_decl
        yylhs.value.build< pair<string,string>  > ();
        break;

      case 42: // "id"
      case 72: // expr
      case 73: // term
      case 74: // factor
      case 83: // root_section
        yylhs.value.build< string > ();
        break;

      case 63: // component_section
      case 64: // decl_list
      case 65: // decl
        yylhs.value.build< vector<Component>  > ();
        break;

      case 86: // problem_node_list
        yylhs.value.build< vector<ProblemNode>  > ();
        break;

      case 81: // system_node_list
        yylhs.value.build< vector<SystemNode>  > ();
        break;

      case 54: // transition_decl
      case 55: // trans_decl_list
        yylhs.value.build< vector<Transition>  > ();
        break;

      case 66: // link_section
      case 67: // link_list
        yylhs.value.build< vector<pair<pair<string,string>,pair<string,string> > >  > ();
        break;

      case 59: // opt_ref_list
      case 60: // ref_list
      case 69: // pattern_section
      case 70: // pattern_list
      case 75: // initial_section
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
#line 137 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  }
#line 893 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 3:
#line 140 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  }
#line 899 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 4:
#line 141 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 905 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 5:
#line 144 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 911 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 6:
#line 145 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.system = System(yystack_[0].value.as< System > ()); }
#line 917 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 7:
#line 146 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { driver.problem = Problem(yystack_[0].value.as< Problem > ()); }
#line 923 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 8:
#line 149 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { ComponentModel(yystack_[0].value.as< ComponentModel > ()); driver.components.push_back(yystack_[0].value.as< ComponentModel > ()); }
#line 929 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 9:
#line 151 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 944 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 10:
#line 170 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 971 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 11:
#line 194 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 977 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 12:
#line 198 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > ().push_back(yystack_[2].value.as< string > ()); yylhs.value.as< vector<string>  > () = Utils::merge(yylhs.value.as< vector<string>  > (),yystack_[0].value.as< vector<string>  > ()); }
#line 983 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 13:
#line 199 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > ().push_back(yystack_[0].value.as< string > ()); }
#line 989 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 14:
#line 202 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 995 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 15:
#line 203 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1001 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 16:
#line 206 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 1007 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 17:
#line 207 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1013 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 18:
#line 210 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[1].value.as< vector<string>  > (); }
#line 1019 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 19:
#line 213 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > () = yystack_[1].value.as< vector<Transition>  > (); }
#line 1025 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 20:
#line 216 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > ().push_back(yystack_[2].value.as< Transition > ()); yylhs.value.as< vector<Transition>  > () = Utils::merge(yylhs.value.as< vector<Transition>  > (),yystack_[0].value.as< vector<Transition>  > ()); }
#line 1031 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 21:
#line 217 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Transition>  > ().push_back(yystack_[0].value.as< Transition > ()); }
#line 1037 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 22:
#line 221 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                    yylhs.value.as< Transition > ().name = yystack_[10].value.as< string > (); yylhs.value.as< Transition > ().input_event = yystack_[8].value.as< pair<string,string>  > ();
                    string s1 = yystack_[6].value.as< string > (); string s2 = yystack_[4].value.as< string > (); yylhs.value.as< Transition > ().s1_s2 = make_pair(s1,s2);
                    yylhs.value.as< Transition > ().out_events = yystack_[1].value.as< vector<pair<string,string> >  > ();
                  }
#line 1047 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 23:
#line 228 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< pair<string,string>  > () = yystack_[0].value.as< pair<string,string>  > (); }
#line 1053 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 24:
#line 229 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {}
#line 1059 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 25:
#line 232 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string s1 = yystack_[3].value.as< string > (); string s2 = yystack_[1].value.as< string > (); yylhs.value.as< pair<string,string>  > () = make_pair(s1,s2); }
#line 1065 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 26:
#line 235 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[0].value.as< vector<pair<string,string> >  > (); }
#line 1071 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 27:
#line 236 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {}
#line 1077 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 28:
#line 239 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[2].value.as< pair<string,string>  > ()); yylhs.value.as< vector<pair<string,string> >  > () = Utils::merge(yylhs.value.as< vector<pair<string,string> >  > (),yystack_[0].value.as< vector<pair<string,string> >  > ()); }
#line 1083 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 29:
#line 240 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[0].value.as< pair<string,string>  > ()); }
#line 1089 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 30:
#line 245 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                        driver.current_net_model.components = yystack_[0].value.as< vector<Component>  > ();
                    }
#line 1097 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 31:
#line 254 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {
                    if(driver.duplicate_network_model_id(yystack_[10].value.as< string > ()))
                    {
                        string msg = "Semantic error: duplicate network model ID ";
                        msg.append(yystack_[10].value.as< string > ());
                        driver.error(loc,msg);
                    }
                    if(yystack_[10].value.as< string > () != yystack_[0].value.as< string > ())
                    {
                        string msg = "Semantic error: wrong end ID ";
                        msg.append(yystack_[0].value.as< string > ());
                        driver.error(loc, msg);
                    }
                    yylhs.value.as< NetworkModel > ().name = yystack_[10].value.as< string > ();
                    yylhs.value.as< NetworkModel > ().components = yystack_[8].value.as< vector<Component>  > ();
                    yylhs.value.as< NetworkModel > ().links = yystack_[6].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ();
                    yylhs.value.as< NetworkModel > ().patterns = yystack_[5].value.as< vector<pair<string,string> >  > ();
                    yylhs.value.as< NetworkModel > ().initials = yystack_[4].value.as< vector<pair<string,string> >  > ();
                    yylhs.value.as< NetworkModel > ().viewer = yystack_[3].value.as< map<pair<string,string>,string>   > ();
                    yylhs.value.as< NetworkModel > ().ruler = yystack_[2].value.as< map<pair<string,string>,string>   > ();

                    driver.semantic_checks(yylhs.value.as< NetworkModel > ());
                  }
#line 1125 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 32:
#line 279 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = yystack_[1].value.as< vector<Component>  > (); }
#line 1131 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 33:
#line 282 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[2].value.as< vector<Component>  > ()); yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[0].value.as< vector<Component>  > ()); }
#line 1137 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 34:
#line 283 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = Utils::merge(yylhs.value.as< vector<Component>  > (),yystack_[0].value.as< vector<Component>  > ()); }
#line 1143 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 35:
#line 286 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<Component>  > () = driver.build_components(yystack_[2].value.as< vector<string>  > (),yystack_[0].value.as< string > ());  }
#line 1149 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 36:
#line 289 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > () = yystack_[1].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > (); }
#line 1155 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 37:
#line 290 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1161 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 38:
#line 293 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ().push_back(yystack_[2].value.as< pair<pair<string,string>,pair<string,string> >  > ()); yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > () = Utils::merge(yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > (),yystack_[0].value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ()); }
#line 1167 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 39:
#line 294 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<pair<string,string>,pair<string,string> > >  > ().push_back(yystack_[0].value.as< pair<pair<string,string>,pair<string,string> >  > ()); }
#line 1173 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 40:
#line 297 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    {  yylhs.value.as< pair<pair<string,string>,pair<string,string> >  > () = make_pair(yystack_[2].value.as< pair<string,string>  > (),yystack_[0].value.as< pair<string,string>  > ()); }
#line 1179 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 41:
#line 300 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[1].value.as< vector<pair<string,string> >  > (); }
#line 1185 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 42:
#line 301 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1191 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 43:
#line 304 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[2].value.as< pair<string,string>  > ()); yylhs.value.as< vector<pair<string,string> >  > () = Utils::merge(yylhs.value.as< vector<pair<string,string> >  > (),yystack_[0].value.as< vector<pair<string,string> >  > ()); }
#line 1197 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 44:
#line 305 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > ().push_back(yystack_[0].value.as< pair<string,string>  > ()); }
#line 1203 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 45:
#line 308 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string s1 = yystack_[2].value.as< string > (); string s2 = yystack_[0].value.as< string > (); yylhs.value.as< pair<string,string>  > () = make_pair(s1,s2); }
#line 1209 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 46:
#line 311 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1215 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 47:
#line 312 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1221 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 48:
#line 315 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1227 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 49:
#line 316 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[2].value.as< string > ()); yylhs.value.as< string > ().append(")"); }
#line 1233 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 50:
#line 317 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[0].value.as< string > ()); }
#line 1239 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 51:
#line 320 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("*"); }
#line 1245 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 52:
#line 321 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("*");}
#line 1251 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 53:
#line 322 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append("|"); yylhs.value.as< string > ().append("\"\""); yylhs.value.as< string > ().append(")"); }
#line 1257 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 54:
#line 323 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > ().append("("); yylhs.value.as< string > ().append(yystack_[1].value.as< string > ()); yylhs.value.as< string > ().append(")"); }
#line 1263 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 55:
#line 325 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { string str = yystack_[0].value.as< pair<string,string>  > ().first; str.append("("); str.append(yystack_[0].value.as< pair<string,string>  > ().second); str.append(")");
                      yylhs.value.as< string > ().append(driver.current_net_model.not_trans(str));
                    }
#line 1271 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 56:
#line 329 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1291 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 57:
#line 346 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<pair<string,string> >  > () = yystack_[1].value.as< vector<pair<string,string> >  > (); }
#line 1297 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 58:
#line 347 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1303 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 59:
#line 350 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1309 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 60:
#line 351 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1315 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 61:
#line 355 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1336 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 62:
#line 372 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > ().insert(yystack_[0].value.as< pair<pair<string,string>,string>  > ()); }
#line 1342 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 63:
#line 375 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< pair<pair<string,string>,string>  > ().first = yystack_[2].value.as< pair<string,string>  > (); yylhs.value.as< pair<pair<string,string>,string>  > ().second = yystack_[0].value.as< string > (); }
#line 1348 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 64:
#line 378 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1354 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 65:
#line 379 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1360 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 66:
#line 387 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1381 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 67:
#line 405 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<SystemNode>  > ().push_back(yystack_[2].value.as< SystemNode > ()); yylhs.value.as< vector<SystemNode>  > () = Utils::merge(yylhs.value.as< vector<SystemNode>  > (),yystack_[0].value.as< vector<SystemNode>  > ()); }
#line 1387 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 68:
#line 406 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<SystemNode>  > ().push_back(yystack_[1].value.as< SystemNode > ()); }
#line 1393 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 69:
#line 414 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1411 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 70:
#line 429 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< string > () = yystack_[1].value.as< string > (); }
#line 1417 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 71:
#line 430 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1423 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 72:
#line 433 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< map<pair<string,string>,string>   > () = yystack_[1].value.as< map<pair<string,string>,string>   > (); }
#line 1429 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 73:
#line 434 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1435 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 74:
#line 440 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1454 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 75:
#line 456 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<ProblemNode>  > ().push_back(yystack_[2].value.as< ProblemNode > ()); yylhs.value.as< vector<ProblemNode>  > () = Utils::merge(yylhs.value.as< vector<ProblemNode>  > (),yystack_[0].value.as< vector<ProblemNode>  > ()); }
#line 1460 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 76:
#line 457 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<ProblemNode>  > ().push_back(yystack_[1].value.as< ProblemNode > ()); }
#line 1466 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 77:
#line 466 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
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
#line 1485 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 78:
#line 483 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[2].value.as< vector<string>  > (); }
#line 1491 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 79:
#line 486 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { yylhs.value.as< vector<string>  > () = yystack_[0].value.as< vector<string>  > (); }
#line 1497 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;

  case 80:
#line 487 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:859
    { }
#line 1503 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
    break;


#line 1507 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:859
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


  const short int spec_parser::yypact_ninf_ = -146;

  const signed char spec_parser::yytable_ninf_ = -1;

  const short int
  spec_parser::yypact_[] =
  {
       3,     8,    14,     2,     6,    47,  -146,    22,  -146,  -146,
    -146,  -146,  -146,    12,    13,    45,    51,  -146,     3,    52,
      53,    40,    41,  -146,    54,    60,    23,    44,    -4,    24,
      61,     5,    26,    62,    26,  -146,    48,    27,    55,    40,
    -146,    67,    32,    41,  -146,    56,    49,    26,    68,    57,
      58,    63,    65,    37,    66,    39,    80,  -146,    73,  -146,
    -146,    26,  -146,    69,    26,    79,    59,  -146,    26,    39,
      77,    88,  -146,    64,    70,    71,    72,    74,    39,    82,
    -146,  -146,    76,    26,    89,  -146,  -146,    75,    81,    84,
      78,    73,    73,    83,    85,  -146,    39,  -146,    86,    87,
      39,    91,  -146,    92,    90,    97,    39,  -146,    39,    93,
      96,    95,    82,    82,    98,  -146,  -146,    39,  -146,   100,
     101,    94,  -146,   102,   108,    99,   103,  -146,  -146,    -6,
    -146,    78,    94,    94,  -146,  -146,  -146,    26,    39,   106,
     -25,  -146,    90,  -146,    -6,    39,  -146,   104,    -2,   -29,
    -146,   109,   111,  -146,   105,   110,   107,   112,   113,  -146,
    -146,   -24,  -146,    -6,    -6,   -29,  -146,  -146,  -146,   114,
     115,   116,  -146,  -146,  -146,   117,  -146,    -2,   -29,  -146,
    -146,  -146,   118,   119,   120,   121,    39,   123,  -146,  -146
  };

  const unsigned char
  spec_parser::yydefact_[] =
  {
       0,     0,     0,     0,     0,     0,     2,     0,     5,     8,
       9,     6,     7,     0,     0,     0,     0,     1,     4,     0,
       0,     0,     0,     3,     0,     0,     0,    71,     0,     0,
       0,     0,     0,    15,     0,    30,     0,     0,    73,     0,
      68,     0,     0,     0,    76,    13,     0,     0,    17,     0,
       0,    34,    37,     0,     0,     0,     0,    67,    58,    74,
      75,     0,    11,     0,     0,     0,     0,    32,     0,     0,
      42,     0,    70,     0,     0,     0,    62,     0,     0,    60,
      12,    14,     0,     0,     0,    35,    33,     0,     0,    39,
       0,    58,    58,     0,     0,    72,     0,    66,    29,     0,
       0,     0,    16,     0,     0,     0,     0,    36,     0,     0,
       0,    44,    60,    60,     0,    63,    61,     0,    57,     0,
       0,    65,    18,     0,     0,    21,     0,    40,    38,     0,
      41,     0,    65,    65,    25,    28,    59,    80,     0,     0,
       0,    19,     0,    10,     0,     0,    56,    45,    47,    50,
      43,     0,     0,    79,     0,     0,     0,     0,     0,    23,
      20,     0,    55,     0,     0,    48,    51,    52,    53,     0,
       0,     0,    64,    77,    24,     0,    54,    46,    49,    31,
      69,    78,     0,     0,     0,     0,    27,     0,    26,    22
  };

  const short int
  spec_parser::yypgoto_[] =
  {
    -146,  -146,   122,  -146,  -146,  -146,  -146,   -31,  -146,  -146,
    -146,  -146,   -37,  -146,  -146,   -55,  -146,  -115,  -146,  -146,
    -146,   124,  -146,  -146,     1,  -146,  -146,    -7,  -146,   -16,
     -28,  -145,   -65,   -75,   -92,  -146,   -90,  -146,   125,  -146,
    -146,  -146,  -146,   126,  -146,  -146,  -146
  };

  const short int
  spec_parser::yydefgoto_[] =
  {
      -1,     5,     6,     7,     8,     9,    33,    49,    48,    65,
      84,   105,   124,   125,   158,   146,   187,    99,    10,    52,
      35,    50,    51,    70,    88,    89,    91,   110,   111,   147,
     148,   149,    79,   101,    75,    76,   139,    11,    27,    28,
      38,    56,    12,    30,    31,   121,   154
  };

  const unsigned char
  spec_parser::yytable_[] =
  {
      74,    46,   135,   165,   116,   157,     1,   176,   119,   166,
     167,   168,    13,   163,    87,     2,    63,    73,    14,   178,
      39,     3,    40,    98,   144,     4,   112,   113,   144,    43,
      80,    44,   165,    82,   164,   145,    73,   132,   133,   145,
      73,    74,   151,   152,    15,    74,   155,    17,    16,    18,
      21,   127,   103,    87,    19,    20,    22,    24,    25,    26,
      29,    32,    98,    34,    37,    36,    41,    42,    45,    54,
      47,   188,    58,    53,    59,    62,    55,    64,    69,    71,
      61,    73,    66,    74,    67,   159,    77,    68,    78,    83,
     162,    90,    72,    92,    93,    81,    96,    95,   100,    94,
     104,    85,   102,   126,   106,   160,   153,   107,   108,   128,
     117,   138,   156,   118,   120,   169,    97,   170,   122,   131,
     109,   129,   130,   142,   150,   114,   136,   115,   161,   134,
     140,    98,   123,   137,   141,   177,   172,   175,   171,     0,
      23,   163,   181,   174,   185,   143,     0,   183,     0,   173,
       0,     0,     0,     0,     0,   186,   179,   180,   189,   182,
       0,   184,     0,     0,    57,     0,     0,     0,     0,    60,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    86
  };

  const short int
  spec_parser::yycheck_[] =
  {
      55,    32,   117,   148,    96,    30,     3,    31,   100,    38,
      39,    40,     4,    37,    69,    12,    47,    42,     4,   164,
      24,    18,    26,    78,    30,    22,    91,    92,    30,    24,
      61,    26,   177,    64,    36,    41,    42,   112,   113,    41,
      42,    96,   132,   133,    42,   100,   138,     0,    42,    27,
       5,   106,    83,   108,    42,    42,     5,     5,     5,    19,
      19,     7,   117,     3,    20,    42,    42,     6,    42,    42,
       8,   186,     5,    25,    42,    26,    21,     9,    13,    42,
      24,    42,    25,   138,    26,   140,     6,    24,    15,    10,
     145,    14,    26,     5,    30,    26,    24,    26,    16,    29,
      11,    42,    26,     6,    29,   142,   137,    26,    24,   108,
      24,    17,     6,    26,    23,     6,    42,     6,    26,    24,
      42,    28,    26,    24,   131,    42,    26,    42,   144,    31,
      28,   186,    42,    32,    26,   163,    26,    24,    33,    -1,
      18,    37,    26,    31,    24,    42,    -1,    29,    -1,    42,
      -1,    -1,    -1,    -1,    -1,    34,    42,    42,    35,    42,
      -1,    42,    -1,    -1,    39,    -1,    -1,    -1,    -1,    43,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    68
  };

  const unsigned char
  spec_parser::yystos_[] =
  {
       0,     3,    12,    18,    22,    44,    45,    46,    47,    48,
      61,    80,    85,     4,     4,    42,    42,     0,    27,    42,
      42,     5,     5,    45,     5,     5,    19,    81,    82,    19,
      86,    87,     7,    49,     3,    63,    42,    20,    83,    24,
      26,    42,     6,    24,    26,    42,    50,     8,    51,    50,
      64,    65,    62,    25,    42,    21,    84,    81,     5,    42,
      86,    24,    26,    50,     9,    52,    25,    26,    24,    13,
      66,    42,    26,    42,    58,    77,    78,     6,    15,    75,
      50,    26,    50,    10,    53,    42,    64,    58,    67,    68,
      14,    69,     5,    30,    29,    26,    24,    42,    58,    60,
      16,    76,    26,    50,    11,    54,    29,    26,    24,    42,
      70,    71,    75,    75,    42,    42,    77,    24,    26,    77,
      23,    88,    26,    42,    55,    56,     6,    58,    67,    28,
      26,    24,    76,    76,    31,    60,    26,    32,    17,    79,
      28,    26,    24,    42,    30,    41,    58,    72,    73,    74,
      70,    79,    79,    50,    89,    77,     6,    30,    57,    58,
      55,    72,    58,    37,    36,    74,    38,    39,    40,     6,
       6,    33,    26,    42,    31,    24,    31,    73,    74,    42,
      42,    26,    42,    29,    42,    24,    34,    59,    60,    35
  };

  const unsigned char
  spec_parser::yyr1_[] =
  {
       0,    43,    44,    45,    45,    46,    46,    46,    47,    47,
      48,    49,    50,    50,    51,    51,    52,    52,    53,    54,
      55,    55,    56,    57,    57,    58,    59,    59,    60,    60,
      62,    61,    63,    64,    64,    65,    66,    66,    67,    67,
      68,    69,    69,    70,    70,    71,    72,    72,    73,    73,
      73,    74,    74,    74,    74,    74,    74,    75,    75,    76,
      76,    77,    77,    78,    79,    79,    80,    81,    81,    82,
      83,    83,    84,    84,    85,    86,    86,    87,    88,    89,
      89
  };

  const unsigned char
  spec_parser::yyr2_[] =
  {
       0,     2,     1,     3,     2,     1,     1,     1,     1,     1,
      11,     3,     3,     1,     3,     0,     3,     0,     3,     3,
       3,     1,    11,     1,     2,     4,     1,     0,     3,     1,
       0,    13,     3,     3,     1,     3,     3,     0,     3,     1,
       3,     3,     0,     3,     1,     3,     3,     1,     2,     3,
       1,     2,     2,     2,     3,     2,     1,     3,     0,     3,
       0,     3,     1,     3,     3,     0,     8,     3,     2,    10,
       3,     0,     3,     0,     6,     3,     2,     9,     5,     1,
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
  "ROOT", "EMERGENCE", "PROBLEM", "OBS", "\",\"", "\":\"", "\";\"",
  "\".\"", "\"=\"", "\"->\"", "\"(\"", "\")\"", "\"[\"", "\"]\"", "\"{\"",
  "\"}\"", "\"&\"", "\"|\"", "\"*\"", "\"+\"", "\"?\"", "\"~\"", "\"id\"",
  "$accept", "specification", "spec_list", "spec", "model_decl",
  "comp_model_decl", "event_decl", "id_list", "input_decl", "output_decl",
  "state_decl", "transition_decl", "trans_decl_list", "trans_decl",
  "event", "ref", "opt_ref_list", "ref_list", "net_model_decl", "$@1",
  "component_section", "decl_list", "decl", "link_section", "link_list",
  "link_decl", "pattern_section", "pattern_list", "pattern_decl", "expr",
  "term", "factor", "initial_section", "viewer_section", "map_list",
  "map_decl", "ruler_section", "system_decl", "system_node_list",
  "system_node", "root_section", "emergence_section", "problem_decl",
  "problem_node_list", "problem_node", "obs_section", "opt_id_list", YY_NULLPTR
  };

#if YYDEBUG
  const unsigned short int
  spec_parser::yyrline_[] =
  {
       0,   137,   137,   140,   141,   144,   145,   146,   149,   150,
     163,   194,   197,   199,   202,   203,   206,   207,   210,   213,
     216,   217,   220,   228,   229,   232,   235,   236,   239,   240,
     245,   243,   279,   282,   283,   286,   289,   290,   293,   294,
     297,   300,   301,   304,   305,   308,   311,   312,   315,   316,
     317,   320,   321,   322,   323,   324,   328,   346,   347,   350,
     351,   354,   371,   375,   378,   379,   382,   405,   406,   409,
     429,   430,   433,   434,   437,   456,   457,   460,   482,   486,
     487
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
#line 2008 "/home/giulio/SpecificationLanguage/specparser.cpp" // lalr1.cc:1167
#line 491 "/home/giulio/SpecificationLanguage/specparser.yy" // lalr1.cc:1168


void yy::spec_parser::error (const location_type& l, const std::string& m)
{
    driver.error (l, m);
}


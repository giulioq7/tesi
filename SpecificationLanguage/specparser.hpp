// A Bison parser, made by GNU Bison 3.0.4.

// Skeleton interface for Bison LALR(1) parsers in C++

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

/**
 ** \file /home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.hpp
 ** Define the yy::parser class.
 */

// C++ LALR(1) parser skeleton written by Akim Demaille.

#ifndef YY_YY_HOME_GIULIO_SCRIVANIA_TESI_SPECIFICATIONLANGUAGE_SPECPARSER_HPP_INCLUDED
# define YY_YY_HOME_GIULIO_SCRIVANIA_TESI_SPECIFICATIONLANGUAGE_SPECPARSER_HPP_INCLUDED
// //                    "%code requires" blocks.
#line 9 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.yy" // lalr1.cc:377

# include <string>
# include <list>
# include "system.h"
# include "problem.h"
class spec_driver;

#line 52 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.hpp" // lalr1.cc:377

# include <cassert>
# include <cstdlib> // std::abort
# include <iostream>
# include <stdexcept>
# include <string>
# include <vector>
# include "stack.hh"
# include "location.hh"
#include <typeinfo>
#ifndef YYASSERT
# include <cassert>
# define YYASSERT assert
#endif


#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif


namespace yy {
#line 129 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.hpp" // lalr1.cc:377



  /// A char[S] buffer to store and retrieve objects.
  ///
  /// Sort of a variant, but does not keep track of the nature
  /// of the stored data, since that knowledge is available
  /// via the current state.
  template <size_t S>
  struct variant
  {
    /// Type of *this.
    typedef variant<S> self_type;

    /// Empty construction.
    variant ()
      : yytypeid_ (YY_NULLPTR)
    {}

    /// Construct and fill.
    template <typename T>
    variant (const T& t)
      : yytypeid_ (&typeid (T))
    {
      YYASSERT (sizeof (T) <= S);
      new (yyas_<T> ()) T (t);
    }

    /// Destruction, allowed only if empty.
    ~variant ()
    {
      YYASSERT (!yytypeid_);
    }

    /// Instantiate an empty \a T in here.
    template <typename T>
    T&
    build ()
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T;
    }

    /// Instantiate a \a T in here from \a t.
    template <typename T>
    T&
    build (const T& t)
    {
      YYASSERT (!yytypeid_);
      YYASSERT (sizeof (T) <= S);
      yytypeid_ = & typeid (T);
      return *new (yyas_<T> ()) T (t);
    }

    /// Accessor to a built \a T.
    template <typename T>
    T&
    as ()
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Const accessor to a built \a T (for %printer).
    template <typename T>
    const T&
    as () const
    {
      YYASSERT (*yytypeid_ == typeid (T));
      YYASSERT (sizeof (T) <= S);
      return *yyas_<T> ();
    }

    /// Swap the content with \a other, of same type.
    ///
    /// Both variants must be built beforehand, because swapping the actual
    /// data requires reading it (with as()), and this is not possible on
    /// unconstructed variants: it would require some dynamic testing, which
    /// should not be the variant's responsability.
    /// Swapping between built and (possibly) non-built is done with
    /// variant::move ().
    template <typename T>
    void
    swap (self_type& other)
    {
      YYASSERT (yytypeid_);
      YYASSERT (*yytypeid_ == *other.yytypeid_);
      std::swap (as<T> (), other.as<T> ());
    }

    /// Move the content of \a other to this.
    ///
    /// Destroys \a other.
    template <typename T>
    void
    move (self_type& other)
    {
      build<T> ();
      swap<T> (other);
      other.destroy<T> ();
    }

    /// Copy the content of \a other to this.
    template <typename T>
    void
    copy (const self_type& other)
    {
      build<T> (other.as<T> ());
    }

    /// Destroy the stored \a T.
    template <typename T>
    void
    destroy ()
    {
      as<T> ().~T ();
      yytypeid_ = YY_NULLPTR;
    }

  private:
    /// Prohibit blind copies.
    self_type& operator=(const self_type&);
    variant (const self_type&);

    /// Accessor to raw memory as \a T.
    template <typename T>
    T*
    yyas_ ()
    {
      void *yyp = yybuffer_.yyraw;
      return static_cast<T*> (yyp);
     }

    /// Const accessor to raw memory as \a T.
    template <typename T>
    const T*
    yyas_ () const
    {
      const void *yyp = yybuffer_.yyraw;
      return static_cast<const T*> (yyp);
     }

    union
    {
      /// Strongest alignment constraints.
      long double yyalign_me;
      /// A buffer large enough to store any of the semantic values.
      char yyraw[S];
    } yybuffer_;

    /// Whether the content is built: if defined, the name of the stored type.
    const std::type_info *yytypeid_;
  };


  /// A Bison parser.
  class spec_parser
  {
  public:
#ifndef YYSTYPE
    /// An auxiliary type to compute the largest semantic type.
    union union_type
    {
      // comp_model_decl
      char dummy1[sizeof(ComponentModel)];

      // net_model_decl
      char dummy2[sizeof(NetworkModel)];

      // pattern_decl
      char dummy3[sizeof(Pattern)];

      // problem_decl
      char dummy4[sizeof(Problem)];

      // problem_node
      char dummy5[sizeof(ProblemNode)];

      // system_decl
      char dummy6[sizeof(System)];

      // system_node
      char dummy7[sizeof(SystemNode)];

      // trans_decl
      char dummy8[sizeof(Transition)];

      // pattern_op
      char dummy9[sizeof(bool)];

      // viewer_section
      // map_list
      // ruler_section
      char dummy10[sizeof(map<pair<string,string>,string>  )];

      // link_decl
      char dummy11[sizeof(pair<pair<string,string>,pair<string,string> > )];

      // map_decl
      char dummy12[sizeof(pair<pair<string,string>,string> )];

      // event
      // ref
      char dummy13[sizeof(pair<string,string> )];

      // "id"
      // expr
      // term
      // factor
      // root_section
      char dummy14[sizeof(string)];

      // component_section
      // decl_list
      // decl
      char dummy15[sizeof(vector<Component> )];

      // pattern_section
      // pattern_list
      char dummy16[sizeof(vector<Pattern> )];

      // problem_node_list
      char dummy17[sizeof(vector<ProblemNode> )];

      // system_node_list
      char dummy18[sizeof(vector<SystemNode> )];

      // transition_decl
      // trans_decl_list
      char dummy19[sizeof(vector<Transition> )];

      // link_section
      // link_list
      // emergence_section
      char dummy20[sizeof(vector<pair<pair<string,string>,pair<string,string> > > )];

      // opt_ref_list
      // ref_list
      // initial_section
      char dummy21[sizeof(vector<pair<string,string> > )];

      // event_decl
      // id_list
      // input_decl
      // output_decl
      // state_decl
      // obs_section
      // opt_id_list
      char dummy22[sizeof(vector<string> )];
};

    /// Symbol semantic values.
    typedef variant<sizeof(union_type)> semantic_type;
#else
    typedef YYSTYPE semantic_type;
#endif
    /// Symbol locations.
    typedef location location_type;

    /// Syntax errors thrown from user actions.
    struct syntax_error : std::runtime_error
    {
      syntax_error (const location_type& l, const std::string& m);
      location_type location;
    };

    /// Tokens.
    struct token
    {
      enum yytokentype
      {
        END_OF_FILE = 0,
        COMPONENT = 258,
        MODEL = 259,
        IS = 260,
        END = 261,
        EVENT = 262,
        INPUT = 263,
        OUTPUT = 264,
        STATE = 265,
        TRANSITION = 266,
        NETWORK = 267,
        LINK = 268,
        PATTERN = 269,
        INITIAL = 270,
        VIEWER = 271,
        RULER = 272,
        SYSTEM = 273,
        NODE = 274,
        ROOT = 275,
        EMERGENCE = 276,
        PROBLEM = 277,
        OBS = 278,
        COMMA = 279,
        COLON = 280,
        SEMI_COLON = 281,
        DOT = 282,
        EQUALS = 283,
        DOUBLE_EQUALS = 284,
        ARROW = 285,
        L_BRACKET = 286,
        R_BRACKET = 287,
        L_SQUARE_BRACKET = 288,
        R_SQUARE_BRACKET = 289,
        L_CURLY_BRACKET = 290,
        R_CURLY_BRACKET = 291,
        AND = 292,
        PIPE = 293,
        STAR = 294,
        PLUS = 295,
        OPT = 296,
        TILDE = 297,
        ID = 298
      };
    };

    /// (External) token type, as returned by yylex.
    typedef token::yytokentype token_type;

    /// Symbol type: an internal symbol number.
    typedef int symbol_number_type;

    /// The symbol type number to denote an empty symbol.
    enum { empty_symbol = -2 };

    /// Internal symbol number for tokens (subsumed by symbol_number_type).
    typedef unsigned char token_number_type;

    /// A complete symbol.
    ///
    /// Expects its Base type to provide access to the symbol type
    /// via type_get().
    ///
    /// Provide access to semantic value and location.
    template <typename Base>
    struct basic_symbol : Base
    {
      /// Alias to Base.
      typedef Base super_type;

      /// Default constructor.
      basic_symbol ();

      /// Copy constructor.
      basic_symbol (const basic_symbol& other);

      /// Constructor for valueless symbols, and symbols from each type.

  basic_symbol (typename Base::kind_type t, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ComponentModel v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const NetworkModel v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Pattern v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Problem v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const ProblemNode v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const System v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const SystemNode v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const Transition v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const bool v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const map<pair<string, string>, string>   v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const pair<pair<string, string>, pair<string, string> >  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const pair<pair<string, string>, string>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const pair<string, string>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const string v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<Component>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<Pattern>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<ProblemNode>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<SystemNode>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<Transition>  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<pair<pair<string, string>, pair<string, string> > >  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<pair<string, string> >  v, const location_type& l);

  basic_symbol (typename Base::kind_type t, const vector<string>  v, const location_type& l);


      /// Constructor for symbols with semantic value.
      basic_symbol (typename Base::kind_type t,
                    const semantic_type& v,
                    const location_type& l);

      /// Destroy the symbol.
      ~basic_symbol ();

      /// Destroy contents, and record that is empty.
      void clear ();

      /// Whether empty.
      bool empty () const;

      /// Destructive move, \a s is emptied into this.
      void move (basic_symbol& s);

      /// The semantic value.
      semantic_type value;

      /// The location.
      location_type location;

    private:
      /// Assignment operator.
      basic_symbol& operator= (const basic_symbol& other);
    };

    /// Type access provider for token (enum) based symbols.
    struct by_type
    {
      /// Default constructor.
      by_type ();

      /// Copy constructor.
      by_type (const by_type& other);

      /// The symbol type as needed by the constructor.
      typedef token_type kind_type;

      /// Constructor from (external) token numbers.
      by_type (kind_type t);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_type& that);

      /// The (internal) type number (corresponding to \a type).
      /// \a empty when empty.
      symbol_number_type type_get () const;

      /// The token.
      token_type token () const;

      /// The symbol type.
      /// \a empty_symbol when empty.
      /// An int, not token_number_type, to be able to store empty_symbol.
      int type;
    };

    /// "External" symbols: returned by the scanner.
    typedef basic_symbol<by_type> symbol_type;

    // Symbol constructors declarations.
    static inline
    symbol_type
    make_END_OF_FILE (const location_type& l);

    static inline
    symbol_type
    make_COMPONENT (const location_type& l);

    static inline
    symbol_type
    make_MODEL (const location_type& l);

    static inline
    symbol_type
    make_IS (const location_type& l);

    static inline
    symbol_type
    make_END (const location_type& l);

    static inline
    symbol_type
    make_EVENT (const location_type& l);

    static inline
    symbol_type
    make_INPUT (const location_type& l);

    static inline
    symbol_type
    make_OUTPUT (const location_type& l);

    static inline
    symbol_type
    make_STATE (const location_type& l);

    static inline
    symbol_type
    make_TRANSITION (const location_type& l);

    static inline
    symbol_type
    make_NETWORK (const location_type& l);

    static inline
    symbol_type
    make_LINK (const location_type& l);

    static inline
    symbol_type
    make_PATTERN (const location_type& l);

    static inline
    symbol_type
    make_INITIAL (const location_type& l);

    static inline
    symbol_type
    make_VIEWER (const location_type& l);

    static inline
    symbol_type
    make_RULER (const location_type& l);

    static inline
    symbol_type
    make_SYSTEM (const location_type& l);

    static inline
    symbol_type
    make_NODE (const location_type& l);

    static inline
    symbol_type
    make_ROOT (const location_type& l);

    static inline
    symbol_type
    make_EMERGENCE (const location_type& l);

    static inline
    symbol_type
    make_PROBLEM (const location_type& l);

    static inline
    symbol_type
    make_OBS (const location_type& l);

    static inline
    symbol_type
    make_COMMA (const location_type& l);

    static inline
    symbol_type
    make_COLON (const location_type& l);

    static inline
    symbol_type
    make_SEMI_COLON (const location_type& l);

    static inline
    symbol_type
    make_DOT (const location_type& l);

    static inline
    symbol_type
    make_EQUALS (const location_type& l);

    static inline
    symbol_type
    make_DOUBLE_EQUALS (const location_type& l);

    static inline
    symbol_type
    make_ARROW (const location_type& l);

    static inline
    symbol_type
    make_L_BRACKET (const location_type& l);

    static inline
    symbol_type
    make_R_BRACKET (const location_type& l);

    static inline
    symbol_type
    make_L_SQUARE_BRACKET (const location_type& l);

    static inline
    symbol_type
    make_R_SQUARE_BRACKET (const location_type& l);

    static inline
    symbol_type
    make_L_CURLY_BRACKET (const location_type& l);

    static inline
    symbol_type
    make_R_CURLY_BRACKET (const location_type& l);

    static inline
    symbol_type
    make_AND (const location_type& l);

    static inline
    symbol_type
    make_PIPE (const location_type& l);

    static inline
    symbol_type
    make_STAR (const location_type& l);

    static inline
    symbol_type
    make_PLUS (const location_type& l);

    static inline
    symbol_type
    make_OPT (const location_type& l);

    static inline
    symbol_type
    make_TILDE (const location_type& l);

    static inline
    symbol_type
    make_ID (const string& v, const location_type& l);


    /// Build a parser object.
    spec_parser (spec_driver& driver_yyarg);
    virtual ~spec_parser ();

    /// Parse.
    /// \returns  0 iff parsing succeeded.
    virtual int parse ();

#if YYDEBUG
    /// The current debugging stream.
    std::ostream& debug_stream () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging stream.
    void set_debug_stream (std::ostream &);

    /// Type for debugging levels.
    typedef int debug_level_type;
    /// The current debugging level.
    debug_level_type debug_level () const YY_ATTRIBUTE_PURE;
    /// Set the current debugging level.
    void set_debug_level (debug_level_type l);
#endif

    /// Report a syntax error.
    /// \param loc    where the syntax error is found.
    /// \param msg    a description of the syntax error.
    virtual void error (const location_type& loc, const std::string& msg);

    /// Report a syntax error.
    void error (const syntax_error& err);

  private:
    /// This class is not copyable.
    spec_parser (const spec_parser&);
    spec_parser& operator= (const spec_parser&);

    /// State numbers.
    typedef int state_type;

    /// Generate an error message.
    /// \param yystate   the state where the error occurred.
    /// \param yyla      the lookahead token.
    virtual std::string yysyntax_error_ (state_type yystate,
                                         const symbol_type& yyla) const;

    /// Compute post-reduction state.
    /// \param yystate   the current state
    /// \param yysym     the nonterminal to push on the stack
    state_type yy_lr_goto_state_ (state_type yystate, int yysym);

    /// Whether the given \c yypact_ value indicates a defaulted state.
    /// \param yyvalue   the value to check
    static bool yy_pact_value_is_default_ (int yyvalue);

    /// Whether the given \c yytable_ value indicates a syntax error.
    /// \param yyvalue   the value to check
    static bool yy_table_value_is_error_ (int yyvalue);

    static const short int yypact_ninf_;
    static const signed char yytable_ninf_;

    /// Convert a scanner token number \a t to a symbol number.
    static token_number_type yytranslate_ (token_type t);

    // Tables.
  // YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
  // STATE-NUM.
  static const short int yypact_[];

  // YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
  // Performed when YYTABLE does not specify something else to do.  Zero
  // means the default is an error.
  static const unsigned char yydefact_[];

  // YYPGOTO[NTERM-NUM].
  static const short int yypgoto_[];

  // YYDEFGOTO[NTERM-NUM].
  static const short int yydefgoto_[];

  // YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
  // positive, shift that token.  If negative, reduce the rule whose
  // number is the opposite.  If YYTABLE_NINF, syntax error.
  static const unsigned char yytable_[];

  static const short int yycheck_[];

  // YYSTOS[STATE-NUM] -- The (internal number of the) accessing
  // symbol of state STATE-NUM.
  static const unsigned char yystos_[];

  // YYR1[YYN] -- Symbol number of symbol that rule YYN derives.
  static const unsigned char yyr1_[];

  // YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.
  static const unsigned char yyr2_[];


    /// Convert the symbol name \a n to a form suitable for a diagnostic.
    static std::string yytnamerr_ (const char *n);


    /// For a symbol, its name in clear.
    static const char* const yytname_[];
#if YYDEBUG
  // YYRLINE[YYN] -- Source line where rule number YYN was defined.
  static const unsigned short int yyrline_[];
    /// Report on the debug stream that the rule \a r is going to be reduced.
    virtual void yy_reduce_print_ (int r);
    /// Print the state stack on the debug stream.
    virtual void yystack_print_ ();

    // Debugging.
    int yydebug_;
    std::ostream* yycdebug_;

    /// \brief Display a symbol type, value and location.
    /// \param yyo    The output stream.
    /// \param yysym  The symbol.
    template <typename Base>
    void yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const;
#endif

    /// \brief Reclaim the memory associated to a symbol.
    /// \param yymsg     Why this token is reclaimed.
    ///                  If null, print nothing.
    /// \param yysym     The symbol.
    template <typename Base>
    void yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const;

  private:
    /// Type access provider for state based symbols.
    struct by_state
    {
      /// Default constructor.
      by_state ();

      /// The symbol type as needed by the constructor.
      typedef state_type kind_type;

      /// Constructor.
      by_state (kind_type s);

      /// Copy constructor.
      by_state (const by_state& other);

      /// Record that this symbol is empty.
      void clear ();

      /// Steal the symbol type from \a that.
      void move (by_state& that);

      /// The (internal) type number (corresponding to \a state).
      /// \a empty_symbol when empty.
      symbol_number_type type_get () const;

      /// The state number used to denote an empty symbol.
      enum { empty_state = -1 };

      /// The state.
      /// \a empty when empty.
      state_type state;
    };

    /// "Internal" symbol: element of the stack.
    struct stack_symbol_type : basic_symbol<by_state>
    {
      /// Superclass.
      typedef basic_symbol<by_state> super_type;
      /// Construct an empty symbol.
      stack_symbol_type ();
      /// Steal the contents from \a sym to build this.
      stack_symbol_type (state_type s, symbol_type& sym);
      /// Assignment, needed by push_back.
      stack_symbol_type& operator= (const stack_symbol_type& that);
    };

    /// Stack type.
    typedef stack<stack_symbol_type> stack_type;

    /// The stack.
    stack_type yystack_;

    /// Push a new state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the symbol
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, stack_symbol_type& s);

    /// Push a new look ahead token on the state on the stack.
    /// \param m    a debug message to display
    ///             if null, no trace is output.
    /// \param s    the state
    /// \param sym  the symbol (for its value and location).
    /// \warning the contents of \a s.value is stolen.
    void yypush_ (const char* m, state_type s, symbol_type& sym);

    /// Pop \a n symbols the three stacks.
    void yypop_ (unsigned int n = 1);

    /// Constants.
    enum
    {
      yyeof_ = 0,
      yylast_ = 197,     ///< Last index in yytable_.
      yynnts_ = 48,  ///< Number of nonterminal symbols.
      yyfinal_ = 17, ///< Termination state number.
      yyterror_ = 1,
      yyerrcode_ = 256,
      yyntokens_ = 44  ///< Number of tokens.
    };


    // User arguments.
    spec_driver& driver;
  };

  // Symbol number corresponding to token number t.
  inline
  spec_parser::token_number_type
  spec_parser::yytranslate_ (token_type t)
  {
    static
    const token_number_type
    translate_table[] =
    {
     0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43
    };
    const unsigned int user_token_number_max_ = 298;
    const token_number_type undef_token_ = 2;

    if (static_cast<int>(t) <= yyeof_)
      return yyeof_;
    else if (static_cast<unsigned int> (t) <= user_token_number_max_)
      return translate_table[t];
    else
      return undef_token_;
  }

  inline
  spec_parser::syntax_error::syntax_error (const location_type& l, const std::string& m)
    : std::runtime_error (m)
    , location (l)
  {}

  // basic_symbol.
  template <typename Base>
  inline
  spec_parser::basic_symbol<Base>::basic_symbol ()
    : value ()
  {}

  template <typename Base>
  inline
  spec_parser::basic_symbol<Base>::basic_symbol (const basic_symbol& other)
    : Base (other)
    , value ()
    , location (other.location)
  {
      switch (other.type_get ())
    {
      case 49: // comp_model_decl
        value.copy< ComponentModel > (other.value);
        break;

      case 62: // net_model_decl
        value.copy< NetworkModel > (other.value);
        break;

      case 72: // pattern_decl
        value.copy< Pattern > (other.value);
        break;

      case 87: // problem_decl
        value.copy< Problem > (other.value);
        break;

      case 89: // problem_node
        value.copy< ProblemNode > (other.value);
        break;

      case 82: // system_decl
        value.copy< System > (other.value);
        break;

      case 84: // system_node
        value.copy< SystemNode > (other.value);
        break;

      case 57: // trans_decl
        value.copy< Transition > (other.value);
        break;

      case 73: // pattern_op
        value.copy< bool > (other.value);
        break;

      case 78: // viewer_section
      case 79: // map_list
      case 81: // ruler_section
        value.copy< map<pair<string,string>,string>   > (other.value);
        break;

      case 69: // link_decl
        value.copy< pair<pair<string,string>,pair<string,string> >  > (other.value);
        break;

      case 80: // map_decl
        value.copy< pair<pair<string,string>,string>  > (other.value);
        break;

      case 58: // event
      case 59: // ref
        value.copy< pair<string,string>  > (other.value);
        break;

      case 43: // "id"
      case 74: // expr
      case 75: // term
      case 76: // factor
      case 85: // root_section
        value.copy< string > (other.value);
        break;

      case 64: // component_section
      case 65: // decl_list
      case 66: // decl
        value.copy< vector<Component>  > (other.value);
        break;

      case 70: // pattern_section
      case 71: // pattern_list
        value.copy< vector<Pattern>  > (other.value);
        break;

      case 88: // problem_node_list
        value.copy< vector<ProblemNode>  > (other.value);
        break;

      case 83: // system_node_list
        value.copy< vector<SystemNode>  > (other.value);
        break;

      case 55: // transition_decl
      case 56: // trans_decl_list
        value.copy< vector<Transition>  > (other.value);
        break;

      case 67: // link_section
      case 68: // link_list
      case 86: // emergence_section
        value.copy< vector<pair<pair<string,string>,pair<string,string> > >  > (other.value);
        break;

      case 60: // opt_ref_list
      case 61: // ref_list
      case 77: // initial_section
        value.copy< vector<pair<string,string> >  > (other.value);
        break;

      case 50: // event_decl
      case 51: // id_list
      case 52: // input_decl
      case 53: // output_decl
      case 54: // state_decl
      case 90: // obs_section
      case 91: // opt_id_list
        value.copy< vector<string>  > (other.value);
        break;

      default:
        break;
    }

  }


  template <typename Base>
  inline
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const semantic_type& v, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {
    (void) v;
      switch (this->type_get ())
    {
      case 49: // comp_model_decl
        value.copy< ComponentModel > (v);
        break;

      case 62: // net_model_decl
        value.copy< NetworkModel > (v);
        break;

      case 72: // pattern_decl
        value.copy< Pattern > (v);
        break;

      case 87: // problem_decl
        value.copy< Problem > (v);
        break;

      case 89: // problem_node
        value.copy< ProblemNode > (v);
        break;

      case 82: // system_decl
        value.copy< System > (v);
        break;

      case 84: // system_node
        value.copy< SystemNode > (v);
        break;

      case 57: // trans_decl
        value.copy< Transition > (v);
        break;

      case 73: // pattern_op
        value.copy< bool > (v);
        break;

      case 78: // viewer_section
      case 79: // map_list
      case 81: // ruler_section
        value.copy< map<pair<string,string>,string>   > (v);
        break;

      case 69: // link_decl
        value.copy< pair<pair<string,string>,pair<string,string> >  > (v);
        break;

      case 80: // map_decl
        value.copy< pair<pair<string,string>,string>  > (v);
        break;

      case 58: // event
      case 59: // ref
        value.copy< pair<string,string>  > (v);
        break;

      case 43: // "id"
      case 74: // expr
      case 75: // term
      case 76: // factor
      case 85: // root_section
        value.copy< string > (v);
        break;

      case 64: // component_section
      case 65: // decl_list
      case 66: // decl
        value.copy< vector<Component>  > (v);
        break;

      case 70: // pattern_section
      case 71: // pattern_list
        value.copy< vector<Pattern>  > (v);
        break;

      case 88: // problem_node_list
        value.copy< vector<ProblemNode>  > (v);
        break;

      case 83: // system_node_list
        value.copy< vector<SystemNode>  > (v);
        break;

      case 55: // transition_decl
      case 56: // trans_decl_list
        value.copy< vector<Transition>  > (v);
        break;

      case 67: // link_section
      case 68: // link_list
      case 86: // emergence_section
        value.copy< vector<pair<pair<string,string>,pair<string,string> > >  > (v);
        break;

      case 60: // opt_ref_list
      case 61: // ref_list
      case 77: // initial_section
        value.copy< vector<pair<string,string> >  > (v);
        break;

      case 50: // event_decl
      case 51: // id_list
      case 52: // input_decl
      case 53: // output_decl
      case 54: // state_decl
      case 90: // obs_section
      case 91: // opt_id_list
        value.copy< vector<string>  > (v);
        break;

      default:
        break;
    }
}


  // Implementation of basic_symbol constructor for each type.

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const location_type& l)
    : Base (t)
    , value ()
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ComponentModel v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const NetworkModel v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Pattern v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Problem v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const ProblemNode v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const System v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const SystemNode v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const Transition v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const bool v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const map<pair<string, string>, string>   v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const pair<pair<string, string>, pair<string, string> >  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const pair<pair<string, string>, string>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const pair<string, string>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const string v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<Component>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<Pattern>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<ProblemNode>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<SystemNode>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<Transition>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<pair<pair<string, string>, pair<string, string> > >  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<pair<string, string> >  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}

  template <typename Base>
  spec_parser::basic_symbol<Base>::basic_symbol (typename Base::kind_type t, const vector<string>  v, const location_type& l)
    : Base (t)
    , value (v)
    , location (l)
  {}


  template <typename Base>
  inline
  spec_parser::basic_symbol<Base>::~basic_symbol ()
  {
    clear ();
  }

  template <typename Base>
  inline
  void
  spec_parser::basic_symbol<Base>::clear ()
  {
    // User destructor.
    symbol_number_type yytype = this->type_get ();
    basic_symbol<Base>& yysym = *this;
    (void) yysym;
    switch (yytype)
    {
   default:
      break;
    }

    // Type destructor.
    switch (yytype)
    {
      case 49: // comp_model_decl
        value.template destroy< ComponentModel > ();
        break;

      case 62: // net_model_decl
        value.template destroy< NetworkModel > ();
        break;

      case 72: // pattern_decl
        value.template destroy< Pattern > ();
        break;

      case 87: // problem_decl
        value.template destroy< Problem > ();
        break;

      case 89: // problem_node
        value.template destroy< ProblemNode > ();
        break;

      case 82: // system_decl
        value.template destroy< System > ();
        break;

      case 84: // system_node
        value.template destroy< SystemNode > ();
        break;

      case 57: // trans_decl
        value.template destroy< Transition > ();
        break;

      case 73: // pattern_op
        value.template destroy< bool > ();
        break;

      case 78: // viewer_section
      case 79: // map_list
      case 81: // ruler_section
        value.template destroy< map<pair<string,string>,string>   > ();
        break;

      case 69: // link_decl
        value.template destroy< pair<pair<string,string>,pair<string,string> >  > ();
        break;

      case 80: // map_decl
        value.template destroy< pair<pair<string,string>,string>  > ();
        break;

      case 58: // event
      case 59: // ref
        value.template destroy< pair<string,string>  > ();
        break;

      case 43: // "id"
      case 74: // expr
      case 75: // term
      case 76: // factor
      case 85: // root_section
        value.template destroy< string > ();
        break;

      case 64: // component_section
      case 65: // decl_list
      case 66: // decl
        value.template destroy< vector<Component>  > ();
        break;

      case 70: // pattern_section
      case 71: // pattern_list
        value.template destroy< vector<Pattern>  > ();
        break;

      case 88: // problem_node_list
        value.template destroy< vector<ProblemNode>  > ();
        break;

      case 83: // system_node_list
        value.template destroy< vector<SystemNode>  > ();
        break;

      case 55: // transition_decl
      case 56: // trans_decl_list
        value.template destroy< vector<Transition>  > ();
        break;

      case 67: // link_section
      case 68: // link_list
      case 86: // emergence_section
        value.template destroy< vector<pair<pair<string,string>,pair<string,string> > >  > ();
        break;

      case 60: // opt_ref_list
      case 61: // ref_list
      case 77: // initial_section
        value.template destroy< vector<pair<string,string> >  > ();
        break;

      case 50: // event_decl
      case 51: // id_list
      case 52: // input_decl
      case 53: // output_decl
      case 54: // state_decl
      case 90: // obs_section
      case 91: // opt_id_list
        value.template destroy< vector<string>  > ();
        break;

      default:
        break;
    }

    Base::clear ();
  }

  template <typename Base>
  inline
  bool
  spec_parser::basic_symbol<Base>::empty () const
  {
    return Base::type_get () == empty_symbol;
  }

  template <typename Base>
  inline
  void
  spec_parser::basic_symbol<Base>::move (basic_symbol& s)
  {
    super_type::move(s);
      switch (this->type_get ())
    {
      case 49: // comp_model_decl
        value.move< ComponentModel > (s.value);
        break;

      case 62: // net_model_decl
        value.move< NetworkModel > (s.value);
        break;

      case 72: // pattern_decl
        value.move< Pattern > (s.value);
        break;

      case 87: // problem_decl
        value.move< Problem > (s.value);
        break;

      case 89: // problem_node
        value.move< ProblemNode > (s.value);
        break;

      case 82: // system_decl
        value.move< System > (s.value);
        break;

      case 84: // system_node
        value.move< SystemNode > (s.value);
        break;

      case 57: // trans_decl
        value.move< Transition > (s.value);
        break;

      case 73: // pattern_op
        value.move< bool > (s.value);
        break;

      case 78: // viewer_section
      case 79: // map_list
      case 81: // ruler_section
        value.move< map<pair<string,string>,string>   > (s.value);
        break;

      case 69: // link_decl
        value.move< pair<pair<string,string>,pair<string,string> >  > (s.value);
        break;

      case 80: // map_decl
        value.move< pair<pair<string,string>,string>  > (s.value);
        break;

      case 58: // event
      case 59: // ref
        value.move< pair<string,string>  > (s.value);
        break;

      case 43: // "id"
      case 74: // expr
      case 75: // term
      case 76: // factor
      case 85: // root_section
        value.move< string > (s.value);
        break;

      case 64: // component_section
      case 65: // decl_list
      case 66: // decl
        value.move< vector<Component>  > (s.value);
        break;

      case 70: // pattern_section
      case 71: // pattern_list
        value.move< vector<Pattern>  > (s.value);
        break;

      case 88: // problem_node_list
        value.move< vector<ProblemNode>  > (s.value);
        break;

      case 83: // system_node_list
        value.move< vector<SystemNode>  > (s.value);
        break;

      case 55: // transition_decl
      case 56: // trans_decl_list
        value.move< vector<Transition>  > (s.value);
        break;

      case 67: // link_section
      case 68: // link_list
      case 86: // emergence_section
        value.move< vector<pair<pair<string,string>,pair<string,string> > >  > (s.value);
        break;

      case 60: // opt_ref_list
      case 61: // ref_list
      case 77: // initial_section
        value.move< vector<pair<string,string> >  > (s.value);
        break;

      case 50: // event_decl
      case 51: // id_list
      case 52: // input_decl
      case 53: // output_decl
      case 54: // state_decl
      case 90: // obs_section
      case 91: // opt_id_list
        value.move< vector<string>  > (s.value);
        break;

      default:
        break;
    }

    location = s.location;
  }

  // by_type.
  inline
  spec_parser::by_type::by_type ()
    : type (empty_symbol)
  {}

  inline
  spec_parser::by_type::by_type (const by_type& other)
    : type (other.type)
  {}

  inline
  spec_parser::by_type::by_type (token_type t)
    : type (yytranslate_ (t))
  {}

  inline
  void
  spec_parser::by_type::clear ()
  {
    type = empty_symbol;
  }

  inline
  void
  spec_parser::by_type::move (by_type& that)
  {
    type = that.type;
    that.clear ();
  }

  inline
  int
  spec_parser::by_type::type_get () const
  {
    return type;
  }

  inline
  spec_parser::token_type
  spec_parser::by_type::token () const
  {
    // YYTOKNUM[NUM] -- (External) token number corresponding to the
    // (internal) symbol number NUM (which must be that of a token).  */
    static
    const unsigned short int
    yytoken_number_[] =
    {
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298
    };
    return static_cast<token_type> (yytoken_number_[type]);
  }
  // Implementation of make_symbol for each symbol type.
  spec_parser::symbol_type
  spec_parser::make_END_OF_FILE (const location_type& l)
  {
    return symbol_type (token::END_OF_FILE, l);
  }

  spec_parser::symbol_type
  spec_parser::make_COMPONENT (const location_type& l)
  {
    return symbol_type (token::COMPONENT, l);
  }

  spec_parser::symbol_type
  spec_parser::make_MODEL (const location_type& l)
  {
    return symbol_type (token::MODEL, l);
  }

  spec_parser::symbol_type
  spec_parser::make_IS (const location_type& l)
  {
    return symbol_type (token::IS, l);
  }

  spec_parser::symbol_type
  spec_parser::make_END (const location_type& l)
  {
    return symbol_type (token::END, l);
  }

  spec_parser::symbol_type
  spec_parser::make_EVENT (const location_type& l)
  {
    return symbol_type (token::EVENT, l);
  }

  spec_parser::symbol_type
  spec_parser::make_INPUT (const location_type& l)
  {
    return symbol_type (token::INPUT, l);
  }

  spec_parser::symbol_type
  spec_parser::make_OUTPUT (const location_type& l)
  {
    return symbol_type (token::OUTPUT, l);
  }

  spec_parser::symbol_type
  spec_parser::make_STATE (const location_type& l)
  {
    return symbol_type (token::STATE, l);
  }

  spec_parser::symbol_type
  spec_parser::make_TRANSITION (const location_type& l)
  {
    return symbol_type (token::TRANSITION, l);
  }

  spec_parser::symbol_type
  spec_parser::make_NETWORK (const location_type& l)
  {
    return symbol_type (token::NETWORK, l);
  }

  spec_parser::symbol_type
  spec_parser::make_LINK (const location_type& l)
  {
    return symbol_type (token::LINK, l);
  }

  spec_parser::symbol_type
  spec_parser::make_PATTERN (const location_type& l)
  {
    return symbol_type (token::PATTERN, l);
  }

  spec_parser::symbol_type
  spec_parser::make_INITIAL (const location_type& l)
  {
    return symbol_type (token::INITIAL, l);
  }

  spec_parser::symbol_type
  spec_parser::make_VIEWER (const location_type& l)
  {
    return symbol_type (token::VIEWER, l);
  }

  spec_parser::symbol_type
  spec_parser::make_RULER (const location_type& l)
  {
    return symbol_type (token::RULER, l);
  }

  spec_parser::symbol_type
  spec_parser::make_SYSTEM (const location_type& l)
  {
    return symbol_type (token::SYSTEM, l);
  }

  spec_parser::symbol_type
  spec_parser::make_NODE (const location_type& l)
  {
    return symbol_type (token::NODE, l);
  }

  spec_parser::symbol_type
  spec_parser::make_ROOT (const location_type& l)
  {
    return symbol_type (token::ROOT, l);
  }

  spec_parser::symbol_type
  spec_parser::make_EMERGENCE (const location_type& l)
  {
    return symbol_type (token::EMERGENCE, l);
  }

  spec_parser::symbol_type
  spec_parser::make_PROBLEM (const location_type& l)
  {
    return symbol_type (token::PROBLEM, l);
  }

  spec_parser::symbol_type
  spec_parser::make_OBS (const location_type& l)
  {
    return symbol_type (token::OBS, l);
  }

  spec_parser::symbol_type
  spec_parser::make_COMMA (const location_type& l)
  {
    return symbol_type (token::COMMA, l);
  }

  spec_parser::symbol_type
  spec_parser::make_COLON (const location_type& l)
  {
    return symbol_type (token::COLON, l);
  }

  spec_parser::symbol_type
  spec_parser::make_SEMI_COLON (const location_type& l)
  {
    return symbol_type (token::SEMI_COLON, l);
  }

  spec_parser::symbol_type
  spec_parser::make_DOT (const location_type& l)
  {
    return symbol_type (token::DOT, l);
  }

  spec_parser::symbol_type
  spec_parser::make_EQUALS (const location_type& l)
  {
    return symbol_type (token::EQUALS, l);
  }

  spec_parser::symbol_type
  spec_parser::make_DOUBLE_EQUALS (const location_type& l)
  {
    return symbol_type (token::DOUBLE_EQUALS, l);
  }

  spec_parser::symbol_type
  spec_parser::make_ARROW (const location_type& l)
  {
    return symbol_type (token::ARROW, l);
  }

  spec_parser::symbol_type
  spec_parser::make_L_BRACKET (const location_type& l)
  {
    return symbol_type (token::L_BRACKET, l);
  }

  spec_parser::symbol_type
  spec_parser::make_R_BRACKET (const location_type& l)
  {
    return symbol_type (token::R_BRACKET, l);
  }

  spec_parser::symbol_type
  spec_parser::make_L_SQUARE_BRACKET (const location_type& l)
  {
    return symbol_type (token::L_SQUARE_BRACKET, l);
  }

  spec_parser::symbol_type
  spec_parser::make_R_SQUARE_BRACKET (const location_type& l)
  {
    return symbol_type (token::R_SQUARE_BRACKET, l);
  }

  spec_parser::symbol_type
  spec_parser::make_L_CURLY_BRACKET (const location_type& l)
  {
    return symbol_type (token::L_CURLY_BRACKET, l);
  }

  spec_parser::symbol_type
  spec_parser::make_R_CURLY_BRACKET (const location_type& l)
  {
    return symbol_type (token::R_CURLY_BRACKET, l);
  }

  spec_parser::symbol_type
  spec_parser::make_AND (const location_type& l)
  {
    return symbol_type (token::AND, l);
  }

  spec_parser::symbol_type
  spec_parser::make_PIPE (const location_type& l)
  {
    return symbol_type (token::PIPE, l);
  }

  spec_parser::symbol_type
  spec_parser::make_STAR (const location_type& l)
  {
    return symbol_type (token::STAR, l);
  }

  spec_parser::symbol_type
  spec_parser::make_PLUS (const location_type& l)
  {
    return symbol_type (token::PLUS, l);
  }

  spec_parser::symbol_type
  spec_parser::make_OPT (const location_type& l)
  {
    return symbol_type (token::OPT, l);
  }

  spec_parser::symbol_type
  spec_parser::make_TILDE (const location_type& l)
  {
    return symbol_type (token::TILDE, l);
  }

  spec_parser::symbol_type
  spec_parser::make_ID (const string& v, const location_type& l)
  {
    return symbol_type (token::ID, v, l);
  }



} // yy
#line 2043 "/home/giulio/Scrivania/Tesi/SpecificationLanguage/specparser.hpp" // lalr1.cc:377




#endif // !YY_YY_HOME_GIULIO_SCRIVANIA_TESI_SPECIFICATIONLANGUAGE_SPECPARSER_HPP_INCLUDED

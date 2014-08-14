/* A Bison parser, made by GNU Bison 2.7.  */

/* Skeleton implementation for Bison LALR(1) parsers in C++
   
      Copyright (C) 2002-2012 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* First part of user declarations.  */
/* Line 279 of lalr1.cc  */
#line 11 "parser.y"


/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
//#include "Math_Lexer.h" // for yylex
#include <J_Symbol_Fwd_Decl.h>

#include "parser.h"
#include "Math_Parser.h"
#include <Constant_Symbol.h>
#include <cstdarg>


using namespace jomike;
void yyerror(const char *msg); // standard error-handling routine

j_symbol_component* jtl::g_input_line = nullptr;

//template<typename... Args>
//void delete_tokens(Args... i_ptrs){
//	j_symbol_component* pointers[] = {i_ptrs...};
//#ifdef WIN32
//	for(auto f_pointer : pointers){
//		delete f_pointer;
//	}
//#else
//	int size = safe_int_cast(sizeof(pointers)/sizeof(Syntax_Tree_Component*));
//	for(int i=0; i < size; i++){
//		delete pointers[i];
//	}
//#endif
//
//}


/* Line 279 of lalr1.cc  */
#line 78 "math_parsing_unit.cc"


#include "math_parsing_unit.hh"

/* User implementation prologue.  */

/* Line 285 of lalr1.cc  */
#line 86 "math_parsing_unit.cc"


# ifndef YY_NULL
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULL nullptr
#  else
#   define YY_NULL 0
#  endif
# endif

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* FIXME: INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
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


/* Suppress unused-variable warnings by "using" E.  */
#define YYUSE(e) ((void) (e))

/* Enable debugging if requested.  */
#if YYDEBUG

/* A pseudo ostream that takes yydebug_ into account.  */
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)	\
do {							\
  if (yydebug_)						\
    {							\
      *yycdebug_ << Title << ' ';			\
      yy_symbol_print_ ((Type), (Value), (Location));	\
      *yycdebug_ << std::endl;				\
    }							\
} while (false)

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug_)				\
    yy_reduce_print_ (Rule);		\
} while (false)

# define YY_STACK_PRINT()		\
do {					\
  if (yydebug_)				\
    yystack_print_ ();			\
} while (false)

#else /* !YYDEBUG */

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Type, Value, Location) YYUSE(Type)
# define YY_REDUCE_PRINT(Rule)        static_cast<void>(0)
# define YY_STACK_PRINT()             static_cast<void>(0)

#endif /* !YYDEBUG */

#define yyerrok		(yyerrstatus_ = 0)
#define yyclearin	(yychar = yyempty_)

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
/* Line 353 of lalr1.cc  */
#line 181 "math_parsing_unit.cc"

  /// Build a parser object.
  Math_Parsing_Unit::Math_Parsing_Unit (jtl::j_symbol** i_symbol_ptr_yyarg, jtl::Math_Parser* i_parser_yyarg)
    :
#if YYDEBUG
      yydebug_ (false),
      yycdebug_ (&std::cerr),
#endif
      i_symbol_ptr (i_symbol_ptr_yyarg),
      i_parser (i_parser_yyarg)
  {
  }

  Math_Parsing_Unit::~Math_Parsing_Unit ()
  {
  }

#if YYDEBUG
  /*--------------------------------.
  | Print this symbol on YYOUTPUT.  |
  `--------------------------------*/

  inline void
  Math_Parsing_Unit::yy_symbol_value_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yyvaluep);
    std::ostream& yyo = debug_stream ();
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    switch (yytype)
      {
         default:
	  break;
      }
  }


  void
  Math_Parsing_Unit::yy_symbol_print_ (int yytype,
			   const semantic_type* yyvaluep, const location_type* yylocationp)
  {
    *yycdebug_ << (yytype < yyntokens_ ? "token" : "nterm")
	       << ' ' << yytname_[yytype] << " ("
	       << *yylocationp << ": ";
    yy_symbol_value_print_ (yytype, yyvaluep, yylocationp);
    *yycdebug_ << ')';
  }
#endif

  void
  Math_Parsing_Unit::yydestruct_ (const char* yymsg,
			   int yytype, semantic_type* yyvaluep, location_type* yylocationp)
  {
    YYUSE (yylocationp);
    YYUSE (yymsg);
    YYUSE (yyvaluep);

    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

    switch (yytype)
      {
  
	default:
	  break;
      }
  }

  void
  Math_Parsing_Unit::yypop_ (unsigned int n)
  {
    yystate_stack_.pop (n);
    yysemantic_stack_.pop (n);
    yylocation_stack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  Math_Parsing_Unit::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  Math_Parsing_Unit::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  Math_Parsing_Unit::debug_level_type
  Math_Parsing_Unit::debug_level () const
  {
    return yydebug_;
  }

  void
  Math_Parsing_Unit::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif

  inline bool
  Math_Parsing_Unit::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  inline bool
  Math_Parsing_Unit::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  Math_Parsing_Unit::parse ()
  {
    /// Lookahead and lookahead in internal form.
    int yychar = yyempty_;
    int yytoken = 0;

    // State.
    int yyn;
    int yylen = 0;
    int yystate = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// Semantic value of the lookahead.
    static semantic_type yyval_default;
    semantic_type yylval = yyval_default;
    /// Location of the lookahead.
    location_type yylloc;
    /// The locations where the error started and ended.
    location_type yyerror_range[3];

    /// $$.
    semantic_type yyval;
    /// @$.
    location_type yyloc;

    int yyresult;

    // FIXME: This shoud be completely indented.  It is not yet to
    // avoid gratuitous conflicts when merging into the master branch.
    try
      {
    YYCDEBUG << "Starting parse" << std::endl;


    /* Initialize the stacks.  The initial state will be pushed in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystate_stack_ = state_stack_type (0);
    yysemantic_stack_ = semantic_stack_type (0);
    yylocation_stack_ = location_stack_type (0);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* New state.  */
  yynewstate:
    yystate_stack_.push (yystate);
    YYCDEBUG << "Entering state " << yystate << std::endl;

    /* Accept?  */
    if (yystate == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    /* Backup.  */
  yybackup:

    /* Try to take a decision without lookahead.  */
    yyn = yypact_[yystate];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    /* Read a lookahead token.  */
    if (yychar == yyempty_)
      {
        YYCDEBUG << "Reading a token: ";
        yychar = yylex (&yylval, i_parser);
      }

    /* Convert token to internal form.  */
    if (yychar <= yyeof_)
      {
	yychar = yytoken = yyeof_;
	YYCDEBUG << "Now at end of input." << std::endl;
      }
    else
      {
	yytoken = yytranslate_ (yychar);
	YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
      }

    /* If the proper action on seeing token YYTOKEN is to reduce or to
       detect an error, take that action.  */
    yyn += yytoken;
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yytoken)
      goto yydefault;

    /* Reduce or error.  */
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
	if (yy_table_value_is_error_ (yyn))
	  goto yyerrlab;
	yyn = -yyn;
	goto yyreduce;
      }

    /* Shift the lookahead token.  */
    YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

    /* Discard the token being shifted.  */
    yychar = yyempty_;

    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yylloc);

    /* Count tokens shifted since error; after three, turn off error
       status.  */
    if (yyerrstatus_)
      --yyerrstatus_;

    yystate = yyn;
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystate];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    /* If YYLEN is nonzero, implement the default value of the action:
       `$$ = $1'.  Otherwise, use the top of the stack.

       Otherwise, the following line sets YYVAL to garbage.
       This behavior is undocumented and Bison
       users should not rely upon it.  */
    if (yylen)
      yyval = yysemantic_stack_[yylen - 1];
    else
      yyval = yysemantic_stack_[0];

    // Compute the default @$.
    {
      slice<location_type, location_stack_type> slice (yylocation_stack_, yylen);
      YYLLOC_DEFAULT (yyloc, slice, yylen);
    }

    // Perform the reduction.
    YY_REDUCE_PRINT (yyn);
    switch (yyn)
      {
          case 2:
/* Line 670 of lalr1.cc  */
#line 176 "parser.y"
    {
	(yyval.symbol) = (yysemantic_stack_[(2) - (1)].expression);
	*i_symbol_ptr = (yysemantic_stack_[(2) - (1)].expression);
	return true;
}
    break;

  case 3:
/* Line 670 of lalr1.cc  */
#line 181 "parser.y"
    {
	(yyval.symbol) = (yysemantic_stack_[(2) - (1)].declaration);
	*i_symbol_ptr = (yysemantic_stack_[(2) - (1)].declaration)->get_copy();
	add_user_symbol((yysemantic_stack_[(2) - (1)].declaration));
	return true;
}
    break;

  case 4:
/* Line 670 of lalr1.cc  */
#line 189 "parser.y"
    {(yyval.symbol) = (yysemantic_stack_[(1) - (1)].symbol);}
    break;

  case 5:
/* Line 670 of lalr1.cc  */
#line 194 "parser.y"
    {
	(yyval.symbol) = (yysemantic_stack_[(2) - (1)].expression);
	
	

}
    break;

  case 6:
/* Line 670 of lalr1.cc  */
#line 200 "parser.y"
    {
	(yyval.symbol) = (yysemantic_stack_[(2) - (1)].declaration);
}
    break;

  case 7:
/* Line 670 of lalr1.cc  */
#line 206 "parser.y"
    {
(yyval.symbol_list) = new Specific_Symbol_List<j_symbol>();
}
    break;

  case 8:
/* Line 670 of lalr1.cc  */
#line 209 "parser.y"
    {
	(yyval.symbol_list) = (yysemantic_stack_[(2) - (1)].symbol_list);
	(yyval.symbol_list)->add_symbol((yysemantic_stack_[(2) - (2)].symbol));
}
    break;

  case 9:
/* Line 670 of lalr1.cc  */
#line 215 "parser.y"
    {
	(yyval.declaration_list) = new Specific_Symbol_List<j_declaration>;
}
    break;

  case 10:
/* Line 670 of lalr1.cc  */
#line 218 "parser.y"
    {
	(yyval.declaration_list) = new Specific_Symbol_List<j_declaration>();
	(yyval.declaration_list)->add_symbol((yysemantic_stack_[(1) - (1)].declaration));
}
    break;

  case 11:
/* Line 670 of lalr1.cc  */
#line 222 "parser.y"
    {
	(yyval.declaration_list) = (yysemantic_stack_[(3) - (1)].declaration_list);
	(yyval.declaration_list)->add_symbol((yysemantic_stack_[(3) - (3)].declaration));
}
    break;

  case 12:
/* Line 670 of lalr1.cc  */
#line 229 "parser.y"
    {(yyval.declaration) = (yysemantic_stack_[(1) - (1)].declaration);}
    break;

  case 13:
/* Line 670 of lalr1.cc  */
#line 230 "parser.y"
    { 
	(yyval.declaration) = (yysemantic_stack_[(1) - (1)].declaration); 
}
    break;

  case 14:
/* Line 670 of lalr1.cc  */
#line 236 "parser.y"
    {
	(yyval.declaration) = new Custom_Routine_Symbol((yysemantic_stack_[(8) - (2)].identifier), *(yysemantic_stack_[(8) - (1)].declaration_list), *(yysemantic_stack_[(8) - (3)].declaration_list), (yysemantic_stack_[(8) - (5)].type_syntax), (yysemantic_stack_[(8) - (7)].symbol_list));
	(yysemantic_stack_[(8) - (1)].declaration_list).destroy();
	(yysemantic_stack_[(8) - (3)].declaration_list).destroy();
}
    break;

  case 15:
/* Line 670 of lalr1.cc  */
#line 244 "parser.y"
    {
	(yyval.declaration_list) = (yysemantic_stack_[(3) - (2)].declaration_list);
}
    break;

  case 16:
/* Line 670 of lalr1.cc  */
#line 251 "parser.y"
    {(yyval.declaration) =  new Variable_Symbol((yysemantic_stack_[(2) - (1)].type_syntax), (yysemantic_stack_[(2) - (2)].identifier));
	
	
}
    break;

  case 17:
/* Line 670 of lalr1.cc  */
#line 255 "parser.y"
    {
	(yyval.declaration) = new Variable_Reference_Symbol((yysemantic_stack_[(4) - (1)].type_syntax), (yysemantic_stack_[(4) - (2)].identifier), (yysemantic_stack_[(4) - (4)].expression)); 
	
	
	
}
    break;

  case 18:
/* Line 670 of lalr1.cc  */
#line 261 "parser.y"
    {
	(yyval.declaration) = new Variable_Symbol((yysemantic_stack_[(4) - (1)].type_syntax), (yysemantic_stack_[(4) - (2)].identifier), *(yysemantic_stack_[(4) - (4)].expression));
	
	
	(yysemantic_stack_[(4) - (4)].expression).destroy();
	
}
    break;

  case 19:
/* Line 670 of lalr1.cc  */
#line 271 "parser.y"
    {(yyval.type_syntax) = make_double_type_syntax();}
    break;

  case 20:
/* Line 670 of lalr1.cc  */
#line 272 "parser.y"
    {(yyval.type_syntax) = make_int_type_syntax();}
    break;

  case 21:
/* Line 670 of lalr1.cc  */
#line 273 "parser.y"
    {(yyval.type_syntax) = make_bool_type_syntax();}
    break;

  case 22:
/* Line 670 of lalr1.cc  */
#line 274 "parser.y"
    {(yyval.type_syntax) = make_void_type_syntax(); }
    break;

  case 23:
/* Line 670 of lalr1.cc  */
#line 278 "parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
	
}
    break;

  case 24:
/* Line 670 of lalr1.cc  */
#line 282 "parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
	
}
    break;

  case 25:
/* Line 670 of lalr1.cc  */
#line 286 "parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].constant_symbol); 
}
    break;

  case 26:
/* Line 670 of lalr1.cc  */
#line 289 "parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
	
}
    break;

  case 27:
/* Line 670 of lalr1.cc  */
#line 293 "parser.y"
    { 
	(yyval.expression) = new Addition_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
}
    break;

  case 28:
/* Line 670 of lalr1.cc  */
#line 296 "parser.y"
    { 
	(yyval.expression) = new Subtraction_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
	
	
}
    break;

  case 29:
/* Line 670 of lalr1.cc  */
#line 301 "parser.y"
    { 
	(yyval.expression) = new Multiplication_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
	
	
}
    break;

  case 30:
/* Line 670 of lalr1.cc  */
#line 306 "parser.y"
    { 
	(yyval.expression) = new Division_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
	
	
}
    break;

  case 31:
/* Line 670 of lalr1.cc  */
#line 311 "parser.y"
    {(yyval.expression) = (yysemantic_stack_[(3) - (2)].expression);  }
    break;

  case 32:
/* Line 670 of lalr1.cc  */
#line 312 "parser.y"
    {
	(yyval.expression) = new Unary_Negate_Expression((yysemantic_stack_[(2) - (2)].expression));
}
    break;

  case 33:
/* Line 670 of lalr1.cc  */
#line 318 "parser.y"
    {
	(yyval.expression) = new Assignment_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
	
}
    break;

  case 34:
/* Line 670 of lalr1.cc  */
#line 330 "parser.y"
    {(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 35:
/* Line 670 of lalr1.cc  */
#line 333 "parser.y"
    {
	(yyval.expression) = new Field_Access_Expression((yysemantic_stack_[(1) - (1)].identifier));
	
}
    break;

  case 36:
/* Line 670 of lalr1.cc  */
#line 344 "parser.y"
    {
	(yyval.expression) = new Call_Expression((yysemantic_stack_[(4) - (1)].identifier), (yysemantic_stack_[(4) - (3)].arguments));
	
	
}
    break;

  case 37:
/* Line 670 of lalr1.cc  */
#line 357 "parser.y"
    {(yyval.arguments) = new Arguments;}
    break;

  case 38:
/* Line 670 of lalr1.cc  */
#line 358 "parser.y"
    {(yyval.arguments) = (yysemantic_stack_[(1) - (1)].arguments);}
    break;

  case 39:
/* Line 670 of lalr1.cc  */
#line 362 "parser.y"
    {
	(yyval.arguments) = (yysemantic_stack_[(1) - (1)].arguments);
	
}
    break;

  case 40:
/* Line 670 of lalr1.cc  */
#line 368 "parser.y"
    {
	(yyval.arguments) = new Arguments;
	(yyval.arguments)->push_back((yysemantic_stack_[(1) - (1)].expression));
	
}
    break;

  case 41:
/* Line 670 of lalr1.cc  */
#line 373 "parser.y"
    {
	(yyval.arguments) = (yysemantic_stack_[(3) - (1)].arguments);
	
	(yyval.arguments)->push_back((yysemantic_stack_[(3) - (3)].expression));
	
}
    break;

  case 42:
/* Line 670 of lalr1.cc  */
#line 382 "parser.y"
    {
	(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol);
}
    break;

  case 43:
/* Line 670 of lalr1.cc  */
#line 385 "parser.y"
    {(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol);}
    break;

  case 44:
/* Line 670 of lalr1.cc  */
#line 386 "parser.y"
    {(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol);  }
    break;

  case 45:
/* Line 670 of lalr1.cc  */
#line 387 "parser.y"
    {(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol); }
    break;


/* Line 670 of lalr1.cc  */
#line 821 "math_parsing_unit.cc"
      default:
        break;
      }

    /* User semantic actions sometimes alter yychar, and that requires
       that yytoken be updated with the new translation.  We take the
       approach of translating immediately before every use of yytoken.
       One alternative is translating here after every semantic action,
       but that translation would be missed if the semantic action
       invokes YYABORT, YYACCEPT, or YYERROR immediately after altering
       yychar.  In the case of YYABORT or YYACCEPT, an incorrect
       destructor might then be invoked immediately.  In the case of
       YYERROR, subsequent parser actions might lead to an incorrect
       destructor call or verbose syntax error message before the
       lookahead is translated.  */
    YY_SYMBOL_PRINT ("-> $$ =", yyr1_[yyn], &yyval, &yyloc);

    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();

    yysemantic_stack_.push (yyval);
    yylocation_stack_.push (yyloc);

    /* Shift the result of the reduction.  */
    yyn = yyr1_[yyn];
    yystate = yypgoto_[yyn - yyntokens_] + yystate_stack_[0];
    if (0 <= yystate && yystate <= yylast_
	&& yycheck_[yystate] == yystate_stack_[0])
      yystate = yytable_[yystate];
    else
      yystate = yydefgoto_[yyn - yyntokens_];
    goto yynewstate;

  /*------------------------------------.
  | yyerrlab -- here on detecting error |
  `------------------------------------*/
  yyerrlab:
    /* Make sure we have latest lookahead translation.  See comments at
       user semantic actions for why this is necessary.  */
    yytoken = yytranslate_ (yychar);

    /* If not already recovering from an error, report this error.  */
    if (!yyerrstatus_)
      {
	++yynerrs_;
	if (yychar == yyempty_)
	  yytoken = yyempty_;
	error (yylloc, yysyntax_error_ (yystate, yytoken));
      }

    yyerror_range[1] = yylloc;
    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */
        if (yychar <= yyeof_)
          {
            /* Return failure if at end of input.  */
            if (yychar == yyeof_)
              YYABORT;
          }
        else
          {
            yydestruct_ ("Error: discarding", yytoken, &yylval, &yylloc);
            yychar = yyempty_;
          }
      }

    /* Else will try to reuse lookahead token after shifting the error
       token.  */
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

    yyerror_range[1] = yylocation_stack_[yylen - 1];
    /* Do not reclaim the symbols of the rule which action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    yystate = yystate_stack_[0];
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;	/* Each real token shifted decrements this.  */

    for (;;)
      {
	yyn = yypact_[yystate];
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

	/* Pop the current state because it cannot handle the error token.  */
	if (yystate_stack_.height () == 1)
	  YYABORT;

	yyerror_range[1] = yylocation_stack_[0];
	yydestruct_ ("Error: popping",
		     yystos_[yystate],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);
	yypop_ ();
	yystate = yystate_stack_[0];
	YY_STACK_PRINT ();
      }

    yyerror_range[2] = yylloc;
    // Using YYLLOC is tempting, but would change the location of
    // the lookahead.  YYLOC is available though.
    YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
    yysemantic_stack_.push (yylval);
    yylocation_stack_.push (yyloc);

    /* Shift the error token.  */
    YY_SYMBOL_PRINT ("Shifting", yystos_[yyn],
		     &yysemantic_stack_[0], &yylocation_stack_[0]);

    yystate = yyn;
    goto yynewstate;

    /* Accept.  */
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    /* Abort.  */
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (yychar != yyempty_)
      {
        /* Make sure we have latest lookahead translation.  See comments
           at user semantic actions for why this is necessary.  */
        yytoken = yytranslate_ (yychar);
        yydestruct_ ("Cleanup: discarding lookahead", yytoken, &yylval,
                     &yylloc);
      }

    /* Do not reclaim the symbols of the rule which action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystate_stack_.height ())
      {
        yydestruct_ ("Cleanup: popping",
                     yystos_[yystate_stack_[0]],
                     &yysemantic_stack_[0],
                     &yylocation_stack_[0]);
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
        if (yychar != yyempty_)
          {
            /* Make sure we have latest lookahead translation.  See
               comments at user semantic actions for why this is
               necessary.  */
            yytoken = yytranslate_ (yychar);
            yydestruct_ (YY_NULL, yytoken, &yylval, &yylloc);
          }

        while (1 < yystate_stack_.height ())
          {
            yydestruct_ (YY_NULL,
                         yystos_[yystate_stack_[0]],
                         &yysemantic_stack_[0],
                         &yylocation_stack_[0]);
            yypop_ ();
          }
        throw;
      }
  }

  // Generate an error message.
  std::string
  Math_Parsing_Unit::yysyntax_error_ (int, int)
  {
    return YY_("syntax error");
  }


  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
  const signed char Math_Parsing_Unit::yypact_ninf_ = -42;
  const signed char
  Math_Parsing_Unit::yypact_[] =
  {
        46,   -42,   -42,   -42,   -42,   -41,   -42,   -42,   -42,   -42,
      51,     1,    51,     8,     3,   -42,     5,   -42,    22,    64,
     -42,    24,   -42,   -42,   -42,    51,    -4,   -23,   -42,    43,
     -42,   -42,     0,    21,   -42,    51,    51,    51,    51,    51,
      57,    -3,   -42,    14,   -42,     1,   -42,    47,    51,    51,
      -4,    -4,   -42,   -42,    57,   -42,    51,   -42,    91,    57,
      57,    57,    33,   -42,     6,   -42,   -42,   -42,    36,    17,
     -42,   -42
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Math_Parsing_Unit::yydefact_[] =
  {
         0,    22,    21,    20,    19,    35,    45,    42,    43,    44,
       0,     9,     0,     0,     0,    13,     0,    12,     0,     0,
      23,    26,    34,    24,    25,    37,    32,     0,    10,     0,
       1,     3,     0,    16,     2,     0,     0,     0,     0,     0,
      40,     0,    38,    39,    15,     0,    31,     0,     0,     0,
      27,    28,    29,    30,    33,    36,     0,    11,     0,    18,
      17,    41,     0,     7,     0,    14,     8,     4,     0,     0,
       6,     5
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Math_Parsing_Unit::yypgoto_[] =
  {
       -42,   -42,   -42,   -42,   -42,   -42,    -8,   -42,    44,   -42,
      20,   -10,   -42,   -42,   -42,   -42,   -42,   -42,   -42,   -42
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  Math_Parsing_Unit::yydefgoto_[] =
  {
        -1,    13,    66,    67,    64,    27,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    41,    42,    43,    24
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Math_Parsing_Unit::yytable_ninf_ = -1;
  const unsigned char
  Math_Parsing_Unit::yytable_[] =
  {
        26,    25,    29,    28,     1,     2,     3,     4,    30,     1,
       2,     3,     4,    44,    31,    40,    45,    32,     5,     6,
       7,     8,     9,    37,    38,    50,    51,    52,    53,    54,
      48,    49,    10,    39,    33,    11,    11,    57,    59,    60,
      55,    11,    35,    36,    37,    38,    61,    65,    12,     1,
       2,     3,     4,    56,    69,    71,    68,    58,     5,     6,
       7,     8,     9,     5,     6,     7,     8,     9,    35,    36,
      37,    38,    10,    63,    70,    34,    47,    10,    62,     0,
       0,    11,    35,    36,    37,    38,    46,     0,    12,    35,
      36,    37,    38,    12,     1,     2,     3,     4
  };

  /* YYCHECK.  */
  const signed char
  Math_Parsing_Unit::yycheck_[] =
  {
        10,    42,    12,    11,     3,     4,     5,     6,     0,     3,
       4,     5,     6,    36,    11,    25,    39,    12,    12,    13,
      14,    15,    16,    27,    28,    35,    36,    37,    38,    39,
       9,    10,    26,     9,    12,    35,    35,    45,    48,    49,
      43,    35,    25,    26,    27,    28,    56,    41,    42,     3,
       4,     5,     6,    39,    64,    38,    64,    10,    12,    13,
      14,    15,    16,    12,    13,    14,    15,    16,    25,    26,
      27,    28,    26,    40,    38,    11,    32,    26,    58,    -1,
      -1,    35,    25,    26,    27,    28,    43,    -1,    42,    25,
      26,    27,    28,    42,     3,     4,     5,     6
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Math_Parsing_Unit::yystos_[] =
  {
         0,     3,     4,     5,     6,    12,    13,    14,    15,    16,
      26,    35,    42,    45,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    63,    42,    55,    49,    50,    55,
       0,    11,    12,    12,    11,    25,    26,    27,    28,     9,
      55,    60,    61,    62,    36,    39,    43,    52,     9,    10,
      55,    55,    55,    55,    55,    43,    39,    50,    10,    55,
      55,    55,    54,    40,    48,    41,    46,    47,    50,    55,
      38,    38
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Math_Parsing_Unit::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,    62,    60,   276,   277,    43,    45,    42,    47,    37,
      33,   278,   279,   280,    46,    91,    93,   281,    59,    44,
     123,   125,    40,    41
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Math_Parsing_Unit::yyr1_[] =
  {
         0,    44,    45,    45,    46,    47,    47,    48,    48,    49,
      49,    49,    50,    50,    51,    52,    53,    53,    53,    54,
      54,    54,    54,    55,    55,    55,    55,    55,    55,    55,
      55,    55,    55,    56,    57,    58,    59,    60,    60,    61,
      62,    62,    63,    63,    63,    63
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Math_Parsing_Unit::yyr2_[] =
  {
         0,     2,     2,     2,     1,     2,     2,     0,     2,     0,
       1,     3,     1,     1,     8,     3,     2,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     3,     3,
       3,     3,     2,     3,     1,     1,     4,     0,     1,     1,
       1,     3,     1,     1,     1,     1
  };

#if YYDEBUG
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Math_Parsing_Unit::yytname_[] =
  {
    "$end", "error", "$undefined", "T_VOID", "T_BOOL", "T_INT", "T_DOUBLE",
  "T_STRING", "T_NULL_PTR", "T_LEFT_ARROW", "T_RIGHT_ARROW", "T_END",
  "T_IDENTIFIER", "T_STRING_CONSTANT", "T_INTEGER_CONSTANT",
  "T_DOUBLE_CONSTANT", "T_BOOL_CONSTANT", "T_OR", "T_AND", "T_NOT_EQUAL",
  "T_EQUAL", "'>'", "'<'", "T_GREATER_EQUAL", "T_LESS_EQUAL", "'+'", "'-'",
  "'*'", "'/'", "'%'", "'!'", "T_DECREMENT", "T_INCREMENT", "NEGATION",
  "'.'", "'['", "']'", "T_DIMENSIONS", "';'", "','", "'{'", "'}'", "'('",
  "')'", "$accept", "Input_Line", "Statement", "Non_Dangling_If_Statement",
  "Statement_List", "Declaration_List", "Declaration",
  "Routine_Definition", "Bracketed_Declaration_List",
  "Variable_Declaration", "Type", "Expression", "Assignment_Expression",
  "LValue", "Field_Access_Expression", "Call", "Expression_List_Wild",
  "Expression_List", "Expression_List_Helper", "Constant_Expression", YY_NULL
  };


  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Math_Parsing_Unit::rhs_number_type
  Math_Parsing_Unit::yyrhs_[] =
  {
        45,     0,    -1,    55,    11,    -1,    50,    11,    -1,    47,
      -1,    55,    38,    -1,    50,    38,    -1,    -1,    48,    46,
      -1,    -1,    50,    -1,    49,    39,    50,    -1,    53,    -1,
      51,    -1,    52,    12,    52,    10,    54,    40,    48,    41,
      -1,    35,    49,    36,    -1,    54,    12,    -1,    54,    12,
      10,    55,    -1,    54,    12,     9,    55,    -1,     6,    -1,
       5,    -1,     4,    -1,     3,    -1,    56,    -1,    59,    -1,
      63,    -1,    57,    -1,    55,    25,    55,    -1,    55,    26,
      55,    -1,    55,    27,    55,    -1,    55,    28,    55,    -1,
      42,    55,    43,    -1,    26,    55,    -1,    57,     9,    55,
      -1,    58,    -1,    12,    -1,    12,    42,    60,    43,    -1,
      -1,    61,    -1,    62,    -1,    55,    -1,    62,    39,    55,
      -1,    14,    -1,    15,    -1,    16,    -1,    13,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  Math_Parsing_Unit::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    11,    14,    17,    18,    21,
      22,    24,    28,    30,    32,    41,    45,    48,    53,    58,
      60,    62,    64,    66,    68,    70,    72,    74,    78,    82,
      86,    90,    94,    97,   101,   103,   105,   110,   111,   113,
     115,   117,   121,   123,   125,   127
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Math_Parsing_Unit::yyrline_[] =
  {
         0,   176,   176,   181,   189,   194,   200,   206,   209,   215,
     218,   222,   229,   230,   236,   244,   251,   255,   261,   271,
     272,   273,   274,   278,   282,   286,   289,   293,   296,   301,
     306,   311,   312,   318,   330,   333,   344,   357,   358,   362,
     368,   373,   382,   385,   386,   387
  };

  // Print the state stack on the debug stream.
  void
  Math_Parsing_Unit::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (state_stack_type::const_iterator i = yystate_stack_.begin ();
	 i != yystate_stack_.end (); ++i)
      *yycdebug_ << ' ' << *i;
    *yycdebug_ << std::endl;
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  Math_Parsing_Unit::yy_reduce_print_ (int yyrule)
  {
    unsigned int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    /* Print the symbols being reduced, and their result.  */
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
	       << " (line " << yylno << "):" << std::endl;
    /* The symbols being reduced.  */
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
		       yyrhs_[yyprhs_[yyrule] + yyi],
		       &(yysemantic_stack_[(yynrhs) - (yyi + 1)]),
		       &(yylocation_stack_[(yynrhs) - (yyi + 1)]));
  }
#endif // YYDEBUG

  /* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
  Math_Parsing_Unit::token_number_type
  Math_Parsing_Unit::yytranslate_ (int t)
  {
    static
    const token_number_type
    translate_table[] =
    {
           0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    30,     2,     2,     2,    29,     2,     2,
      42,    43,    27,    25,    39,    26,    34,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    38,
      22,     2,    21,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    35,     2,    36,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    40,     2,    41,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    23,    24,    31,    32,
      33,    37
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Math_Parsing_Unit::yyeof_ = 0;
  const int Math_Parsing_Unit::yylast_ = 97;
  const int Math_Parsing_Unit::yynnts_ = 20;
  const int Math_Parsing_Unit::yyempty_ = -2;
  const int Math_Parsing_Unit::yyfinal_ = 30;
  const int Math_Parsing_Unit::yyterror_ = 1;
  const int Math_Parsing_Unit::yyerrcode_ = 256;
  const int Math_Parsing_Unit::yyntokens_ = 44;

  const unsigned int Math_Parsing_Unit::yyuser_token_number_max_ = 281;
  const Math_Parsing_Unit::token_number_type Math_Parsing_Unit::yyundef_token_ = 2;


} // yy
/* Line 1141 of lalr1.cc  */
#line 1317 "math_parsing_unit.cc"
/* Line 1142 of lalr1.cc  */
#line 391 "parser.y"


/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */
 


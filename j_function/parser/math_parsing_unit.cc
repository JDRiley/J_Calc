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
#line 11 "parser/parser.y"


/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
#include "j_yy_stack.h"
#include "parser.h"
#include "Math_Parser.h"
#include "../Constant_Symbol.h"
#include "J_Calc_Parser.h"
#include <J_UI/J_UI_String.h>
#include "../Expression_List.h"
#include "../Call_Expression.h"
#include "../J_Symbol_Identifier.h"
#include "../Field_Access_Expression.h"
#include "../Arguments.h"
#include "../j_expression.h"
#include "../Modulo_Expression.h"
#include "../Specific_Symbol_List.h"
#include "../J_Symbol_Scope.h"
#include "../Custom_Routine_Symbol.h"
#include "../Assignment_Expression.h"
#include "../Statement_Block.h"
#include "../Expression_Statement.h"
#include "../If_Statement.h"
#include "../Relational_Binary_Expression.h"
#include "../Pre_Increment_Expression.h"
#include "../Pre_Decrement_Expression.h"
#include "../Transfer_Statement.h"
#include "../Void_Empty_Expression.h"
#include "../For_Statement.h"


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
#line 98 "parser/math_parsing_unit.cc"


#include "math_parsing_unit.hh"

/* User implementation prologue.  */

/* Line 285 of lalr1.cc  */
#line 106 "parser/math_parsing_unit.cc"


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
#line 201 "parser/math_parsing_unit.cc"

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
        yychar = yylex (&yylval, &yylloc, i_parser);
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
#line 174 "parser/parser.y"
    {
	(yyval.statement) = new Expression_Statement((yysemantic_stack_[(2) - (1)].expression)->get_copy());
	*i_symbol_ptr = (yysemantic_stack_[(2) - (1)].expression);
	return true;
}
    break;

  case 3:
/* Line 670 of lalr1.cc  */
#line 179 "parser/parser.y"
    {
	(yyval.statement) = (yysemantic_stack_[(2) - (1)].declaration);
	*i_symbol_ptr = (yysemantic_stack_[(2) - (1)].declaration)->get_copy();
	//add_user_symbol($1);
	return true;
}
    break;

  case 4:
/* Line 670 of lalr1.cc  */
#line 185 "parser/parser.y"
    {
	(yyval.statement) = (yysemantic_stack_[(2) - (1)].if_statement);
	*i_symbol_ptr = (yysemantic_stack_[(2) - (1)].if_statement)->get_copy();
	return true;
}
    break;

  case 5:
/* Line 670 of lalr1.cc  */
#line 192 "parser/parser.y"
    {
	(yyval.statement) = new Expression_Statement((yysemantic_stack_[(2) - (1)].expression));
}
    break;

  case 6:
/* Line 670 of lalr1.cc  */
#line 195 "parser/parser.y"
    {
	(yyval.statement) = (yysemantic_stack_[(1) - (1)].if_statement);
}
    break;

  case 7:
/* Line 670 of lalr1.cc  */
#line 199 "parser/parser.y"
    {
	(yyval.statement) = (yysemantic_stack_[(2) - (1)].declaration);
}
    break;

  case 8:
/* Line 670 of lalr1.cc  */
#line 202 "parser/parser.y"
    {
	(yyval.statement) = new Transfer_Statement((yysemantic_stack_[(3) - (2)].expression));
}
    break;

  case 9:
/* Line 670 of lalr1.cc  */
#line 205 "parser/parser.y"
    {
	(yyval.statement) = (yysemantic_stack_[(1) - (1)].statement);
}
    break;

  case 10:
/* Line 670 of lalr1.cc  */
#line 212 "parser/parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(4) - (3)].expression);
}
    break;

  case 11:
/* Line 670 of lalr1.cc  */
#line 216 "parser/parser.y"
    {
	(yyval.if_statement) = new If_Statement((yysemantic_stack_[(2) - (1)].expression), (yysemantic_stack_[(2) - (2)].statement_block));
}
    break;

  case 12:
/* Line 670 of lalr1.cc  */
#line 219 "parser/parser.y"
    {
	(yyval.if_statement) = new If_Statement((yysemantic_stack_[(4) - (1)].expression), (yysemantic_stack_[(4) - (2)].statement_block), (yysemantic_stack_[(4) - (4)].statement_block));
}
    break;

  case 13:
/* Line 670 of lalr1.cc  */
#line 222 "parser/parser.y"
    {
	(yyval.if_statement) = new If_Statement((yysemantic_stack_[(4) - (1)].expression), (yysemantic_stack_[(4) - (2)].statement_block), (yysemantic_stack_[(4) - (4)].if_statement));
}
    break;

  case 14:
/* Line 670 of lalr1.cc  */
#line 225 "parser/parser.y"
    {
	(yyval.if_statement) = new If_Statement((yysemantic_stack_[(5) - (1)].expression), (yysemantic_stack_[(5) - (2)].expression), (yysemantic_stack_[(5) - (4)].expression));
}
    break;

  case 15:
/* Line 670 of lalr1.cc  */
#line 230 "parser/parser.y"
    {
	(yyval.statement_block) = new Statement_Block((yysemantic_stack_[(3) - (2)].symbol_list));
}
    break;

  case 16:
/* Line 670 of lalr1.cc  */
#line 235 "parser/parser.y"
    {
(yyval.symbol_list) = new Specific_Symbol_List<j_symbol>();
}
    break;

  case 17:
/* Line 670 of lalr1.cc  */
#line 238 "parser/parser.y"
    {
	(yyval.symbol_list) = (yysemantic_stack_[(2) - (1)].symbol_list);
	(yyval.symbol_list)->add_symbol((yysemantic_stack_[(2) - (2)].statement));
}
    break;

  case 18:
/* Line 670 of lalr1.cc  */
#line 246 "parser/parser.y"
    {
	(yyval.statement) = new For_Statement((yysemantic_stack_[(8) - (3)].statement), (yysemantic_stack_[(8) - (4)].expression), (yysemantic_stack_[(8) - (6)].expression), (yysemantic_stack_[(8) - (8)].statement_block));
}
    break;

  case 19:
/* Line 670 of lalr1.cc  */
#line 250 "parser/parser.y"
    {
	(yyval.declaration_list) = new Specific_Symbol_List<j_declaration>;
}
    break;

  case 20:
/* Line 670 of lalr1.cc  */
#line 253 "parser/parser.y"
    {
	(yyval.declaration_list) = new Specific_Symbol_List<j_declaration>();
	(yyval.declaration_list)->add_symbol((yysemantic_stack_[(1) - (1)].declaration));
}
    break;

  case 21:
/* Line 670 of lalr1.cc  */
#line 257 "parser/parser.y"
    {
	(yyval.declaration_list) = (yysemantic_stack_[(3) - (1)].declaration_list);
	(yyval.declaration_list)->add_symbol((yysemantic_stack_[(3) - (3)].declaration));
}
    break;

  case 22:
/* Line 670 of lalr1.cc  */
#line 264 "parser/parser.y"
    {(yyval.declaration) = (yysemantic_stack_[(1) - (1)].declaration);}
    break;

  case 23:
/* Line 670 of lalr1.cc  */
#line 265 "parser/parser.y"
    { 
	(yyval.declaration) = (yysemantic_stack_[(1) - (1)].declaration);
	(yyloc) = (yylocation_stack_[(1) - (1)]);
}
    break;

  case 24:
/* Line 670 of lalr1.cc  */
#line 272 "parser/parser.y"
    {
	(yyval.declaration) = new Custom_Routine_Symbol((yysemantic_stack_[(6) - (2)].identifier), *(yysemantic_stack_[(6) - (1)].declaration_list), *(yysemantic_stack_[(6) - (3)].declaration_list), (yysemantic_stack_[(6) - (5)].type_syntax), (yysemantic_stack_[(6) - (6)].statement_block));
	(yysemantic_stack_[(6) - (1)].declaration_list).destroy();
	(yysemantic_stack_[(6) - (3)].declaration_list).destroy();

}
    break;

  case 25:
/* Line 670 of lalr1.cc  */
#line 281 "parser/parser.y"
    {
	(yyval.declaration_list) = (yysemantic_stack_[(3) - (2)].declaration_list);
}
    break;

  case 26:
/* Line 670 of lalr1.cc  */
#line 288 "parser/parser.y"
    {(yyval.declaration) =  new Variable_Symbol((yysemantic_stack_[(2) - (1)].type_syntax), (yysemantic_stack_[(2) - (2)].identifier));
	
	
}
    break;

  case 27:
/* Line 670 of lalr1.cc  */
#line 292 "parser/parser.y"
    {
	(yyval.declaration) = new Variable_Reference_Symbol((yysemantic_stack_[(4) - (1)].type_syntax), (yysemantic_stack_[(4) - (2)].identifier), (yysemantic_stack_[(4) - (4)].expression)); 
	
	
	
}
    break;

  case 28:
/* Line 670 of lalr1.cc  */
#line 298 "parser/parser.y"
    {
	(yyval.declaration) = new Variable_Symbol((yysemantic_stack_[(4) - (1)].type_syntax), (yysemantic_stack_[(4) - (2)].identifier), *(yysemantic_stack_[(4) - (4)].expression));
	
	
	(yysemantic_stack_[(4) - (4)].expression).destroy();
	
}
    break;

  case 29:
/* Line 670 of lalr1.cc  */
#line 308 "parser/parser.y"
    {(yyval.type_syntax) = make_double_type_syntax();}
    break;

  case 30:
/* Line 670 of lalr1.cc  */
#line 309 "parser/parser.y"
    {(yyval.type_syntax) = make_int_type_syntax();}
    break;

  case 31:
/* Line 670 of lalr1.cc  */
#line 310 "parser/parser.y"
    {(yyval.type_syntax) = make_bool_type_syntax();}
    break;

  case 32:
/* Line 670 of lalr1.cc  */
#line 311 "parser/parser.y"
    {(yyval.type_syntax) = make_void_type_syntax(); }
    break;

  case 33:
/* Line 670 of lalr1.cc  */
#line 312 "parser/parser.y"
    {(yyval.type_syntax) = make_string_type_syntax(); }
    break;

  case 34:
/* Line 670 of lalr1.cc  */
#line 316 "parser/parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
	
}
    break;

  case 35:
/* Line 670 of lalr1.cc  */
#line 320 "parser/parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
	
}
    break;

  case 36:
/* Line 670 of lalr1.cc  */
#line 324 "parser/parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].constant_symbol); 
}
    break;

  case 37:
/* Line 670 of lalr1.cc  */
#line 327 "parser/parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
	
}
    break;

  case 38:
/* Line 670 of lalr1.cc  */
#line 331 "parser/parser.y"
    { 
	(yyval.expression) = new Addition_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
}
    break;

  case 39:
/* Line 670 of lalr1.cc  */
#line 334 "parser/parser.y"
    { 
	(yyval.expression) = new Subtraction_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
	
	
}
    break;

  case 40:
/* Line 670 of lalr1.cc  */
#line 339 "parser/parser.y"
    { 
	(yyval.expression) = new Multiplication_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
	
	
}
    break;

  case 41:
/* Line 670 of lalr1.cc  */
#line 344 "parser/parser.y"
    { 
	(yyval.expression) = new Division_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
}
    break;

  case 42:
/* Line 670 of lalr1.cc  */
#line 347 "parser/parser.y"
    {
	(yyval.expression) = new Modulo_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
}
    break;

  case 43:
/* Line 670 of lalr1.cc  */
#line 350 "parser/parser.y"
    {
	(yyval.expression) = new Pre_Increment_Expression((yysemantic_stack_[(2) - (2)].expression));
}
    break;

  case 44:
/* Line 670 of lalr1.cc  */
#line 353 "parser/parser.y"
    {
	(yyval.expression) = new Pre_Decrement_Expression((yysemantic_stack_[(2) - (2)].expression));
}
    break;

  case 45:
/* Line 670 of lalr1.cc  */
#line 356 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::GREATER);
}
    break;

  case 46:
/* Line 670 of lalr1.cc  */
#line 359 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::LESS);
}
    break;

  case 47:
/* Line 670 of lalr1.cc  */
#line 362 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::GREATER_EQUAL);
}
    break;

  case 48:
/* Line 670 of lalr1.cc  */
#line 365 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::LESS_EQUAL);
}
    break;

  case 49:
/* Line 670 of lalr1.cc  */
#line 368 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::AND);
}
    break;

  case 50:
/* Line 670 of lalr1.cc  */
#line 371 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::OR);
}
    break;

  case 51:
/* Line 670 of lalr1.cc  */
#line 374 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::EQUAL);
}
    break;

  case 52:
/* Line 670 of lalr1.cc  */
#line 377 "parser/parser.y"
    {
	(yyval.expression) = new Relational_Binary_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression), Operators::NOT_EQUAL);
}
    break;

  case 53:
/* Line 670 of lalr1.cc  */
#line 380 "parser/parser.y"
    {(yyval.expression) = (yysemantic_stack_[(3) - (2)].expression);  }
    break;

  case 54:
/* Line 670 of lalr1.cc  */
#line 381 "parser/parser.y"
    {
	(yyval.expression) = new Unary_Negate_Expression((yysemantic_stack_[(2) - (2)].expression));
}
    break;

  case 55:
/* Line 670 of lalr1.cc  */
#line 387 "parser/parser.y"
    {
	(yyval.expression) = new Void_Empty_Expression;
}
    break;

  case 56:
/* Line 670 of lalr1.cc  */
#line 390 "parser/parser.y"
    {
	(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression);
}
    break;

  case 57:
/* Line 670 of lalr1.cc  */
#line 395 "parser/parser.y"
    {
	(yyval.expression) = new Assignment_Expression((yysemantic_stack_[(3) - (1)].expression), (yysemantic_stack_[(3) - (3)].expression));
	
}
    break;

  case 58:
/* Line 670 of lalr1.cc  */
#line 407 "parser/parser.y"
    {(yyval.expression) = (yysemantic_stack_[(1) - (1)].expression); }
    break;

  case 59:
/* Line 670 of lalr1.cc  */
#line 410 "parser/parser.y"
    {
	(yyval.expression) = new Field_Access_Expression((yysemantic_stack_[(1) - (1)].identifier));
	
}
    break;

  case 60:
/* Line 670 of lalr1.cc  */
#line 421 "parser/parser.y"
    {
	(yyval.expression) = new Call_Expression((yysemantic_stack_[(4) - (1)].identifier), (yysemantic_stack_[(4) - (3)].arguments));
	
	
}
    break;

  case 61:
/* Line 670 of lalr1.cc  */
#line 434 "parser/parser.y"
    {(yyval.arguments) = new Arguments;}
    break;

  case 62:
/* Line 670 of lalr1.cc  */
#line 435 "parser/parser.y"
    {(yyval.arguments) = (yysemantic_stack_[(1) - (1)].arguments);}
    break;

  case 63:
/* Line 670 of lalr1.cc  */
#line 439 "parser/parser.y"
    {
	(yyval.arguments) = (yysemantic_stack_[(1) - (1)].arguments);
	
}
    break;

  case 64:
/* Line 670 of lalr1.cc  */
#line 445 "parser/parser.y"
    {
	(yyval.arguments) = new Arguments;
	(yyval.arguments)->push_back((yysemantic_stack_[(1) - (1)].expression));
	
}
    break;

  case 65:
/* Line 670 of lalr1.cc  */
#line 450 "parser/parser.y"
    {
	(yyval.arguments) = (yysemantic_stack_[(3) - (1)].arguments);
	
	(yyval.arguments)->push_back((yysemantic_stack_[(3) - (3)].expression));
	
}
    break;

  case 66:
/* Line 670 of lalr1.cc  */
#line 459 "parser/parser.y"
    {
	(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol);
}
    break;

  case 67:
/* Line 670 of lalr1.cc  */
#line 462 "parser/parser.y"
    {(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol);}
    break;

  case 68:
/* Line 670 of lalr1.cc  */
#line 463 "parser/parser.y"
    {(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol);  }
    break;

  case 69:
/* Line 670 of lalr1.cc  */
#line 464 "parser/parser.y"
    {(yyval.constant_symbol) = (yysemantic_stack_[(1) - (1)].constant_symbol); }
    break;


/* Line 670 of lalr1.cc  */
#line 1032 "parser/math_parsing_unit.cc"
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
  const signed char Math_Parsing_Unit::yypact_ninf_ = -54;
  const short int
  Math_Parsing_Unit::yypact_[] =
  {
       177,   -54,   -54,   -54,   -54,   -54,   -39,   -54,   -54,   -54,
     -54,   213,    -9,    -9,     3,   -29,   213,    37,   206,    27,
      60,   -54,    52,   -54,    57,   328,   -54,    67,   -54,   -54,
     -54,   213,    17,   -54,   -54,   -54,   -34,   -54,   213,    59,
     -54,   -54,    61,   341,   -54,   -54,    68,    54,   -54,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,   213,   355,    48,   -54,    50,   -54,     3,   237,
     -54,    94,   -27,   213,    83,   213,   213,   367,    -1,   133,
     133,     5,     5,     5,     5,    17,    17,   -54,   -54,   -54,
     355,   -54,   213,   -54,   -54,   213,    62,   -54,   -54,   -54,
     -54,    66,   262,   -54,   -54,   284,    38,   355,   355,   355,
     355,    73,   137,   -54,   -54,   -54,    70,   -54,   213,   -54,
     306,   213,    72,    70,   -54
  };

  /* YYDEFACT[S] -- default reduction number in state S.  Performed when
     YYTABLE doesn't specify something else to do.  Zero means the
     default is an error.  */
  const unsigned char
  Math_Parsing_Unit::yydefact_[] =
  {
         0,    32,    31,    30,    29,    33,    59,    69,    66,    67,
      68,     0,     0,     0,    19,     0,     0,     0,     0,     0,
       0,    23,     0,    22,     0,     0,    34,    37,    58,    35,
      36,    61,    54,    59,    44,    43,     0,    20,     0,     0,
       1,    16,    11,     0,     4,     3,     0,    26,     2,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    64,     0,    62,    63,    25,     0,     0,
      53,     0,     0,     0,     0,     0,     0,    50,    49,    52,
      51,    45,    46,    47,    48,    38,    39,    40,    41,    42,
      57,    60,     0,    21,    10,    55,     0,    15,    17,     6,
       9,     0,     0,    13,    12,     0,     0,    28,    27,    65,
      56,     0,     0,     7,     5,    14,     0,     8,     0,    24,
       0,    55,     0,     0,    18
  };

  /* YYPGOTO[NTERM-NUM].  */
  const signed char
  Math_Parsing_Unit::yypgoto_[] =
  {
       -54,   -54,     4,   -54,     2,   -53,   -54,   -54,   -54,     1,
     -54,    71,   -54,    13,     0,     8,   -54,    55,   -54,   -54,
     -54,   -54,   -54,   -54
  };

  /* YYDEFGOTO[NTERM-NUM].  */
  const signed char
  Math_Parsing_Unit::yydefgoto_[] =
  {
        -1,    17,    98,    18,    99,    42,    71,   100,    36,   101,
      21,    22,    23,    24,   102,   111,    26,    27,    28,    29,
      64,    65,    66,    30
  };

  /* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule which
     number is the opposite.  If YYTABLE_NINF_, syntax error.  */
  const signed char Math_Parsing_Unit::yytable_ninf_ = -1;
  const unsigned char
  Math_Parsing_Unit::yytable_[] =
  {
        25,    20,    19,    31,    67,    33,     1,     2,     3,     4,
       5,    32,    68,    38,    15,    37,    39,    41,    43,   104,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    63,    57,    58,    59,    60,    61,    40,    69,    44,
      14,     1,     2,     3,     4,     5,    59,    60,    61,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,   119,    75,    76,    46,    34,    35,    93,
     124,    47,    45,   105,   103,   107,   108,    62,    49,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    91,   109,    72,   106,   110,    92,     1,     2,     3,
       4,     5,    70,    95,   112,    14,   113,    96,     6,     7,
       8,     9,    10,   117,    41,   123,   118,    74,   120,   116,
       0,   110,    11,     0,     0,     0,     0,    12,    13,   122,
       0,    14,     0,     0,     0,    15,    16,     0,     0,    97,
       1,     2,     3,     4,     5,     0,    95,     0,     0,     0,
      96,     6,     7,     8,     9,    10,    53,    54,    55,    56,
      57,    58,    59,    60,    61,    11,     0,     0,     0,     0,
      12,    13,     0,     0,    14,     0,     0,     0,    15,    16,
       1,     2,     3,     4,     5,     0,     0,     0,     0,     0,
       0,     6,     7,     8,     9,    10,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    11,     0,     0,     0,     0,
      12,    13,     0,     0,    14,     0,     0,     0,    15,    16,
       6,     7,     8,     9,    10,     0,     0,     6,     7,     8,
       9,    10,     0,     0,    11,     0,     0,     0,     0,    12,
      13,    11,     0,     0,     0,     0,    12,    13,    16,     0,
      41,     0,     0,     0,     0,    16,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      94,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,     0,     0,     0,     0,     0,     0,
       0,     0,   114,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,     0,     0,     0,     0,
       0,     0,     0,     0,   115,    49,    50,    51,    52,    53,
      54,    55,    56,    57,    58,    59,    60,    61,     0,     0,
      48,     0,     0,     0,     0,     0,   121,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      49,    50,    51,    52,    53,    54,    55,    56,    57,    58,
      59,    60,    61,    73,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61
  };

  /* YYCHECK.  */
  const signed char
  Math_Parsing_Unit::yycheck_[] =
  {
         0,     0,     0,    42,    38,    14,     3,     4,     5,     6,
       7,    11,    46,    42,    41,    14,    16,    44,    18,    72,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    31,    27,    28,    29,    30,    31,     0,    38,    12,
      37,     3,     4,     5,     6,     7,    29,    30,    31,    49,
      50,    51,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,   116,    10,    11,    14,    12,    13,    68,
     123,    14,    12,    73,    72,    75,    76,    10,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    43,    92,    32,    11,    95,    46,     3,     4,     5,
       6,     7,    43,     9,    42,    37,    40,    13,    14,    15,
      16,    17,    18,    40,    44,    43,   112,    46,   118,   106,
      -1,   121,    28,    -1,    -1,    -1,    -1,    33,    34,   121,
      -1,    37,    -1,    -1,    -1,    41,    42,    -1,    -1,    45,
       3,     4,     5,     6,     7,    -1,     9,    -1,    -1,    -1,
      13,    14,    15,    16,    17,    18,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    28,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    37,    -1,    -1,    -1,    41,    42,
       3,     4,     5,     6,     7,    -1,    -1,    -1,    -1,    -1,
      -1,    14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    28,    -1,    -1,    -1,    -1,
      33,    34,    -1,    -1,    37,    -1,    -1,    -1,    41,    42,
      14,    15,    16,    17,    18,    -1,    -1,    14,    15,    16,
      17,    18,    -1,    -1,    28,    -1,    -1,    -1,    -1,    33,
      34,    28,    -1,    -1,    -1,    -1,    33,    34,    42,    -1,
      44,    -1,    -1,    -1,    -1,    42,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      43,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    40,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    40,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    -1,    -1,
      12,    -1,    -1,    -1,    -1,    -1,    40,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    30,    31
  };

  /* STOS_[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
  const unsigned char
  Math_Parsing_Unit::yystos_[] =
  {
         0,     3,     4,     5,     6,     7,    14,    15,    16,    17,
      18,    28,    33,    34,    37,    41,    42,    48,    50,    51,
      56,    57,    58,    59,    60,    61,    63,    64,    65,    66,
      70,    42,    61,    14,    64,    64,    55,    56,    42,    61,
       0,    44,    52,    61,    12,    12,    14,    14,    12,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    10,    61,    67,    68,    69,    38,    46,    61,
      43,    53,    32,    32,    58,    10,    11,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    43,    46,    56,    43,     9,    13,    45,    49,    51,
      54,    56,    61,    51,    52,    61,    11,    61,    61,    61,
      61,    62,    42,    40,    40,    40,    60,    40,    49,    52,
      61,    40,    62,    43,    52
  };

#if YYDEBUG
  /* TOKEN_NUMBER_[YYLEX-NUM] -- Internal symbol number corresponding
     to YYLEX-NUM.  */
  const unsigned short int
  Math_Parsing_Unit::yytoken_number_[] =
  {
         0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    62,    60,   278,   279,    43,    45,    42,
      47,    37,    33,   280,   281,   282,    46,    91,    93,   283,
      59,    63,    40,    41,   123,   125,    44
  };
#endif

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
  const unsigned char
  Math_Parsing_Unit::yyr1_[] =
  {
         0,    47,    48,    48,    48,    49,    49,    49,    49,    49,
      50,    51,    51,    51,    51,    52,    53,    53,    54,    55,
      55,    55,    56,    56,    57,    58,    59,    59,    59,    60,
      60,    60,    60,    60,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    62,    62,    63,    64,    65,
      66,    67,    67,    68,    69,    69,    70,    70,    70,    70
  };

  /* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
  const unsigned char
  Math_Parsing_Unit::yyr2_[] =
  {
         0,     2,     2,     2,     2,     2,     1,     2,     3,     1,
       4,     2,     4,     4,     5,     3,     0,     2,     8,     0,
       1,     3,     1,     1,     6,     3,     2,     4,     4,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     2,     2,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     2,     0,     1,     3,     1,     1,
       4,     0,     1,     1,     1,     3,     1,     1,     1,     1
  };

#if YYDEBUG
  /* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
     First, the terminals, then, starting at \a yyntokens_, nonterminals.  */
  const char*
  const Math_Parsing_Unit::yytname_[] =
  {
    "$end", "error", "$undefined", "T_VOID", "T_BOOL", "T_INT", "T_DOUBLE",
  "T_STRING", "T_NULL_PTR", "T_TRANSFER", "T_LEFT_ARROW", "T_RIGHT_ARROW",
  "T_END", "T_FOR", "T_IDENTIFIER", "T_STRING_CONSTANT",
  "T_INTEGER_CONSTANT", "T_DOUBLE_CONSTANT", "T_BOOL_CONSTANT", "T_OR",
  "T_AND", "T_NOT_EQUAL", "T_EQUAL", "'>'", "'<'", "T_GREATER_EQUAL",
  "T_LESS_EQUAL", "'+'", "'-'", "'*'", "'/'", "'%'", "'!'", "T_DECREMENT",
  "T_INCREMENT", "NEGATION", "'.'", "'['", "']'", "T_DIMENSIONS", "';'",
  "'?'", "'('", "')'", "'{'", "'}'", "','", "$accept", "Input_Line",
  "Statement", "Test_Expression", "If_Statement", "Statement_Block",
  "Statement_List", "For_Statement", "Declaration_List", "Declaration",
  "Routine_Definition", "Bracketed_Declaration_List",
  "Variable_Declaration", "Type", "Expression", "Expression_Wild",
  "Assignment_Expression", "LValue", "Field_Access_Expression", "Call",
  "Expression_List_Wild", "Expression_List", "Expression_List_Helper",
  "Constant_Expression", YY_NULL
  };


  /* YYRHS -- A `-1'-separated list of the rules' RHS.  */
  const Math_Parsing_Unit::rhs_number_type
  Math_Parsing_Unit::yyrhs_[] =
  {
        48,     0,    -1,    61,    12,    -1,    56,    12,    -1,    51,
      12,    -1,    61,    40,    -1,    51,    -1,    56,    40,    -1,
       9,    62,    40,    -1,    54,    -1,    41,    42,    61,    43,
      -1,    50,    52,    -1,    50,    52,    32,    52,    -1,    50,
      52,    32,    51,    -1,    50,    61,    32,    61,    40,    -1,
      44,    53,    45,    -1,    -1,    53,    49,    -1,    13,    42,
      49,    61,    40,    62,    43,    52,    -1,    -1,    56,    -1,
      55,    46,    56,    -1,    59,    -1,    57,    -1,    58,    14,
      58,    11,    60,    52,    -1,    37,    55,    38,    -1,    60,
      14,    -1,    60,    14,    11,    61,    -1,    60,    14,    10,
      61,    -1,     6,    -1,     5,    -1,     4,    -1,     3,    -1,
       7,    -1,    63,    -1,    66,    -1,    70,    -1,    64,    -1,
      61,    27,    61,    -1,    61,    28,    61,    -1,    61,    29,
      61,    -1,    61,    30,    61,    -1,    61,    31,    61,    -1,
      34,    64,    -1,    33,    64,    -1,    61,    23,    61,    -1,
      61,    24,    61,    -1,    61,    25,    61,    -1,    61,    26,
      61,    -1,    61,    20,    61,    -1,    61,    19,    61,    -1,
      61,    22,    61,    -1,    61,    21,    61,    -1,    42,    61,
      43,    -1,    28,    61,    -1,    -1,    61,    -1,    64,    10,
      61,    -1,    65,    -1,    14,    -1,    14,    42,    67,    43,
      -1,    -1,    68,    -1,    69,    -1,    61,    -1,    69,    46,
      61,    -1,    16,    -1,    17,    -1,    18,    -1,    15,    -1
  };

  /* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
     YYRHS.  */
  const unsigned char
  Math_Parsing_Unit::yyprhs_[] =
  {
         0,     0,     3,     6,     9,    12,    15,    17,    20,    24,
      26,    31,    34,    39,    44,    50,    54,    55,    58,    67,
      68,    70,    74,    76,    78,    85,    89,    92,    97,   102,
     104,   106,   108,   110,   112,   114,   116,   118,   120,   124,
     128,   132,   136,   140,   143,   146,   150,   154,   158,   162,
     166,   170,   174,   178,   182,   185,   186,   188,   192,   194,
     196,   201,   202,   204,   206,   208,   212,   214,   216,   218
  };

  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
  const unsigned short int
  Math_Parsing_Unit::yyrline_[] =
  {
         0,   174,   174,   179,   185,   192,   195,   199,   202,   205,
     212,   216,   219,   222,   225,   230,   235,   238,   246,   250,
     253,   257,   264,   265,   272,   281,   288,   292,   298,   308,
     309,   310,   311,   312,   316,   320,   324,   327,   331,   334,
     339,   344,   347,   350,   353,   356,   359,   362,   365,   368,
     371,   374,   377,   380,   381,   387,   390,   395,   407,   410,
     421,   434,   435,   439,   445,   450,   459,   462,   463,   464
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
       2,     2,     2,    32,     2,     2,     2,    31,     2,     2,
      42,    43,    29,    27,    46,    28,    36,    30,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    40,
      24,     2,    23,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    37,     2,    38,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,    45,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    25,    26,
      33,    34,    35,    39
    };
    if ((unsigned int) t <= yyuser_token_number_max_)
      return translate_table[t];
    else
      return yyundef_token_;
  }

  const int Math_Parsing_Unit::yyeof_ = 0;
  const int Math_Parsing_Unit::yylast_ = 398;
  const int Math_Parsing_Unit::yynnts_ = 24;
  const int Math_Parsing_Unit::yyempty_ = -2;
  const int Math_Parsing_Unit::yyfinal_ = 40;
  const int Math_Parsing_Unit::yyterror_ = 1;
  const int Math_Parsing_Unit::yyerrcode_ = 256;
  const int Math_Parsing_Unit::yyntokens_ = 47;

  const unsigned int Math_Parsing_Unit::yyuser_token_number_max_ = 283;
  const Math_Parsing_Unit::token_number_type Math_Parsing_Unit::yyundef_token_ = 2;


} // yy
/* Line 1141 of lalr1.cc  */
#line 1624 "parser/math_parsing_unit.cc"
/* Line 1142 of lalr1.cc  */
#line 468 "parser/parser.y"


/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */
 


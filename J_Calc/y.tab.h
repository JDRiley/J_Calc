/* A Bison parser, made by GNU Bison 2.7.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2012 Free Software Foundation, Inc.
   
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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_Void = 258,
     T_Bool = 259,
     T_Int = 260,
     T_Double = 261,
     T_String = 262,
     T_Class = 263,
     T_Null = 264,
     T_Else = 265,
     T_Extends = 266,
     T_This = 267,
     T_Interface = 268,
     T_Implements = 269,
     T_While = 270,
     T_For = 271,
     T_If = 272,
     T_Return = 273,
     T_Break = 274,
     T_New = 275,
     T_NewArray = 276,
     T_Print = 277,
     T_ReadInteger = 278,
     T_ReadLine = 279,
     T_Identifier = 280,
     T_StringConstant = 281,
     T_IntConstant = 282,
     T_DoubleConstant = 283,
     T_BoolConstant = 284,
     T_Switch = 285,
     T_Case = 286,
     T_Default = 287,
     T_Or = 288,
     T_And = 289,
     T_NotEqual = 290,
     T_Equal = 291,
     T_GreaterEqual = 292,
     T_LessEqual = 293,
     T_Decrem = 294,
     T_Increm = 295,
     NEGATION = 296,
     T_Dims = 297
   };
#endif


#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{
/* Line 2058 of yacc.c  */
#line 63 "parser.y"

    int								integerConstant;
    bool							boolConstant;
    char*							stringConstant;

    double							doubleConstant;
	//Here this was char normally
    jomike::Identifier_Name*		identifier; // +1 for terminating null
    jomike::Declaration_Syntax*		declaration;
    jomike::Declaration_List*		declaration_list;
	jomike::Prototype_List*			prototype_list;
	jomike::Prototype_Declaration*	prototype_declaration;
	jomike::Program_Tree*			program_tree;
	jomike::Variable_Syntax*		variable;
	jomike::Type_Syntax*			type;
	jomike::Type_Named*				type_named;
	jomike::Statement_Syntax*		statement_syntax; 
	jomike::Statement_Component*	statement_component;
	jomike::Statement_List*			statement_list;
	jomike::Statement_Block*		statement_block;
	jomike::LValue_Expression*		lvalue;
	jomike::Field_Access_Expression*field_access;
	jomike::Expression*				expression;
	jomike::Call_Expression*		call_expression;
	jomike::Expression_List*		expression_list;
	jomike::Prefix_Operator*		prefix_operator;
	jomike::Named_Type_List*		named_type_list;
	jomike::Expression_Wild*		expression_wild;
	jomike::Field_List*				field_list;
	jomike::Variable_Declaration*	variable_declaration;
	jomike::Function_Declaration*	function_declaration;


/* Line 2058 of yacc.c  */
#line 133 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;
# define yyltype YYLTYPE /* obsolescent; will be withdrawn */
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;
extern YYLTYPE yylloc;
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */

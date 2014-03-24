/* File: parser.y
 * --------------
 * Yacc input file to generate the parser for the compiler.
 *
 * pp2: your job is to write a parser that will construct the parse tree
 *      and if no parse errors were found, print it.  The parser should 
 *      accept the language as described in specification, and as augmented 
 *      in the pp2 handout.
 */
%debug
%{

/* Just like lex, the text within this first region delimited by %{ and %}
 * is assumed to be C/C++ code and will be copied verbatim to the y.tab.c
 * file ahead of the definitions of the yyparse() function. Add other header
 * file inclusions or C++ variable declarations/prototypes that are needed
 * by your code here.
 */
//#include "Math_Lexer.h" // for yylex
#include <J_Symbol_Fwd_Decl.h>

#include "parser.h"
#include <Constant_Symbol.h>

using namespace jomike;
void yyerror(const char *msg); // standard error-handling routine

j_symbol_component* jtl::g_input_line = nullptr;

template<typename... Args>
void delete_tokens(Args... i_ptrs){
	j_symbol_component* pointers[] = {i_ptrs...};
#ifdef WIN32
	for(auto f_pointer : pointers){
		delete f_pointer;
	}
#else
	int size = safe_int_cast(sizeof(pointers)/sizeof(Syntax_Tree_Component*));
	for(int i=0; i < size; i++){
		delete pointers[i];
	}
#endif

}

%}
%skeleton "lalr1.cc"
%defines
%define parser_class_name "Math_Parsing_Unit"
%parse-param{jtl::Math_Parser* i_parser}
%lex-param{jtl::Math_Parser* i_parser}

%code requires{
#include<J_UI/J_UI_String.h>
#include "J_Calc_Fwd_Decl.h"
#include <J_Symbol_Fwd_Decl.h>
}

/* The section before the first %% is the Definitions section of the yacc
 * input file. Here is where you declare tokens and types, add precedence
 * and associativity options, and so on.
 */
 
/* yylval 
 * ------
 * Here we define the type of the yylval global variable that is used by
 * the scanner to store attibute information about the token just scanned
 * and thus communicate that information to the parser. 
 *
 * pp2: You will need to add new fields to this union as you add different 
 *      attributes to your non-terminal symbols.
 */
%union {
    jomike::J_UI_String*			identifier;
    jomike::j_symbol_component*		symbol_component;
	jomike::Constant_Symbol*		constant_symbol;
}

%destructor {} <integer_constant>
%destructor {} <boolean_constant>
%destructor {} <string_constant>
%destructor {} <double_constant>
%destructor {delete $$;} <*>

/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Yacc will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */



%token							T_VOID T_BOOL T_INT T_DOUBLE T_STRING 
 
%token							T_NULL_PTR 
%token							T_LEFT_ARROW

%token	<identifier>			T_IDENTIFIER
%token	<constant_symbol>		T_STRING_CONSTANT 
%token	<constant_symbol>		T_INTEGER_CONSTANT
%token	<constant_symbol>		T_DOUBLE_CONSTANT
%token	<constant_symbol>		T_BOOL_CONSTANT

//Operator Precence
%right '='
%left T_OR
%left   T_AND
%left T_EQUAL T_NOT_EQUAL
%left   T_LESS_EQUAL T_GREATER_EQUAL  '>' '<' 

%left '+' '-'

%left '*' '/' '%'

%nonassoc '!' NEGATION T_INCREMENT T_DECREMENT

%left '.'
%nonassoc '['
%left T_DIMENSIONS

/* Non-terminal types
 * ------------------
 * In order for yacc to assign/access the correct field of $$, $1, we
 * must to declare which field is appropriate for the non-terminal.
 * As an example, this first type declaration establishes that the DeclList
 * non-terminal uses the field named "declList" in the yylval union. This
 * means that when we are setting $$ for a reduction for DeclList ore reading
 * $n which corresponds to a DeclList nonterminal we are accessing the field
 * of the union named "declList" which is of type List<Decl*>.
 * pp2: You'll need to add many of these of your own.
 */
 
 %type	<symbol_component>	Input_Line
 %type	<symbol_component>	Expression
 %type	<constant_symbol>	Constant_Expression
%%
/* Rules
* -----
* All productions and actions should be placed between the start and stop
* %% markers which delimit the Rules section.
	 
*/

Input_Line
: Expression {$$ = g_input_line = $1; }
;

Expression
: Constant_Expression {$$ = $1; }
;

Constant_Expression
: T_INTEGER_CONSTANT {$$ = $1; }
;


%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */
 


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


#include "J_Calc_Parser.h"
#include "../j_constant_expression.h"
#include <j_symbol/Constant_Symbol.h>
#include "../Call_Expression.h"
#include <j_symbol/J_Symbol_Identifier.h>
#include "../Field_Access_Expression.h"
#include "../Arguments.h"
#include "../Modulo_Expression.h"
#include <j_symbol/Specific_Symbol_List.h>
#include "../j_declaration.h"
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
#include "../Variable_Symbol.h"
#include "../Variable_Reference_Symbol.h"
#include "../Unary_Negate_Expression.h"
#include "../Subtraction_Expression.h"
#include "../Multiplication_Expression.h"
#include "../Addition_Expression.h"
#include "../Division_Expression.h"
#include "../Type_Factory.h"

using namespace jomike;
void yyerror(const char *msg); // standard error-handling routine




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

%}
%skeleton "lalr1.cc"
%defines
%define parser_class_name {J_Calc_Parsing_Unit}
%parse-param{jtl::j_calc_symbol** i_symbol_ptr}
%parse-param{jtl::J_Calc_Parser* i_parser}
%lex-param{jtl::J_Calc_Parser* i_parser}


%code requires{
#include "../J_Calc_Symbol_Fwd_Decl.h"



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


//%destructor{delete $$;} <*>

/* Tokens
* ------
* Here we tell yacc about all the token types that we are using.
* Yacc will assign unique numbers to these and export the #define
* in the generated y.tab.h header file.
*/





%token							T_VOID T_BOOL T_INT T_DOUBLE T_STRING

%token							T_NULL_PTR T_TRANSFER
%token							T_LEFT_ARROW T_RIGHT_ARROW
%token							T_END T_FOR
%token	<identifier>			T_IDENTIFIER
%token	<constant_symbol>		T_STRING_CONSTANT
%token	<constant_symbol>		T_INTEGER_CONSTANT
%token	<constant_symbol>		T_DOUBLE_CONSTANT
%token	<constant_symbol>		T_BOOL_CONSTANT


//Operator Precedence
%right T_LEFT_ARROW
%left T_OR
%left   T_AND
%left T_EQUAL T_NOT_EQUAL
%left   T_LESS_EQUAL T_GREATER_EQUAL  '>' '<'

%left '+' '-'

%left '*' '/' '%'

%nonassoc '!' NEGATION T_INCREMENT T_DECREMENT

%left '.'
%nonassoc '[' ']'
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

%type	<arguments>	Expression_List Expression_List_Helper Expression_List_Wild		
%type	<expression>		Expression Call Field_Access_Expression LValue
%type	<expression>		Assignment_Expression Test_Expression Expression_Wild
%type	<constant_symbol>	Constant_Expression
%type	<declaration>		Declaration Variable_Declaration Routine_Definition
%type	<type_syntax>		Type
%type	<symbol_list>		Statement_List
%type	<declaration_list>	Declaration_List Bracketed_Declaration_List
%type	<statement_block>	Statement_Block
%type	<statement>			Statement Input_Line  For_Statement
%type	<if_statement>		If_Statement
%%

/* Rules
* -----
* All productions and actions should be placed between the start and stop
* %% markers which delimit the Rules section.
*/


Input_Line
: Expression T_END {
	$$ = new Expression_Statement(@$, $1->get_copy());
	*i_symbol_ptr = $$;
	return true;
}
|Declaration T_END{
	$$ = $1;
	*i_symbol_ptr = $1->get_copy();
	//add_user_symbol($1);
	return true;
}
| If_Statement T_END{
	$$ = $1;
	*i_symbol_ptr = $1->get_copy();
	return true;
}
;
Statement
: Expression ';' {
	$$ = new Expression_Statement(@$, $1);
}
| If_Statement{
	$$ = $1;
}

| Declaration ';' {
	$$ = $1;
}
| T_TRANSFER Expression_Wild ';' {
	$$ = new Transfer_Statement(@$, $2);
}
| For_Statement{
	$$ = $1;
}

;

Test_Expression
: '?' '(' Expression ')' {
	$$ = $3;
}
If_Statement
:  Test_Expression Statement_Block{
	$$ = new If_Statement(@$, $1, $2);
}
| Test_Expression Statement_Block '!' Statement_Block{
	$$ = new If_Statement(@$, $1, $2, $4);
}
| Test_Expression Statement_Block '!' If_Statement{
	$$ = new If_Statement(@$, $1, $2, $4);
}
| Test_Expression Expression '!' Expression ';'{
	J_Calc_Symbol_List* expr_list = new J_Calc_Symbol_List(@2);
	expr_list->add_symbol($2);

	Statement_Block* state_block = new Statement_Block(expr_list);
	$$ = new If_Statement(@$, $1, state_block, new Expression_Statement(@4 + @5, $4));
}
;
Statement_Block
: '{' Statement_List '}'{
	$$ = new Statement_Block($2);
}
;
Statement_List
: /*empty*/ {
$$ = new Specific_Symbol_List<j_calc_symbol>(@$);
}
| Statement_List Statement {
	$$ = $1;
	$$->add_symbol($2);
	$$->set_location(@$);
}
;


For_Statement
: T_FOR '(' Statement  Expression ';' Expression_Wild ')' Statement_Block{
	$$ = new For_Statement(@$, $3, $4, $6, $8);
}
;

Declaration_List
: /*empty*/ {
	$$ = new Specific_Symbol_List<j_declaration>(@$);
}
|Declaration {
	$$ = new Specific_Symbol_List<j_declaration>(@$);
	$$->add_symbol($1);
}
| Declaration_List ',' Declaration{
	$$ = $1;
	$$->add_symbol($3);
}
;

Declaration
: Variable_Declaration {$$ = $1;}
| Routine_Definition{ 
	$$ = $1;
	@$ = @1;
}
;

Routine_Definition
: Bracketed_Declaration_List T_IDENTIFIER Bracketed_Declaration_List T_RIGHT_ARROW Type Statement_Block{
	$$ = new Custom_Routine_Symbol(@$, $2, *$1, *$3, $5, $6);
	//$1.destroy();
	//$3.destroy();

}
;

Bracketed_Declaration_List
: '[' Declaration_List ']' {
	$$ = $2;
}

;

Variable_Declaration
: Type T_IDENTIFIER{$$ =  new Variable_Symbol(@$, $1, $2);
	
	
}
| Type T_IDENTIFIER T_RIGHT_ARROW Expression {
	$$ = new Variable_Reference_Symbol(@$, $1, $2, $4); 
	
	
	
}
| Type T_IDENTIFIER T_LEFT_ARROW Expression {
	$$ = new Variable_Symbol(@$, $1, $2, *$4);
	
	
	$4.destroy();
	
}
;

Type 
: T_DOUBLE{$$ = make_double_type_syntax(@$);}
| T_INT{$$ = make_int_type_syntax(@$);}
| T_BOOL{$$ = make_bool_type_syntax(@$);}
| T_VOID{$$ = make_void_type_syntax(@$); }
| T_STRING{$$ = make_string_type_syntax(@$); }
;

Expression
: Assignment_Expression{
	$$ = $1;
	
}
| Call{
	$$ = $1;
	
}
| Constant_Expression{
	//TODO: reintroduce j_calc constant symbols so they derive from j_calc_expressions
	$$ = $1; 
}
| LValue {
	$$ = $1;
	
}
| Expression '+' Expression { 
	$$ = new Addition_Expression(@$, $1, $3);
}
| Expression '-' Expression { 
	$$ = new Subtraction_Expression(@$, $1, $3);
	
	
}
| Expression '*' Expression { 
	$$ = new Multiplication_Expression(@$, $1, $3);
	
	
}
| Expression '/' Expression { 
	$$ = new Division_Expression(@$, $1, $3);
}
| Expression '%' Expression{
	$$ = new Modulo_Expression(@$, $1, $3);
}
| T_INCREMENT LValue{
	$$ = new Pre_Increment_Expression(@$, $2);
}
| T_DECREMENT LValue{
	$$ = new Pre_Decrement_Expression(@$, $2);
}
| Expression '>' Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::GREATER);
}
| Expression '<' Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::LESS);
}
| Expression T_GREATER_EQUAL Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::GREATER_EQUAL);
}
| Expression T_LESS_EQUAL Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::LESS_EQUAL);
}
| Expression  T_AND Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::AND);
}
| Expression T_OR Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::OR);
}
| Expression T_EQUAL Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::EQUAL);
}
| Expression T_NOT_EQUAL Expression{
	$$ = new Relational_Binary_Expression(@$, $1, $3, Operators::NOT_EQUAL);
}
| '(' Expression ')' {$$ = $2;  }
| '-' Expression{
	$$ = new Unary_Negate_Expression(@$, $2);
}
;

Expression_Wild
: /*empty*/{
	$$ = new Void_Empty_Expression(@$);
}
| Expression{
	$$ = $1;
}

Assignment_Expression
: LValue T_LEFT_ARROW Expression {
	$$ = new Assignment_Expression(@$, $1, $3);
	
}
;


LValue
: /*Expression '[' Expression ']' {
	$$ = new Array_Access_Expression(@$, *$1, *$3, @$);
	delete_tokens($1, $3);
}
| */Field_Access_Expression {$$ = $1; }
;
Field_Access_Expression
: T_IDENTIFIER {
	$$ = new Field_Access_Expression(@$, $1);
	
}
/*| Expression T_BACKSLASH T_Identifier {
	$$ = new Field_Access_Expression(@$, $1, *$3);
	delete_tokens($3);
}*/
;

Call
: T_IDENTIFIER '(' Expression_List_Wild ')' {
	$$ = new Call_Expression(@$, $1, $3);
	
	
}
//| Expression T_BACKSLASH T_IDENTIFIER '(' Expression_List_Wild ')' {
//	$$ = new Call_Expression(@$, *$1, $3, $5);
//	delete_tokens($1, $5);
//	delete $3;
//}
;

Expression_List_Wild
:	/*empty*/ {$$ = new Arguments(@$);}
| Expression_List {
	//$$ = $1;
}
;

Expression_List
: Expression_List_Helper {
	//$$ = $1;
	
}
;
Expression_List_Helper
: Expression {
	$$ = new Arguments(@$);
	$$->push_back($1);
	
}
| Expression_List_Helper ',' Expression {
	$$ = $1;
	
	$$->push_back($3);
	
}
;

Constant_Expression
: T_INTEGER_CONSTANT {
	$$ = $1;
}
| T_DOUBLE_CONSTANT{$$ = $1;}
| T_BOOL_CONSTANT{$$ = $1;  }
| T_STRING_CONSTANT{$$ = $1; }
;


%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */
 


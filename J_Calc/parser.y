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
#include "scanner.h" // for yylex
#include "Syntax_Tree_Fwd_Decl.h"
#include "parser.h"
#include "errors.h"
#include "utility.h"

using namespace jomike;
using std::array;
void yyerror(const char *msg); // standard error-handling routine

jomike::Program_Tree* g_program_tree = nullptr;

template<typename... Args>
void delete_tokens(Args... i_ptrs){
	Syntax_Tree_Component* pointers[] = {i_ptrs...};
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
}

%destructor {} <integerConstant>
%destructor {} <boolConstant>
%destructor {} <stringConstant>
%destructor {} <doubleConstant>
%destructor {delete $$;} <*>

/* Tokens
 * ------
 * Here we tell yacc about all the token types that we are using.
 * Yacc will assign unique numbers to these and export the #define
 * in the generated y.tab.h header file.
 */

 

%token   T_Void T_Bool T_Int T_Double T_String T_Class 



 
%token T_Null T_Else
%token T_Extends T_This T_Interface T_Implements
%token   T_While T_For T_If  T_Return T_Break
%token   T_New T_NewArray T_Print T_ReadInteger T_ReadLine

%token   <identifier> T_Identifier
%token   <stringConstant> T_StringConstant 
%token   <expression> T_IntConstant
%token   <doubleConstant> T_DoubleConstant
%token   <boolConstant> T_BoolConstant

%token   T_Switch T_Case T_Default




//Operator Precence

%right '='
%left T_Or
%left   T_And
%nonassoc T_Equal T_NotEqual
%nonassoc   T_LessEqual T_GreaterEqual  '>' '<' 

%left '+' '-'

%left '*' '/' '%'

%nonassoc '!' NEGATION T_Increm T_Decrem

%left '.'
%nonassoc '['
%left T_Dims

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
%type <declaration_list>	Declaration_List 
%type <prototype_list>		Prototype_List;
%type <declaration>			Declaration 
%type <variable_declaration>Variable_Declaration 
%type <function_declaration>Function_Declaration
%type <prototype_declaration>Prototype
%type <declaration>			Class_Declaration 
%type <declaration>			Interface_Declaration
%type <variable>			Variable
%type <type>				Type 
%type <type_named>			Named_Type
%type <declaration_list>	Formal_List
%type <field_list>			Field_List;
%type <declaration_list>	Formal_List_Helper
%type <named_type_list>		Named_Type_Comma_List Class_Implements_Rule
%type <statement_block>		Statement_Block
%type <statement_component>	Statement Non_Dangling_If_Statement If_Dangling_Statement
%type <statement_list>		Statement_List 
%type <statement_list>		Statement_List_Block_End
%type <declaration_list>	Variable_Declaration_List
%type <declaration_list>	Declaration_List_Helper
%type <statement_syntax>	Return_Statement 
%type <statement_syntax>	If_Else_Statement 

%type <expression>			Expression Constant Assignment_Expression
%type <expression_list>		Expression_List Expression_List_Wild
%type <expression_wild>		Expression_Wild
%type <call_expression>		Call
%type <lvalue>				LValue
%type <field_access>		Field_Access_Expression

%type <prefix_operator>		'!'
%%
/* Rules
 * -----
 * All productions and actions should be placed between the start and stop
 * %% markers which delimit the Rules section.
	 
 */
Program 
	: Declaration_List {
		g_program_tree = new Program_Tree(*$1);
		// if no errors, advance to next phase
		Symbol_Map* symbol_map = nullptr;
		if (ReportError::NumErrors() == 0) {
		

			symbol_map = g_program_tree->check_syntax();
		}
		if (ReportError::NumErrors()) {
		

			//ReportError::PrintErrors();
		}
		delete symbol_map;
		delete_tokens(g_program_tree, $1);

	}
	;

Declaration_List 
	: Declaration_List_Helper {$$ = $1;}
	;
Declaration_List_Helper
	: Declaration_List_Helper Declaration {
		$$ = $1; 
		$$->add_declaration(*$2);
		delete_tokens($2);
	}
	| Declaration {
		$$ = new Declaration_List({$1}, @$);
		delete_tokens($1);
	}
	;

Declaration 
	: Variable_Declaration {$$ = $1;}
	| Function_Declaration {$$ = $1;}
	| Class_Declaration {$$ = $1;}
	| Interface_Declaration {$$ = $1;}
	;
          
Variable_Declaration 
	: Variable ';' {
		//intentionally set to location of variable
		$$ = new Variable_Declaration(*$1, $1->identifier().location());
			delete_tokens($1);
		}
	;

Variable_Declaration_List
	: /*empty*/ {$$ = new Declaration_List(@$);}
	| Variable_Declaration_List Variable_Declaration {
		$$ = $1;
		$$->add_declaration(*$2);
		delete_tokens($2);
	}
	;
Variable 
	: Type T_Identifier {
		$$ = new Variable_Syntax(*$1, *$2);
		delete_tokens($1, $2);	
	}
	;
Type 
	: T_Int {$$ = make_type(Type_Types::INT);}
	| T_Double {$$ = make_type(Type_Types::DOUBLE);}
	| T_Bool {$$ = make_type(Type_Types::BOOL);}
	| T_String {$$ = make_type(Type_Types::STRING);}
	| Named_Type {$$ = $1;}
	| Type T_Dims {
		$$ = make_array_type(*$1, @1);
		delete_tokens($1);	
	}
	;
Named_Type
	: T_Identifier {
		$$ = make_named_type(*$1, @1);
		delete_tokens($1);	
	}
	;
Function_Declaration
	: Type T_Identifier '(' Formal_List ')' Statement_Block {
		$$ = make_function_declaration(*$1, *$2, *$4, *$6, @2);
		delete_tokens($1, $2, $4, $6);
	}
	| T_Void T_Identifier '(' Formal_List ')' Statement_Block {
		Type_Syntax* void_type = make_type(Type_Types::VOID);
		$$ = make_function_declaration(*void_type, *$2
			, *$4, *$6, @2);
		delete_tokens(void_type, $2, $4, $6);
	}
	;
Formal_List
	: /*empty*/ {
		$$ = new Declaration_List(@$);
	}

	| Formal_List_Helper {$$ = $1;}
	;

Formal_List_Helper
	: Variable {
		$$ = new Declaration_List(@1);
		Declaration_Syntax* variable_declaration 
			= new Variable_Declaration(*$1, @1);
		$$->add_declaration(*variable_declaration);
		delete_tokens(variable_declaration, $1);
	}
	|	Formal_List ',' Variable {
		$$ = $1;
		Declaration_Syntax* variable_declaration 
			= new Variable_Declaration(*$3, @3);
		$$->add_declaration(*variable_declaration);
		delete_tokens(variable_declaration, $3);
	}
	;
Class_Declaration
	: T_Class T_Identifier T_Extends Named_Type Class_Implements_Rule '{' Field_List '}' {
		$$ = new Class_Declaration(*$2, *$4, *$5, *$7, @$);
		delete_tokens($2, $4, $5, $7);

	}
	| T_Class T_Identifier Class_Implements_Rule '{' Field_List '}' {
		$$ =  new Class_Declaration(*$2, *$3, *$5, @$);
		delete_tokens($2, $3, $5);
	}
	;
Class_Implements_Rule
	: /*empty*/ {$$ = new Named_Type_List(@$);}
	| T_Implements Named_Type_Comma_List {$$ = $2;}
	;
Named_Type_Comma_List
	: Named_Type {
		$$ = new Named_Type_List(@$);
		$$->add_named_type(*$1);
		delete_tokens($1);
	}
	| Named_Type_Comma_List ',' Named_Type {
		$$ = $1;
		$$->add_named_type(*$3);
		delete_tokens($3);
	}
	;

Field_List
	: /*empty*/ {$$ = new Field_List(@$);}
	| Field_List Variable_Declaration {
		$$ = $1;
		$$->add_variable(*$2);
		delete_tokens($2);
	}
		| Field_List Function_Declaration {
		$$ = $1;
		$$->add_method(*$2);
		delete_tokens($2);
	}
	;


Interface_Declaration
	: T_Interface T_Identifier '{' Prototype_List '}' {
		$$ = new Interface_Declaration(*$2, *$4, @$);
		delete_tokens($2, $4);
	}
	;

Prototype_List
	: /*empty*/ { $$ = new Prototype_List(@$);}
	| Prototype_List Prototype {
		$$ = $1;
		$$->add_prototype(*$2);
		delete_tokens($2);
	}
	;

Prototype
	: Type T_Identifier '(' Formal_List ')' ';' {
		$$ = make_prototype_declaration(*$1, *$2, *$4, @2);
		delete_tokens($1, $2, $4);
	}
	| T_Void T_Identifier '(' Formal_List ')' ';' {
		Type_Syntax* void_type = make_type(Type_Types::VOID);
		$$ = make_prototype_declaration(*void_type, *$2
			, *$4, @2);
		delete_tokens(void_type, $2, $4);
	}
	;
Statement_Block
	: '{' Variable_Declaration_List Statement_List_Block_End {
		$$ = new Statement_Block(*$2, *$3);
		delete_tokens($2, $3);
	}
	;

Statement_List_Block_End
	: '}' {
		$$ = new Statement_List(@$);
	}
	| Statement_List '}' {
		$$ = $1;
	}
	;

Statement_List
	: Statement {
		$$ = new Statement_List(@$);
		$$->add_statement(*$1);
		delete_tokens($1);
	}
	| Statement_List Statement {
		$$ = $1;
		$$->add_statement(*$2);
		delete_tokens($2);
	}
	;
Statement
	: If_Dangling_Statement {$$ = $1;}
	| Non_Dangling_If_Statement {$$ = $1;}
	;

Non_Dangling_If_Statement
	: ';' {$$ = make_expression_statement(@$);}
	| Expression ';' {
		$$ = make_expression_statement(*$1, @$);
		delete_tokens($1);	
	}
	| T_Print '(' Expression_List ')' ';' {
		$$ = make_print_statement(*$3, @$);
		delete_tokens($3);	
	}
	| Return_Statement {$$ = $1;}
	| T_Break ';' {$$ = make_break_statement(@1);}
	| Statement_Block {$$ = $1;}
	| T_While '(' Expression ')' Non_Dangling_If_Statement {
		$$ = make_while_statement(*$3, *$5, @$);
		delete_tokens($3, $5);
	}
	| T_For '(' Expression_Wild ';' Expression ';' Expression_Wild ')' Non_Dangling_If_Statement {
		$$ = make_for_statement(*$3, *$5, *$7, *$9, @$);
		delete_tokens($3, $5, $7, $9);
	}
	| If_Else_Statement {$$ = $1;}
	;

If_Else_Statement
	: T_If '(' Expression ')' Non_Dangling_If_Statement T_Else Non_Dangling_If_Statement {
		$$ = make_if_statement(*$3, *$5, *$7, @$);
		delete_tokens($3, $5, $7);
	}
	;

If_Dangling_Statement
	: T_If '(' Expression ')' Statement {
		$$ = make_if_statement(*$3, *$5, @$);
		delete_tokens($3, $5);
	}
	| T_If '(' Expression ')' Non_Dangling_If_Statement T_Else If_Dangling_Statement {
		$$ = make_if_statement(*$3, *$5, *$7, @$);
		delete_tokens($3, $5, $7);
	}
	| T_While '(' Expression ')' If_Dangling_Statement {
		$$ = make_while_statement(*$3, *$5, @$);
		delete_tokens($3, $5);
	}
	| T_For '(' Expression_Wild ';' Expression ';' Expression_Wild ')' If_Dangling_Statement {
		$$ = make_for_statement(*$3, *$5, *$7, *$9, @$);
		delete_tokens($3, $5, $7, $9);
	}
	;


Return_Statement
	: T_Return Expression_Wild ';' {
		$$ = make_return_statement(*$2, @1);
		delete_tokens($2);
	}
	;
Expression_Wild
	: /*empty string*/ {$$ = new Expression_Wild(@$);}
	| Expression {
		$$ = new Expression_Wild(*$1);
		delete_tokens($1);
	}
	;
Expression 
	: Assignment_Expression {$$ = $1;}
	| Constant {$$ = $1;}
	| LValue { $$ = $1;}
	| T_This { $$ = make_this_ptr_expression(@$);}
	| Call {$$ = $1;}
	| Expression '+' Expression { 
		$$ = make_binary_arithmetic_expression(
		*$1, *$3, Binary_Operator(Operators::PLUS, @2));
		delete_tokens($1, $3);
	}
	| Expression '-' Expression { 
		$$ = make_binary_arithmetic_expression(
		*$1, *$3, Binary_Operator(Operators::MINUS, @2));
		delete_tokens($1, $3);
	}
	| Expression '*' Expression { 
		$$ = make_binary_arithmetic_expression(
		*$1, *$3, Binary_Operator(Operators::MULTIPLY, @2));
		delete_tokens($1, $3);
	}
	| Expression '/' Expression { 
		$$ = make_binary_arithmetic_expression(
		*$1, *$3, Binary_Operator(Operators::DIVIDE, @2));
		delete_tokens($1, $3);
	}
	| Expression '%' Expression { 
		$$ = make_binary_arithmetic_expression(
		*$1, *$3, Binary_Operator(Operators::MODULO, @2));
		delete_tokens($1, $3);
	}
	| Expression '<' Expression {
		$$ = new Relational_Binary_Expression (*$1, *$3
			, Relational_Binary_Operator(Operators::LESS, @2));
			delete_tokens($1, $3);
	}
	| Expression T_LessEqual Expression {
		$$ = new Relational_Binary_Expression(*$1, *$3
			, Relational_Binary_Operator(Operators::LESS_EQUAL, @2));
			delete_tokens($1, $3);
	}
	| Expression '>' Expression {
		$$ = new Relational_Binary_Expression(*$1, *$3
			, Relational_Binary_Operator(Operators::GREATER, @2));
			delete_tokens($1, $3);
	}
	| Expression T_GreaterEqual Expression {
		$$ = new Relational_Binary_Expression(*$1, *$3
			, Relational_Binary_Operator(Operators::GREATER_EQUAL, @2));
			delete_tokens($1, $3);
	}
	| Expression T_Equal Expression {
		$$ = new Relational_Binary_Expression(*$1, *$3
			, Relational_Binary_Operator(Operators::EQUAL, @2));
			delete_tokens($1, $3);
	}
	| Expression T_NotEqual Expression {
		$$ = new Relational_Binary_Expression(*$1, *$3
			, Relational_Binary_Operator(Operators::NOT_EQUAL, @2));
			delete_tokens($1, $3);
	}
	| Expression T_And Expression {
		$$ = new Relational_Binary_Expression(*$1, *$3
			, Relational_Binary_Operator(Operators::AND, @2));
			delete_tokens($1, $3);
	}
	| Expression T_Or Expression {
		$$ = new Relational_Binary_Expression(*$1, *$3
			, Relational_Binary_Operator(Operators::OR, @2));
			delete_tokens($1, $3);
	}
	| T_ReadInteger '(' ')' {$$ = make_read_integer_expression(@$);}
	| T_ReadLine '(' ')' {$$ = make_read_line_expression(@$);}
	| T_New '(' T_Identifier ')' {
		Type_Named* type = make_named_type(*$3, @3);
		$$ = make_new_expression(*type, @$);
		delete_tokens(type, $3);
	}
	| T_NewArray '(' Expression ',' Type ')' {
		$$ = make_new_array_expression(*$5, *$3, @$);
		delete_tokens($3, $5);
	}
	| '!' Expression {
		$$ = make_prefix_logical_expression(*$1, *$2);
		delete_tokens($1, $2);
	}
	| '-' Expression %prec NEGATION {
		
		$$ = make_unary_prefix_arithmetic_expression(Prefix_Operator(Prefix_Operators::MINUS, @1)
		, *$2);
		delete_tokens($2);
	}
	| '(' Expression ')' {$$ = $2;}
	;

Assignment_Expression
	: LValue '=' Expression {
		$$ = make_assignment_expression(
		*$1, *$3, Binary_Operator(Operators::ASSIGN, @2));
		delete_tokens($1, $3);
	}
	;

LValue
	: Expression '[' Expression ']' {
		$$ = new Array_Access_Expression(*$1, *$3, @$);
		delete_tokens($1, $3);
	}
	| Field_Access_Expression {$$ = $1; }
	;
Field_Access_Expression
	: T_Identifier {
		$$ = new Field_Access_Expression(*$1);
		delete_tokens($1);
	}
	| Expression '.' T_Identifier {
		$<field_access>$ = new Field_Access_Expression(*$1, *$3);
		delete_tokens($1, $3);
	}
	;

Call
	: T_Identifier '(' Expression_List_Wild ')' {
		$$ = new Call_Expression(*$1, *$3, @$);
		delete_tokens($1, $3);
	}
	| Expression '.' T_Identifier '(' Expression_List_Wild ')' {
		$$ = new Call_Expression(*$1, *$3, *$5, @$);
		delete_tokens($1, $3, $5);
	}
	;
Expression_List_Wild
	:	/*empty*/ {$$ = new Expression_List(@$);}
	| Expression_List {$$ = $1;}
	;

Expression_List
	: Expression_List_Helper {
		$$ = $<expression_list>1;
	}
	;
Expression_List_Helper
	: Expression {
		$<expression_list>$ = new Expression_List(@$);
		$<expression_list>$->add_expression(*$1);
		delete_tokens($1);
	}
	| Expression_List_Helper ',' Expression {
		$<expression_list>$ = $<expression_list>1;
		$<expression_list>$->add_expression(*$3);
		delete_tokens($3);
	}
	;
Constant
	: T_IntConstant {$$ = $1;}
	| T_DoubleConstant {$$ = make_double_constant_expression($1, @1);}
	| T_BoolConstant {$$ = make_bool_constant_expression($1, @1);}
	| T_StringConstant {$$ = make_string_constant_expression($1, @1);}
	| T_Null {$$ = make_null_constant_expression(@1);}
	;
%%

/* The closing %% above marks the end of the Rules section and the beginning
 * of the User Subroutines section. All text from here to the end of the
 * file is copied verbatim to the end of the generated y.tab.c file.
 * This section is where you put definitions of helper functions.
 */
 
/* Function: InitParser
 * --------------------
 * This function will be called before any calls to yyparse().  It is designed
 * to give you an opportunity to do anything that must be done to initialize
 * the parser (set global variables, configure starting state, etc.). One
 * thing it already does for you is assign the value of the global variable
 * yydebug that controls whether yacc prints debugging information about
 * parser actions (shift/reduce) and contents of state stack during parser.
 * If set to false, no information is printed. Setting it to true will give
 * you a running trail that might be helpful when debugging your parser.
 * Please be sure the variable is set to false when submitting your final
 * version.
 */
void InitParser()
{
   PrintDebug("parser", "Initializing parser");
   yydebug = false;
}

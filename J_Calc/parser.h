/* File: parser.h
 * --------------
 * This file provides constants and type definitions that will
 * are used and/or exported by the yacc-generated parser.
 */

#ifndef PARSER_H
#define PARSER_H

  // here we need to include things needed for the yylval union
  // (types, classes, constants, etc.)
  
#include "scanner.h"            // for MaxIdentLen
//
#include "Program_Tree.h"
//
#include "Declaration_List.h"
//
#include "Syntax_Component.h"
//
#include "Declaration_Syntax.h"
//
#include "Variable_Declaration.h"
//
#include "Variable_Syntax.h"
//
#include "Type_Syntax.h"
//
#include "Type_Factory.h"
//
#include "Declaration_Factory.h"
//
#include "Statement_Block.h"
//
#include "Statement_List.h"
//
#include "Statement_Factory.h"
//
#include "Field_Access_Expression.h"
//
#include "Expression_Factory.h"
//
#include "Binary_Operators.h"
//
#include "Relational_Binary_Operator.h"
//
#include "Call_Expression.h"
//
#include "Expression_List.h"
//
#include "Array_Access_Expression.h"
//
#include "Prefix_Operator.h"
//
#include "Statement_Syntax.h"
//
#include "Expression_Wild.h"

//
#include "Named_Type_List.h"
//
#include "Class_Declaration.h"
//
#include "Interface_Declaration.h"
//
#include "Relational_Binary_Expression.h"
//
#include "Type_Named.h"
//
#include "Identifier_Name.h"

//
#include "Symbol_Map.h"
//
#include "Field_List.h"
//
#include "Function_Declaration.h"
//
#include "Variable_Declaration.h"
//
#include "Prototype_Declaration.h"
//
#include "Prototype_List.h"
// Next, we want to get the exported defines for the token codes and
// typedef for YYSTYPE and exported global variable yylval.  These
// definitions are generated and written to the y.tab.h header file. But
// because that header does not have any protection against being
// re-included and those definitions are also present in the y.tab.c,
// we can get into trouble if we don't take precaution to not include if
// we are compiling y.tab.c, which we use the YYBISON symbol for. 
// Managing C headers can be such a mess! 



#ifndef YYBISON                
#include "y.tab.h"              
#endif

#define YY_NO_UNISTD_H
#ifdef _DEBUG


#define YYDEBUG 1
#endif

extern jomike::Program_Tree* g_program_tree;
int yyparse();              // Defined in the generated y.tab.c file
void InitParser();          // Defined in parser.y

#endif //PARSER_H

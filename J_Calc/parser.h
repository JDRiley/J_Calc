/* File: parser.h
* --------------
* This file provides constants and type definitions that will
* are used and/or exported by the yacc-generated parser.
*/

#ifndef PARSER_H
#define PARSER_H

// here we need to include things needed for the yylval union
// (types, classes, constants, etc.)

         

// Next, we want to get the exported defines for the token codes and
// typedef for YYSTYPE and exported global variable yylval.  These
// definitions are generated and written to the y.tab.h header file. But
// because that header does not have any protection against being
// re-included and those definitions are also present in the y.tab.c,
// we can get into trouble if we don't take precaution to not include if
// we are compiling y.tab.c, which we use the YYBISON symbol for. 
// Managing C headers can be such a mess! 


//
#include <J_Symbol_Fwd_Decl.h>
#include "Math_Parser.h"
//
#include <J_UI/J_UI_String.h>
#include "Math_Parser.h"
//
#include <Variable_Reference_Symbol.h>
//
#include <Variable_Symbol.h>
//
#include <Type_Syntax.h>
//
#include <Type_Factory.h>

#ifndef YYBISON                
#include "math_parsing_unit.hh"           
#endif
//
#include "Math_Lexer.h"
//
#include <Addition_Expression.h>
//
#include <Subtraction_Expression.h>
//
#include <Multiplication_Expression.h>
//
#include <Division_Expression.h>
//
#include <Unary_Negate_Expression.h>
#define YY_NO_UNISTD_H
#ifdef _DEBUG


#define YYDEBUG 1
#endif



namespace jomike {
J_FWD_DECL(Math_Parser)
extern j_symbol_component* g_input_line;
}

         // Defined in the generated y.tab.c file
void InitParser();          // Defined in parser.y

#endif //PARSER_H

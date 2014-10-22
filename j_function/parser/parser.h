/* File: parser.h
* --------------
* This file provides constants and type definitions that will
* are used and/or exported by the yacc-generated parser.
*/

#ifndef PARSER_H
#define PARSER_H



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

//#ifndef YYBISON                
//#include "math_parsing_unit.hh"           
//#endif
//
//#include "Math_Lexer.h"
//
#include "../Addition_Expression.h"
//
#include "../Subtraction_Expression.h"
//
#include "../Multiplication_Expression.h"
//
#include "../Division_Expression.h"
//
#include "../Unary_Negate_Expression.h"

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

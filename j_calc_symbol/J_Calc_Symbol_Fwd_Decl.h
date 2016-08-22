#ifndef J_CALC_SYMBOL_FWD_DECL_H
#define J_CALC_SYMBOL_FWD_DECL_H

#include <J_Fwd_Decl.h>
//
#include <j_symbol/J_Symbol_Fwd_Decl.h>

#define YY_YY_J_CALC_PARSER_STACK_HH_INCLUDED
#define YY_YY_J_CALC_PARSER_LOCATION_HH_INCLUDED

namespace jomike{

typedef js_string_t jc_string_t;

J_FWD_DECL(J_Calc_Parser)
J_FWD_DECL(J_Calc_Lexer)
J_FWD_DECL(J_Symbol_Scope)
J_FWD_DECL_LOWER(j_calc_symbol)
J_FWD_DECL(j_calc_symbol)
J_FWD_DECL(j_value)
J_FWD_DECL_LOWER(j_composite_symbol)
J_FWD_DECL_LOWER(j_routine_symbol)

J_FWD_DECL_LOWER(j_expression)
J_FWD_DECL(J_Constant_Symbol)
J_FWD_DECL(Type_Syntax)
J_STRUCT_FWD_DECL(Lexer_Location)
J_FWD_DECL_LOWER(j_declaration)
J_FWD_DECL(Value_Expression)
J_FWD_DECL(Expression_List)
J_FWD_DECL(Call_Expression)

J_FWD_DECL(Arguments)
J_FWD_DECL(Type_Routine)
J_FWD_DECL(Math_Parser)
J_FWD_DECL(Math_Lexer)
J_FWD_DECL(Statement_Block)
J_FWD_DECL(j_statement)
J_FWD_DECL(If_Statement)
template<typename Symbol_t>
class Specific_Symbol_List;

typedef Specific_Symbol_List<j_calc_symbol> J_Calc_Symbol_List;
typedef Specific_Symbol_List<j_declaration> Declaration_List;
typedef Specific_Symbol_List<Type_Syntax> Type_Syntax_List;

typedef j_symbol_Shared_t J_Symbol_Shared_t;
typedef j_value_Shared_t J_Value_Shared_t;
}

#endif //J_CALC_SYMBOL_FWD_DECL_H

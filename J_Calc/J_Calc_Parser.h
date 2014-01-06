#ifndef J_CALC_PARSER_H
#define J_CALC_PARSER_H

#include "J_Calc_Fwd_Decl.h"
#include <J_Utile.h>
#include <J_Fwd_Decl.h>
#include <J_UI/J_UI_String.h>
//
#include <J_Gadget_Classes.h>
//
#include <map>
//
#include <ex_array.h>
namespace jomike{
J_FWD_DECL(j_routine_symbol)
J_FWD_DECL(j_value)
J_FWD_DECL(j_symbol)
typedef Delimiter_Handler<J_UI_Char> Delim_t;
class J_Calc_Math_Input_Parser{
public:
	void convert_to_proper_math_input(J_UI_String*);
	J_UI_String evaluate_math_input(const J_UI_String&);
private:
	static J_Calc_Math_Input_Parser& get_instance();
	J_Calc_Math_Input_Parser();
	typedef J_Calc_Math_Input_Parser Math_Parser_t;
	friend class Instance_Pointer<J_Calc_Math_Input_Parser>;

	typedef j_symbol* (J_Calc_Math_Input_Parser::*Declaration_Cmd_t)
		(J_UI_Const_Iter*, J_UI_Const_Iter);


	std::map<J_UI_String, Declaration_Cmd_t> M_declaration_functions;


	j_routine_symbol* get_routine(J_UI_String::const_iterator*
		, J_UI_String::const_iterator);

	typedef ex_array<J_Symbol_Shared_t> Symbol_Cont_t;
	j_symbol* dbl_declaration_parser(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos);
	Symbol_Cont_t get_declared_symbols(J_UI_Const_Iter* i_pos
		, J_UI_Const_Iter end_scope_pos);

	j_symbol* symbol_declaration_parser(J_UI_Const_Iter* i_pos, J_UI_String::const_iterator i_end_pos);

	J_UI_String dbl_declaration_cmd(const J_UI_String&);

	void check_end_state_and_advance(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos
		, bool i_end_pos_ok, const Delimiter_Handler<J_UI_Char>& irk_delims
		, const char* const ik_message);

	J_UI_String get_statement(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos);
};

J_UI_String::const_iterator get_closing_brace(J_UI_String::const_iterator ik_pos);

}

#endif
#ifndef J_CALC_PARSER_H
#define J_CALC_PARSER_H

#include "J_Calc_Fwd_Decl.h"
#include <J_Utile.h>
#include <J_Fwd_Decl.h>
#include <J_UI/J_UI_String.h>
//
#include <map>
//
#include <ex_array.h>
namespace jomike{
J_FWD_DECL(J_Routine_Symbol)
J_FWD_DECL(j_value)
J_FWD_DECL(j_symbol)

class J_Calc_Math_Input_Parser{
public:
	void convert_to_proper_math_input(J_UI_String*);
	J_UI_String evaluate_math_input(const J_UI_String&);
private:
	static J_Calc_Math_Input_Parser& get_instance();
	J_Calc_Math_Input_Parser();
	typedef J_Calc_Math_Input_Parser Math_Parser_t;
	friend class Instance_Pointer<J_Calc_Math_Input_Parser>;

	typedef J_UI_String (J_Calc_Math_Input_Parser::*Declaration_Cmd_t)
		(const J_UI_String&);


	std::map<J_UI_String, Declaration_Cmd_t> M_declaration_functions;


	J_Routine_Symbol* get_routine(J_UI_String::const_iterator*
		, J_UI_String::const_iterator);

	j_symbol* symbol_declaration_parser(J_UI_Const_Iter* i_pos, J_UI_String::const_iterator i_end_pos);

	J_UI_String dbl_declaration_cmd(const J_UI_String&);


};

}

#endif
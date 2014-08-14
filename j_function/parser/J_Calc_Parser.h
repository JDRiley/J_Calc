#ifndef J_CALC_PARSER_H
#define J_CALC_PARSER_H

#include "J_Symbol_Fwd_Decl.h"
//
#include <J_Utile.h>
//
#include <J_UI/J_UI_Fwd_Decl.h>


namespace jomike{

j_symbol* get_symbol(const J_UI_String& irk_string);

void add_user_symbol(j_symbol*);



class J_Calc_Math_Input_Parser{
public:
	void convert_to_proper_math_input(J_UI_String*);
	J_UI_String evaluate_math_input(const J_UI_Multi_String&);
private:
	static J_Calc_Math_Input_Parser& get_instance();
	J_Calc_Math_Input_Parser();
	typedef J_Calc_Math_Input_Parser Math_Parser_t;
	friend class Instance_Pointer<J_Calc_Math_Input_Parser>;


};


}

#endif
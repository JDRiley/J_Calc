#ifndef J_FUNCTION_FACTORY_H
#define J_FUNCTION_FACTORY_H
#include <J_Error.h>
//
#include <J_UI/J_UI_String.h>
//
#include "Arguments.h"
//
#include <J_Fwd_Decl.h>
//
#include <J_Gadget_Classes.h>

namespace jomike{

class j_symbol;

class J_Symbol_Syntax_Error : public J_Syntax_Error{
public:
	J_Symbol_Syntax_Error(const char* const ik_message);
};

j_symbol* create_j_symbol(const J_UI_String&);

j_symbol* create_multi_j_symbol(J_UI_String::const_iterator i_start_pos
	, J_UI_String::const_iterator i_end_pos
	, J_UI_String::const_iterator* i_end_symbol_pos
	, const Delimiter_Handler<J_UI_Char>& irk_delim);

j_symbol* create_j_symbol(J_UI_String::const_iterator i_begin_pos, J_UI_String::const_iterator i_end_pos
	, J_UI_String::const_iterator* i_end_symbol_pos);

/*bool check_parenthesis(const J_UI_String&)*/
/*
	Checks the string for proper parenthesis alignment
*/
void check_parenthesis_ex(const J_UI_String& );

J_UI_String::const_iterator get_closing_parenthesis(J_UI_String::const_iterator); 

J_UI_String::const_iterator
get_closing_bracket(J_UI_String::const_iterator ik_pos);



//TODO Define this
bool is_reserved_symbol(const J_UI_String&);
//end define

extern const Delimiter_Handler<J_UI_Char> gk_symbol_delimiter;
//
//template<typename Iter>
//Iter get_symbol_name(Iter i_pos, Iter i_end){
//
//	advance_white_space(&i_pos, i_end);
//	auto found_pos = std::find_if(i_pos, i_end
//		, [](J_UI_Char i_char){
//		return gk_symbol_delimiter.is_delim(i_char);
//	});
//
//	return found_pos;
//
//}


template<typename Iter>
J_UI_String get_symbol_name(Iter* i_pos, Iter i_end){

	advance_white_space(i_pos, i_end);
	auto start_pos = *i_pos;
	*i_pos = std::find_if(*i_pos, i_end
		, [](J_UI_Char i_char){
		return gk_symbol_delimiter.is_delim(i_char);
	});
	
	return J_UI_String(start_pos, *i_pos);

}


}
#endif
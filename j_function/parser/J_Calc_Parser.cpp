#include "J_Calc_Parser.h"
//
#include <J_Utile.h>

//
#include <j_symbol.h>

//
#include <J_UI/J_UI_Multi_String.h>
//Algorithm
#include <algorithm>
//
#include <functional>
//
#include <J_String.h>
//
#include <type_traits>
//
#include "Math_Parser.h"
//
#include <J_Symbol_Error.h>
using std::bind; using std::equal_to; using std::none_of;
using namespace std::placeholders; using std::to_string;

//Containers
#include <array>
using std::string;
using std::array;



//IO
#include <iostream>
using std::cerr; using std::endl;

//Utilities 
#include <cassert>
namespace jomike{



void J_Calc_Math_Input_Parser::convert_to_proper_math_input(J_UI_String* i_string){
	int new_line_count = 0;
	for(auto string_pos = i_string->begin(); string_pos != i_string->end(); ++string_pos){
		if(J_UI_Char('\n') == *string_pos){
			++new_line_count;
		}

		if(!string_pos->is_space()){
			break;
		}
	}
	i_string->white_space_normalize();
	for(int i = 0; i < new_line_count; ++i){
		i_string->push_front('\n');
	}
};

J_Calc_Math_Input_Parser::J_Calc_Math_Input_Parser(){

}

J_Calc_Math_Input_Parser& J_Calc_Math_Input_Parser::get_instance(){
	static J_Calc_Math_Input_Parser parser;
	return parser;
}





J_UI_String J_Calc_Math_Input_Parser::evaluate_math_input(const J_UI_Multi_String& irk_string){

	Math_Parser parser;
	string error_string;
	
	try{
		if(j_symbol* new_symbol_raw = parser.parse(irk_string.std_str())){
			j_symbol_unique_t new_symbol(new_symbol_raw);
			J_UI_String return_val((J_Font_Face)nullptr);
			new_symbol->process();
			if(new_symbol->has_value()){
				
				return_val = new_symbol->get_value().to_str();
			} else{
				return_val = new_symbol->get_display_name();
			}

			return return_val;
		} else{
			return error_string;
		}
	}catch(J_Syntax_Error& er_error){
		er_error.print();
		return er_error.str();
	} catch(J_Value_Error& er_error){
		er_error.print();
		return er_error.str();
	} catch(J_Sym_Argument_Error& er_error){
		er_error.print();
		return er_error.str();
	}catch(J_Symbol_Error& er_error){
		er_error.print();
		return er_error.str();
	}
}


J_UI_String::const_iterator get_closing_brace(J_UI_String::const_iterator ik_pos){
	if('{' != ik_pos->charcode()){
		return ik_pos;
	}
	int brace_count = 0;
	while(j_true){
		switch(ik_pos->charcode()){
		case '{': ++brace_count; break;
		case '}': --brace_count; break;
		default:
			;
		}
		if(!brace_count){
			return ik_pos;
		} else{
			++ik_pos;
		}
	}
	assert(0);
	return J_UI_String::const_iterator();
}

}
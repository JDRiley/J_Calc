#include "j_function_factory.h"
//
#include <j_composite_symbol.h>
//
#include <j_number_symbol.h>
//
#include "J_Calc_Data.h"
//
#include <J_UI/J_UI_Fwd_Decl.h>
//
#include <J_UI/J_UI_String.h>
//
#include <j_type.h>
//
#include <J_Gadget_Classes.h>

//Algorithm
#include <algorithm>
//
#include <functional>

using std::bind; using std::find; using std::for_each; using std::transform;
using namespace std::placeholders;

//Containers

using std::string;

//IO
#include <iostream>

using std::cerr; using std::endl;

//Math
#include <numeric>


using std::numeric_limits;

namespace jomike{
static Instance_Pointer<J_Calc_Data> s_calc_data;

static bool is_char_allowed(J_UI_Char i_char){
	return (i_char.is_alpha_numeric() || (i_char == '_'));
}


/*J_Symbol_Syntax_Error Constructor*/
J_Symbol_Syntax_Error::J_Symbol_Syntax_Error(const char* const ik_message)
	:J_Syntax_Error(ik_message){}

const J_Char_t END_CHAR = ';';

const ex_array<J_UI_Char> gsk_symbol_delimiters
	= {' ', '\t', '\n', '\0', ';', ':', '(', '-', ',', '+', '=', '[', ']'};

extern const Delimiter_Handler<J_UI_Char> gk_symbol_delimiter(gsk_symbol_delimiters.begin()
	, gsk_symbol_delimiters.end());


/*j_symbol* create_j_symbol(const J_UI_String&)*/
j_symbol* create_j_symbol(const J_UI_String& irk_string){
	J_UI_String normalized_str(irk_string);
	normalized_str.white_space_normalize();
	normalized_str.push_back(END_CHAR);

	cerr << "\nOriginal Str: " << irk_string
		<< "\nNormalized Str: " << normalized_str << endl;
	check_parenthesis_ex(normalized_str);

	j_composite_symbol_unique_t new_symbol(new j_composite_symbol);

	auto string_pos = normalized_str.cbegin();
	auto symbol_start = string_pos;
	bool creating_symbol = false;

	j_composite_symbol::Join_Type join_type = j_composite_symbol::Join_Type::ADD;

	while(string_pos != normalized_str.end()){

		J_UI_Char c = *string_pos;
		if(c.is_alpha()){
			symbol_start = creating_symbol ? symbol_start : string_pos;
			creating_symbol = true;
		}

		J_UI_String::const_iterator new_it(normalized_str.begin());


		++new_it;
		if(c.is_number() && !creating_symbol){
			j_number_symbol new_num_symbol(read_double_and_advance(&string_pos, normalized_str.end() - string_pos));
			new_symbol->append(join_type, new_num_symbol);
			continue;
		}

		if(is_char_allowed(c)){
			++string_pos;
			continue;
		}

		if(creating_symbol){
			J_UI_String symbol_string(symbol_start, string_pos);
			if(is_reserved_symbol(symbol_string)){
				new_symbol->append(join_type, *s_calc_data->get_reserved_symbol(symbol_string));
			} else{
				new_symbol->append(join_type, *s_calc_data->get_symbol(symbol_string));
			}
		}

		switch(c.charcode()){

		case '+': join_type = j_composite_symbol::Join_Type::ADD; break;

		case '/': join_type = j_composite_symbol::Join_Type::DIVIDE; break;

		case '*': join_type = j_composite_symbol::Join_Type::MULTIPLY; break;
		case '-': join_type = j_composite_symbol::Join_Type::SUBTRACT; break;
		case '(': {
						auto closing_pos = get_closing_parenthesis(string_pos);
						if(creating_symbol){
							Arguments args = get_args_from_string(J_UI_String(string_pos + 1, closing_pos));
							(*new_symbol)[new_symbol->size() - 1].set_args(std::move(args));
						} else{
							new_symbol->append(join_type
								, *j_symbol_unique_t(create_j_symbol(J_UI_String(string_pos + 1, closing_pos))));
						}

						string_pos = closing_pos + 1;
		}
			break;
		case END_CHAR:
			break;
		case ' ': /*suspiciously empty here*/;; break;
		default:
			throw J_Symbol_Syntax_Error("Could Not Parse a char");
		}
		creating_symbol = false;
		++string_pos;
	}
	if(!new_symbol->size()){
		throw J_Symbol_Syntax_Error("Empty Composite Encountered");
	}
	return new_symbol.release();
}

j_symbol* create_multi_j_symbol(J_UI_String::const_iterator* i_pos
	, J_UI_String::const_iterator i_end_pos
	, const Delimiter_Handler<J_UI_Char>& irk_delim){


	j_symbol* new_arg = create_j_symbol(i_pos, i_end_pos);

	advance_white_space(i_pos, i_end_pos);

	if((*i_pos == i_end_pos)
		|| (irk_delim.is_delim((*i_pos)->charcode()))){
		return new_arg;
	}




	j_composite_symbol_unique_t new_composite_symbol(new j_composite_symbol);
	new_composite_symbol->append(j_composite_symbol::Join_Type::ADD, *new_arg);
	delete new_arg;
	while(!irk_delim.is_delim((*i_pos)->charcode())){
		auto join_type = get_composite_symbol_join_type(*(*i_pos++));
		new_arg = create_j_symbol(i_pos, i_end_pos);

		new_composite_symbol->append(join_type, *new_arg);

		delete new_arg;
		if(*i_pos == i_end_pos){
			break;
		}
		advance_white_space(i_pos, i_end_pos);
	}


	return new_composite_symbol.release();



}



/*j_symbol* create_j_symbol(const J_UI_String&)*/
j_symbol* create_j_symbol(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos){
	J_UI_String test_string(*i_pos, i_end_pos);
	cerr << "\nOriginal Str: " << test_string << endl;
	check_parenthesis_ex(test_string);


	bool creating_symbol = false;
	j_symbol* symbol = nullptr;
	J_UI_String symbol_string;
	advance_white_space(i_pos, i_end_pos);
	if((*i_pos)->is_alpha()){
		creating_symbol = true;
		symbol_string = get_symbol_name(i_pos, i_end_pos);
	}

	if((*i_pos)->is_number() || (**i_pos == '-')){
		assert(!creating_symbol);
		symbol = new j_number_symbol(read_double_and_advance(i_pos, i_end_pos - *i_pos));
		return symbol;
	}

	if(creating_symbol){
		if(is_reserved_symbol(symbol_string)){
			symbol = s_calc_data->get_reserved_symbol(symbol_string)->get_copy();
		} else{
			symbol = s_calc_data->get_symbol(symbol_string)->get_copy();
		}
	}
	advance_white_space(i_pos, i_end_pos);

	switch((*i_pos)->charcode()){
	case '(': {
		auto closing_pos = get_closing_parenthesis(*i_pos);
		if(creating_symbol){
			assert(symbol);
			symbol->set_args(get_args_from_string(J_UI_String(*i_pos + 1, closing_pos)));
		} else{
			assert(!symbol);
			return create_j_symbol(i_pos, closing_pos);
		}

		*i_pos = closing_pos + 1;
	}
		break;
	default:
		;
	}


	if(!symbol){
		throw J_Symbol_Syntax_Error("Empty Symbol Encountered");
	}

	return symbol;
}



/*bool check_parenthesis_ex(const Display_STring& irk_string)*/
void check_parenthesis_ex(const J_UI_String& irk_string){
	int open_parenthesis_count = 0;
	int open_bracket_count = 0;
	int open_square_bracket_count = 0;

	for_each(irk_string.char_begin(), irk_string.char_end(), [&](const char y_char){
		switch(y_char){
		case '(': ++open_parenthesis_count; break;
		case ')': --open_parenthesis_count; break;
		case '{': ++open_bracket_count; break;
		case '}': --open_bracket_count; break;
		case '[': ++open_square_bracket_count; break;
		case ']': --open_square_bracket_count; break;
		default:
			;
		}

		if(open_bracket_count < 0){
			throw J_Symbol_Syntax_Error("Unmatched '}' bracket!");
		}
		if(open_parenthesis_count < 0){
			throw J_Symbol_Syntax_Error("Unmatched ')' bracket!");
		}
		if(open_square_bracket_count < 0){
			throw J_Symbol_Syntax_Error("Unmatched ']' bracket!");
		}
	}
	);

	if(open_bracket_count){
		throw J_Symbol_Syntax_Error("Unmatched '{' bracket!");
	}
	if(open_parenthesis_count){
		throw J_Symbol_Syntax_Error("Unmatched '(' bracket!");
	}
	if(open_square_bracket_count){
		throw J_Symbol_Syntax_Error("Unmatched '[' bracket!");
	}

}


J_UI_String::const_iterator get_closing_parenthesis(J_UI_String::const_iterator ik_pos){

	if('(' != ik_pos->charcode()){
		return ik_pos;
	}
	int parenthesis_count = 0;
	while(j_true){
		switch(ik_pos->charcode()){
		case '(': ++parenthesis_count; break;
		case ')': --parenthesis_count; break;
		default:
			;
		}
		if(!parenthesis_count){
			return ik_pos;
		} else{
			++ik_pos;
		}
	}
	assert(0);
	return J_UI_String::const_iterator();
}


Arguments get_args_from_string(const J_UI_String& /*irk_string*/){
	assert(!"Deprecated");
	return Arguments();
}


J_UI_String::const_iterator	find_next_arg(J_UI_String::const_iterator i_start_pos
	, J_UI_String::const_iterator i_end_pos){
	const J_UI_Char k_arg_end_char(',');
	J_UI_String::const_iterator end_arg_pos = i_start_pos;

	while(end_arg_pos < i_end_pos){
		end_arg_pos = find(i_start_pos, i_end_pos, k_arg_end_char);

		auto paren_pos = find(i_start_pos, i_end_pos, J_UI_Char('('));

		if(end_arg_pos <= paren_pos){
			//If they are equal it must be i_end_pos
			assert((end_arg_pos < paren_pos) || (end_arg_pos == i_end_pos));
			break;
		}
		i_start_pos = ++get_closing_parenthesis(paren_pos);
	}

	return end_arg_pos;

}




//j_dbl read_double_and_advance(J_UI_String::const_iterator* i_string_pos, ptrdiff_t i_max_len){
//	auto& string_pos = *i_string_pos;
//
//	string new_string(J_UI_String::to_char_iterator(string_pos)
//		, J_UI_String::to_char_iterator(string_pos +i_max_len));
//
//	char* end_char_pos;
//	Dbl_t return_val = strtod(new_string.c_str(), &end_char_pos);
//	string_pos += safe_int_cast(end_char_pos - &new_string[0]);
//	
//	return return_val;
//}

}


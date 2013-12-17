#include "J_Calc_Parser.h"
#include "J_Calc_Data.h"
#include <J_Utile.h>
#include <j_function_factory.h>
#include <j_function.h>
#include "J_Calc_Error.h"
//Algorithm
#include <algorithm>
#include <functional>
//
#include <J_String.h>
#include <type_traits>
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

static Instance_Pointer<J_Calc_Data> s_calc_data;

static const J_UI_String sk_assignment_string("->");

const ex_array<J_UI_Char> srk_statement_delims = {';', ':'};
const Delimiter_Handler<J_UI_Char> sk_statement_delim(srk_statement_delims.begin()
	, srk_statement_delims.end());
const ex_array<J_UI_Char> srk_arg_delims = {','};

const Delimiter_Handler<J_UI_Char> srk_arg_delim(srk_arg_delims.begin(), srk_arg_delims.end());
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
	M_declaration_functions[to_display_string("Dbl")] = &Math_Parser_t::dbl_declaration_cmd;
}

J_Calc_Math_Input_Parser& J_Calc_Math_Input_Parser::get_instance(){
	static J_Calc_Math_Input_Parser parser;
	return parser;
}
Arguments get_arguments(const J_UI_String& irk_string){
	check_parenthesis_ex(irk_string);
	J_UI_String argument_string(irk_string.cbegin(),
		get_closing_parenthesis(irk_string.begin()));
	argument_string.pop_front();
	return Arguments(argument_string);
}

static bool is_assignment_statement(const J_UI_String& irk_string){
	auto begin_pos = irk_string.begin();
	advance_white_space(&begin_pos, irk_string.end());


	if(irk_string.size() < 2){
		return false;
	}
	return equal(sk_assignment_string.begin(), sk_assignment_string.end(), irk_string.begin());
}


static j_symbol* assign_symbol(j_symbol*, const J_UI_String&);

J_UI_String J_Calc_Math_Input_Parser::evaluate_math_input(const J_UI_String& irk_string){
	auto string_pos = irk_string.begin();
	J_UI_String cur_word;
	
	advance_white_space(&string_pos, irk_string.end());
	if(J_UI_Char('[') == *string_pos){
		//Declaring a routine
		J_Routine_Symbol_Unique_t new_routine(
				get_routine(&string_pos, irk_string.end())
			);

		s_calc_data->add_user_symbol(&*new_routine);
		return new_routine->name();
	} 

	string_pos = get_symbol_name(string_pos, irk_string.end(), &cur_word);
	
	cerr << "Math Input Received word: " << cur_word << endl;

	J_UI_String remaining_string(irk_string.substr(string_pos - irk_string.begin()));
	remaining_string.push_back(';');
	j_symbol* symbol = nullptr;
	try{
		if(M_declaration_functions.count(cur_word)){
			remaining_string.pop_front();
			assert(!symbol);
			return (this->*M_declaration_functions[cur_word])(remaining_string);
		} else if(is_assignment_statement(remaining_string)){
			assert(!symbol);
			symbol = get_symbol(cur_word);
			symbol = assign_symbol(symbol, remaining_string);
			return to_string(symbol->value());
		} else{
			auto temp_symbol = J_Symbol_Unique_t(create_j_symbol(irk_string));
			return to_string(temp_symbol->value());
		}

		
	} catch(J_Syntax_Error& e){
		return J_UI_String("Symbol Syntax Error: " + e.str());
	} catch(J_Argument_Error& e){
		return J_UI_String("Argument Error: " + e.str());
	}
}

J_UI_String J_Calc_Math_Input_Parser::dbl_declaration_cmd(const J_UI_String& irk_input){

	cerr << "\nDbl declaration input: " << irk_input;
	assert(!gk_symbol_delimiter.is_delim(irk_input[0]));

	switch(irk_input[0].charcode()){
	case '~':
		//Handle Units here. '~' denotes the units of the dbl being declared
		assert(0);
	default:
		;
	}
	J_UI_String symbol_name;
	auto string_pos = irk_input.begin();

	string_pos = get_symbol_name(string_pos, irk_input.end(), &symbol_name);

	j_symbol* new_symbol = nullptr;
	try{
		if(!s_calc_data->symbol_name_availability_status(symbol_name.lu_str())){
			throw J_Syntax_Error("Symbol Name Unavailable");
		}




		advance_white_space(&string_pos, irk_input.end());
		switch(string_pos++->charcode()){
		case '=':
			new_symbol = create_multi_j_symbol(string_pos, irk_input.end()
				, &string_pos, sk_statement_delim);
			break;
		case ';':
			new_symbol = new j_number_symbol(symbol_name);
			break;
		default:
			throw J_Syntax_Error("Unexpected Character Encountered");
		}

		new_symbol->set_name(symbol_name);
		s_calc_data->add_user_symbol(new_symbol);
		J_UI_String display_name(new_symbol->get_display_name());
		delete new_symbol;
		return display_name;

	} catch(J_Error& e){
		delete new_symbol;
		return e.str();
	}


}


j_symbol* dbl_declaration_parser(J_UI_Const_Iter*, J_UI_Const_Iter);


J_Routine_Symbol* J_Calc_Math_Input_Parser
	::get_routine(J_UI_String::const_iterator* i_start_pos
	, J_UI_String::const_iterator i_end_pos){
		assert(J_UI_Char('[') == **i_start_pos);
		auto i_pos = *i_start_pos;
		auto end_scope_pos = get_closing_bracket(i_pos);
		advance_white_space(&++i_pos, i_end_pos);
		J_Routine_Symbol_Unique_t routine_symbol(new J_Routine_Symbol);
		while(i_pos < end_scope_pos){

			i_pos = symbol_declaration_parser(i_pos, i_end_pos, routine_symbol, end_scope_pos);

		}
		assert(J_UI_Char(']') == *i_pos);
		advance_white_space(&++i_pos, i_end_pos);


		J_UI_String symbol_name;
		i_pos = get_symbol_name(i_pos, i_end_pos, &symbol_name);
		routine_symbol->set_name(symbol_name);

		if(i_pos == i_end_pos){
			throw J_Syntax_Error("Args for routine are unspecified");
		}




		return routine_symbol.release();
}

j_symbol* J_Calc_Math_Input_Parser::symbol_declaration_parser(J_UI_Const_Iter* i_pos
	, J_UI_String::const_iterator i_end_pos){
	
	auto& string_pos = *i_pos;
	if(J_UI_Char('[') == *string_pos){
		//Declaring a routine
		J_Routine_Symbol* new_routine = get_routine(&string_pos, i_end_pos);
		advance_white_space(&string_pos, i_end_pos);
		assert(*i_pos == string_pos);
		return new_routine;
	}

	J_UI_String cur_word = get_symbol_name(i_pos, i_end_pos);

	if(cur_word.std_str() == "Dbl"){
		advance_white_space(i_pos, i_end_pos);
		dbl_declaration_parser(routine_symbol.get(), i_pos
			, end_scope_pos, &i_pos);
		if(i_pos == end_scope_pos){
			continue;
		}
		assert(srk_arg_delim.is_delim(*i_pos));
		advance_white_space(&++i_pos, end_scope_pos);
	} else{
		throw J_Syntax_Error("Could not determine scope declaration");
	}	return i_pos;
	assert(!"The method or operation is not yet check.");
}

j_symbol* dbl_declaration_parser(J_UI_Const_Iter*, J_UI_Const_Iter){

	cerr << "\nDbl declaration input: " << J_UI_String(i_start_pos, i_end_pos);
	assert(!gk_symbol_delimiter.is_delim(*i_start_pos));

	switch(i_start_pos->charcode()){
	case '~':
		//Handle Units here. '~' denotes the units of the dbl being declared
		assert(0);
	default:
		;
	}
	J_UI_String symbol_name;
	auto string_pos = i_start_pos;

	string_pos = get_symbol_name(string_pos, i_end_pos, &symbol_name);

	j_symbol* new_symbol = nullptr;
	try{
		if(!i_routine_symbol->symbol_name_availability_status(symbol_name)){
			throw J_Syntax_Error("Symbol Name Unavailable");
		}




		advance_white_space(&string_pos, i_end_pos);
		switch(string_pos++->charcode()){
		case '=':
			new_symbol = create_multi_j_symbol(string_pos, i_end_pos
				, &string_pos, srk_arg_delim);
			break;
		case ',':
			new_symbol = new j_number_symbol(symbol_name);
			break;
		default:
			throw J_Syntax_Error("Unexpected Character Encountered");
		}

		new_symbol->set_name(symbol_name);
		i_routine_symbol->add_scope_symbol(*new_symbol);
		J_UI_String display_name(new_symbol->get_display_name());
		delete new_symbol;
		*i_end_symbol_pos = string_pos;
	} catch(J_Error& e){
		delete new_symbol;
		throw e;
	}


}




j_symbol* assign_symbol(j_symbol* i_symbol, const J_UI_String& irk_string){
	J_UI_String symbol_name = i_symbol->name();
	if(!s_calc_data->is_user_symbol(symbol_name)){
		throw J_Syntax_Error("Cannot Assign to non user symbol");
	}

	auto string_pos = irk_string.begin();
	advance_white_space(&string_pos, irk_string.end());
	assert(equal(sk_assignment_string.begin(), sk_assignment_string.end(), string_pos));

	string_pos += sk_assignment_string.size();

	auto symbol = create_j_symbol(irk_string.substr(string_pos - irk_string.begin()));
	symbol->set_name(symbol_name);

	s_calc_data->remove_user_symbol(symbol_name);
	s_calc_data->add_user_symbol(symbol);
	delete symbol;
	return s_calc_data->get_user_symbol(symbol_name);
}

}
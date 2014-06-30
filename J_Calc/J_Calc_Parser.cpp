#include "J_Calc_Parser.h"
//
#include "J_Calc_Data.h"
//
#include <J_Utile.h>
//
#include "j_function_factory.h"
//
#include <j_symbol.h>
//
#include <j_number_symbol.h>
//
#include <j_routine_symbol.h>
//
#include "J_Calc_Error.h"
//Algorithm
#include <algorithm>
//
#include <functional>
//
#include <J_String.h>
//
#include <type_traits>
//
#include "j_conditional_construct_symbol.h"
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

static Instance_Pointer<J_Calc_Data> s_calc_data;

static const J_UI_String sk_assignment_string("->");

const ex_array<J_UI_Char> srk_statement_delims = {';', ':', '}'};
const Delimiter_Handler<J_UI_Char> sk_statement_delim(srk_statement_delims.begin()
	, srk_statement_delims.end());
const ex_array<J_UI_Char> srk_arg_delims = {',', ']', ';'};

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
	M_declaration_functions["Dbl"] 
		= &Math_Parser_t::dbl_declaration_parser;
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
	return get_args_from_string(argument_string);
}






J_UI_String J_Calc_Math_Input_Parser::evaluate_math_input(const J_UI_Multi_String& irk_string){

	Math_Parser parser;
	string error_string;
	
	try{
		if(j_symbol* new_symbol = parser.parse(irk_string.std_str() + ';')){
			J_UI_String return_val;
			if(new_symbol->has_value()){
				return_val = new_symbol->get_value().to_str();
			} else{
				return_val = new_symbol->get_display_name();
			}
			delete new_symbol;
			return return_val;
		} else{
			return error_string;
		}
	}catch(J_Syntax_Error& er_error){
		er_error.print();
		return er_error.str();
	}catch(J_Sym_Argument_Error& er_error){
		er_error.print();
		return er_error.str();
	}catch(J_Symbol_Error& er_error){
		er_error.print();
		return er_error.str();
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
	
	auto string_pos = irk_input.begin();

	J_UI_String symbol_name = get_symbol_name(&string_pos, irk_input.end());

	j_symbol* new_symbol = nullptr;
	try{
		if(!s_calc_data->symbol_name_availability_status(symbol_name.lu_str())){
			throw J_Syntax_Error("Symbol Name Unavailable");
		}




		advance_white_space(&string_pos, irk_input.end());
		switch(string_pos++->charcode()){
		case '=':
			new_symbol = create_multi_j_symbol(&string_pos, irk_input.end()
				, sk_statement_delim);
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

J_UI_String J_Calc_Math_Input_Parser::get_statement(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos){
	int if_statement_depth = 0;
	J_UI_String statement_string;
	while(*i_pos < i_end_pos){
		if(!if_statement_depth && **i_pos == ':'){
			break;
		}

		if(if_statement_depth && **i_pos == '!'){
			--if_statement_depth;
		}

		if(**i_pos == '?'){
			++if_statement_depth;
		}

		statement_string.push_back(*(*i_pos)++);
	}
	return statement_string;
}

j_routine_symbol* J_Calc_Math_Input_Parser
::get_routine(J_UI_String::const_iterator* i_pos
	, J_UI_String::const_iterator i_end_pos){
	assert(J_UI_Char('[') == **i_pos);



	
	cerr << "\nRoutine String: " << J_UI_String(*i_pos, i_end_pos);
	j_routine_symbol_shared_t routine_symbol(new j_routine_symbol);

	//Add Scope Symbols
	{
		auto end_scope_pos = get_closing_bracket(*i_pos);
		advance_white_space(&++*i_pos, i_end_pos);
		Symbol_Cont_t scope_symbols(get_declared_symbols(i_pos, end_scope_pos));

		for(auto f_symbol : scope_symbols){
			routine_symbol->add_scope_symbol(*f_symbol);
		}
	}

	assert(J_UI_Char(']') == **i_pos);
	advance_white_space(&++*i_pos, i_end_pos);


	J_UI_String symbol_name = get_symbol_name(i_pos, i_end_pos);

	routine_symbol->set_name(symbol_name);
	advance_white_space(i_pos, i_end_pos);

	if((*i_pos == i_end_pos) || (**i_pos != '[')){
		throw J_Syntax_Error("Args for routine are unspecified");
	}

	//Add Argument Symbols
	{
		auto end_args_pos = get_closing_bracket(*i_pos);
		++*i_pos;
		Symbol_Cont_t arg_symbols(get_declared_symbols(i_pos, end_args_pos));

		for(auto f_symbol : arg_symbols){
			routine_symbol->add_arg_symbol(*f_symbol);
		}
	}
	assert(J_UI_Char(']') == **i_pos);
	advance_white_space(&++*i_pos, i_end_pos);

	if((i_end_pos <= ((*i_pos) + 2))
		|| !equal(sk_assignment_string.begin(), sk_assignment_string.end(), *i_pos)){
		throw J_Syntax_Error("Could Not Read Routine Return Type");
	}
	*i_pos += 2;
	advance_white_space(i_pos, i_end_pos);

	if(**i_pos != '['){
		throw J_Syntax_Error("Could Not Start Interface Declaration Scope");
	}

	{
		auto end_interface_pos = get_closing_bracket(*i_pos);
		advance_white_space(&++*i_pos, i_end_pos);
		j_symbol_unique_t return_symbol(symbol_declaration_parser(i_pos, i_end_pos));
		routine_symbol->set_return_symbol(*return_symbol);
		advance_white_space(i_pos, i_end_pos);
		check_end_state_and_advance(i_pos, end_interface_pos, true, Delim_t({','})
			, "Could Not Read Rest OF Interface");

		Symbol_Cont_t interface_symbols(get_declared_symbols(i_pos, end_interface_pos));
		for(auto f_symbol : interface_symbols){
			routine_symbol->add_interface_symbol(*f_symbol);
		}
	}

	check_end_state_and_advance(i_pos, i_end_pos, false, Delim_t({']'})
		, "Could Not Start Statement Read");

	{
		auto end_statements_pos = get_closing_brace(*i_pos);
		check_end_state_and_advance(i_pos, end_statements_pos, false, Delim_t({'{'})
			, "Could Not Read Rest OF Interface");

		while(*i_pos < end_statements_pos){
			J_UI_String statement_string = get_statement(i_pos, end_statements_pos);
			cerr << "\nStatement String: " << statement_string;
			routine_symbol->add_statement(statement_string);
			check_end_state_and_advance(i_pos, i_end_pos, true, sk_statement_delim
				, "Could Not Add Routine Statement");

		}
		if((*i_pos < i_end_pos) && (**i_pos == '}')){
			++*i_pos;
		}
		
	}
	return routine_symbol->get_copy();
}

void J_Calc_Math_Input_Parser::check_end_state_and_advance(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos, bool i_end_pos_ok, const Delimiter_Handler<J_UI_Char>& irk_delims, const char* const ik_message){
	bool is_end_pos = *i_pos == i_end_pos;
	bool valid = true;

	valid = i_end_pos_ok || !is_end_pos;

	if(valid && !is_end_pos){
		valid = irk_delims.is_delim(**i_pos);
	}

	if(!valid){
		throw J_Syntax_Error(ik_message);
	}
	if(!is_end_pos){
		advance_white_space(&++*i_pos, i_end_pos);
	}
}

j_symbol* J_Calc_Math_Input_Parser::symbol_declaration_parser(J_UI_Const_Iter* i_pos
	, J_UI_String::const_iterator i_end_pos){
	j_symbol* new_symbol = nullptr;

	if(J_UI_Char('[') == **i_pos){
		//Declaring a routine
		new_symbol = get_routine(i_pos, i_end_pos);

	}else{
		J_UI_String cur_word = get_symbol_name(i_pos, i_end_pos);
		advance_white_space(i_pos, i_end_pos);
		auto decl_func_pos = M_declaration_functions.find(cur_word);

		new_symbol = M_declaration_functions.end() == decl_func_pos
			? nullptr : (this->*decl_func_pos->second)(i_pos, i_end_pos);
	}



	if(!new_symbol){
		throw J_Syntax_Error("Could not determine scope declaration");
	}

	return new_symbol;
}

J_Calc_Math_Input_Parser::Symbol_Cont_t J_Calc_Math_Input_Parser
	::get_declared_symbols(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos){
		J_UI_String debug_string(*i_pos, i_end_pos);
		cerr << "\nDeclaring Symbols: " << debug_string;
		Symbol_Cont_t symbols;
		while(*i_pos < i_end_pos){
			J_Symbol_Shared_t new_symbol(symbol_declaration_parser(i_pos, i_end_pos));
			symbols.push_back(new_symbol);
			if(*i_pos == i_end_pos){
				continue;
			}
			if(!srk_arg_delim.is_delim(**i_pos)){
 				throw J_Syntax_Error("Unexpected Character between declared symbols");
			}
			advance_white_space(&++*i_pos, i_end_pos);

		}
		return symbols;
}

j_symbol* J_Calc_Math_Input_Parser
	::dbl_declaration_parser(J_UI_Const_Iter* i_pos, J_UI_Const_Iter i_end_pos){

	cerr << "\nDbl declaration input: " << J_UI_String(*i_pos, i_end_pos);
	assert(!gk_symbol_delimiter.is_delim(**i_pos));

	switch((*i_pos)->charcode()){
	case '~':
		//Handle Units here. '~' denotes the units of the dbl being declared
		assert(0);
	default:
		;
	}
	J_UI_String symbol_name =  get_symbol_name(i_pos, i_end_pos);

	j_symbol* new_symbol = nullptr;
	try{

		advance_white_space(i_pos, i_end_pos);
		
		if(*i_pos == i_end_pos){
			new_symbol = new j_number_symbol(symbol_name);
		}
		
		//to bypass switch if we already made the symbol
		switch(new_symbol ? '\0' : (*i_pos)++->charcode()){
		case '=':
			new_symbol = create_multi_j_symbol(i_pos, i_end_pos, srk_arg_delim);
			break;
		case ',':
		case ';':
		case ']':
			--*i_pos;
		case ':':
			new_symbol = new j_number_symbol(symbol_name);
			break;
		case '\0':
			break;
		default:
			throw J_Syntax_Error("Unexpected Character Encountered");
		}

		new_symbol->set_name(symbol_name.std_str());

		return new_symbol;
	} catch(J_Error& e){
		delete new_symbol;
		throw e;
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
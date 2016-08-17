#include "J_Calc_Evaluater.h"

//
#include <string>
//
#include <j_symbol/J_Symbol_Fwd_Decl.h>
//
#include "J_Calc_Symbol.h"
//
#include "J_Calc_Parser/J_Calc_Parser.h"
//
#include "J_Symbol_Error.h"


using std::string;

namespace jomike{

std::string evaluate_math_input(const std::string& irk_input){
	J_Calc_Parser parser;
	string error_string = "UNexpected Error parsing input: " + irk_input;

	try{
		string return_val;
		if(j_calc_symbol* new_symbol_raw = parser.parse(irk_input)){
			j_calc_symbol_unique_t new_symbol(new_symbol_raw);
			
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
	} catch(J_Syntax_Error& er_error){
		er_error.print();
		return er_error.str();
	} catch(J_Value_Error& er_error){
		er_error.print();
		return er_error.str();
	} catch(J_Sym_Argument_Error& er_error){
		er_error.print();
		return er_error.str();
	} catch(J_Symbol_Error& er_error){
		er_error.print();
		return er_error.str();
	}


}


}




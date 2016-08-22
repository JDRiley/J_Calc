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
#include "JC_Symbol_Error.h"



namespace jomike{

jc_string_t evaluate_math_input(const jc_string_t& irk_input){
	J_Calc_Parser parser;
	jc_string_t error_string = L"Unexpected Error parsing input: " + irk_input;

	try{
		jc_string_t return_val;
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
	} catch(J_Syntax_Error<jc_string_t::value_type>& er_error){
		er_error.print();
		return er_error.message();
	} catch(J_Value_Error& er_error){
		er_error.print();
		return er_error.message();
	} catch(JC_Sym_Argument_Error& er_error){
		er_error.print();
		return er_error.message();
	} catch(JC_Symbol_Error& er_error){
		er_error.print();
		return er_error.message();
	}


}


}




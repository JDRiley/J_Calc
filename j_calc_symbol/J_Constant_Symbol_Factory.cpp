#include "J_Constant_Symbol_Factory.h"
//
#include <j_symbol/Constant_Symbol.h>
//
#include "j_constant_expression.h"


namespace jomike{

jc_constant_symbol* make_bool_constant_symbol(bool i_val, const yy::location& i_loc){
	return new Bool_Constant_Symbol<j_constant_expression<bool>>(i_val, i_loc);
}

jc_constant_symbol* make_int_constant_symbol(int i_val, const yy::location& i_loc){
	auto symbol =  new Int_Constant_Symbol<j_constant_expression<j_llint>>(i_val, i_loc);
	return symbol;
}

jc_constant_symbol* make_double_constant_symbol(j_dbl i_val, const yy::location& i_loc){
	return new Dbl_Constant_Symbol<j_constant_expression<j_dbl>>(i_val, i_loc);
}

jc_constant_symbol* make_string_constant_symbol(const jc_string_t& irk_string, const yy::location& i_loc){
	return new String_Constant_Symbol<j_constant_expression<jc_string_t>, jc_string_t>(irk_string, i_loc);
}



}




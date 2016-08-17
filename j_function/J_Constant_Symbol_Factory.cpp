#include "J_Constant_Symbol_Factory.h"
//
#include "J_Constant_Symbol.h"

namespace jomike{

J_Constant_Symbol* make_bool_constant_symbol(bool i_val, const yy::location& i_loc){
	return new Bool_Constant_Symbol(i_loc, i_val);
}

J_Constant_Symbol* make_int_constant_symbol(int i_val, const yy::location& i_loc){
	auto symbol =  new Int_Constant_Symbol(i_loc, i_val);
	return symbol;
}

J_Constant_Symbol* make_double_constant_symbol(j_dbl i_val, const yy::location& i_loc){
	return new Dbl_Constant_Symbol(i_loc, i_val);
}

J_Constant_Symbol* make_string_constant_symbol(const std::string& irk_string, const yy::location& i_loc){
	return new String_Constant_Symbol(i_loc, irk_string);
}



}




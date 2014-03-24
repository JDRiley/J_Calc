#include "Constant_Symbol_Factory.h"
//
#include "Constant_Symbol.h"

namespace jomike{

Constant_Symbol* make_bool_constant_symbol(bool i_val, const Lexer_Location& i_loc){
	return new Bool_Constant_Symbol(i_val, i_loc);
}

Constant_Symbol* make_int_constant_symbol(int i_val, const Lexer_Location& i_loc){
	auto symbol =  new Int_Constant_Symbol(i_val, i_loc);
	return symbol;
}

Constant_Symbol* make_double_constant_symbol(j_dbl i_val, const Lexer_Location& i_loc){
	return new Dbl_Constant_Symbol(i_val, i_loc);
}

Constant_Symbol* make_string_constant_symbol(const std::string& irk_string, const Lexer_Location& i_loc){
	return new String_Constant_Symbol(irk_string, i_loc);
}



}




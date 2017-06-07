#ifndef CONSTANT_SYMBOL_FACTORY_H
#define CONSTANT_SYMBOL_FACTORY_H

#include "J_Calc_Symbol_Fwd_Decl.h"
//
#include <string>


namespace jomike{
	
jc_constant_symbol* make_bool_constant_symbol(bool i_val, const yy::location& i_loc);

jc_constant_symbol* make_int_constant_symbol(int i_val, const yy::location& i_loc);

jc_constant_symbol* make_double_constant_symbol(j_dbl i_val, const yy::location& i_loc);

jc_constant_symbol* make_string_constant_symbol(
	const jc_string_t& irk_string, const yy::location& i_loc);
}

#endif //CONSTANT_SYMBOL_FACTORY_H



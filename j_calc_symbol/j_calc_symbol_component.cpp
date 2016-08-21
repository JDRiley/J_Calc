#include "j_calc_symbol_component.h"


namespace jomike{

j_calc_symbol_component::j_calc_symbol_component(
	const yy::location& irk_loc, Symbol_Types i_symbol_type):j_symbol(irk_loc){

	M_symbol_type = i_symbol_type;
}

void j_calc_symbol_component::set_type(Symbol_Types i_symbol_type){
	M_symbol_type = i_symbol_type;
}
Symbol_Types j_calc_symbol_component::symbol_type()const{
	return M_symbol_type;
}


}




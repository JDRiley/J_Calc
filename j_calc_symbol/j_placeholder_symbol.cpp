#include "j_placeholder_symbol.h"
//
#include <cassert>
//
#include <j_symbol/J_Symbol_Identifier.h>
//


namespace jomike{
bool j_placeholder_symbol::is_placeholder()const {
	return true;
}

j_placeholder_symbol::j_placeholder_symbol(const yy::location& irk_loc, j_size_t i_index)
:j_expression(irk_loc
	, new J_Symbol_Identifier<jc_string_t>(L"#" + std::to_wstring(i_index) + L"_Placeholder")
	, Symbol_Types::PLACEHOLDER)
	, M_placeholder_index(i_index){
	assert(i_index >= J_SIZE_T_ZERO);
}

void j_placeholder_symbol::clear(){
}

bool j_placeholder_symbol::has_value()const {
	return false;
}

j_value j_placeholder_symbol::derived_get_value(const Arguments&)const {
	throw J_Argument_Error(L"Getting value of place holder symbol");

}

j_size_t j_placeholder_symbol::placeholder_index()const{
	return M_placeholder_index;
}

j_placeholder_symbol* j_placeholder_symbol::get_copy()const {
	return new j_placeholder_symbol(*this);
}

j_placeholder_symbol* j_placeholder_symbol::move_copy(){
	return  new j_placeholder_symbol(std::move(*this));
}

jc_string_t j_placeholder_symbol::get_display_name(){
	return name();
}

void j_placeholder_symbol::process(){

}

void j_placeholder_symbol::alert_symbol_scope_set(){

}



}




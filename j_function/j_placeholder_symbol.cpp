#include "j_placeholder_symbol.h"
//
#include <cassert>
//
#include "J_Symbol_Identifier.h"
//
using std::string;

namespace jomike{
bool j_placeholder_symbol::is_placeholder()const {
	return true;
}

j_placeholder_symbol::j_placeholder_symbol(j_size_t i_index)
:j_expression(Symbol_Types::PLACEHOLDER,
	new J_Symbol_Identifier(string("#") + std::to_string(i_index) + "_Placeholder"))
	, M_Placeholder_index(i_index){
	assert(i_index >= J_SIZE_T_ZERO);
}

void j_placeholder_symbol::clear(){
}

bool j_placeholder_symbol::has_value()const {
	return false;
}

j_value j_placeholder_symbol::derived_get_value(const Arguments&)const {
	throw J_Argument_Error("Getting value of place holder symbol");

}

j_size_t j_placeholder_symbol::placeholder_index()const{
	return M_Placeholder_index;
}

j_placeholder_symbol* j_placeholder_symbol::get_copy()const {
	return new j_placeholder_symbol(*this);
}

j_placeholder_symbol* j_placeholder_symbol::move_copy(){
	return  new j_placeholder_symbol(std::move(*this));
}

jomike::J_UI_String j_placeholder_symbol::get_display_name(){
	return name();
}



}




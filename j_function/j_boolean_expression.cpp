#include "j_boolean_expression.h"
#include "Boolean_Testers.h"

using std::all_of;
using std::mem_fn;

namespace jomike{




void j_boolean_expression::clear(){
	assert(!"The method or operation is not implemented.");
	assert(!"The method or operation is not yet check.");
}

bool j_boolean_expression::has_value()const{
	return all_of(M_symbols.begin(), M_symbols.end()
		, mem_fn(&j_symbol::has_value));

}

j_value j_boolean_expression::derived_get_value(const Arguments& i_args)const{
	if(1 == M_symbols.size()){
		return M_symbols.front()->get_value(i_args).convert_units(J_Unit(J_Unit_Types::BOOLEAN));
	}

	assert((M_symbols.size() - 1) == M_boolean_testers.size());

	for(int i = 0, b = 0; b < M_boolean_testers.size(); i++, b++){
		if(!M_boolean_testers[b]->eval(M_symbols[i]->get_value(i_args)
			, M_symbols[i + 1]->get_value(i_args))){
			return J_VALUE_FALSE_BOOLEAN;
		}
	}

	return J_VALUE_TRUE_BOOLEAN;
}

j_boolean_expression* j_boolean_expression::move_copy(){
	return  new j_boolean_expression(std::move(*this));
}

j_boolean_expression* j_boolean_expression::get_copy()const {
	return new j_boolean_expression(*this);
}

j_boolean_expression::j_boolean_expression():j_symbol(Symbol_Types::BOOL){

}

J_UI_String j_boolean_expression::get_display_name(){
	if(has_value()){
		return get_value().to_str();
	}
	return name();
}

}




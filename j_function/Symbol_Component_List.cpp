#include "Symbol_Component_List.h"
//
#include "Arguments.h"
//
#include "J_Symbol_Scope.h"
//
#include "J_Symbol_Error.h"
namespace jomike{





void Symbol_Component_List::add_symbol_component(j_symbol_component* i_symbol){
	M_components.emplace_back(i_symbol);
	delete i_symbol;
}

void Symbol_Component_List::add_symbol_component(const j_symbol_component& i_component){
	M_components.emplace_back(i_component);
}

jomike::Symbol_Component_List::symbol_iterator Symbol_Component_List::end()const{
	return M_components.end();
}

jomike::Symbol_Component_List::symbol_iterator Symbol_Component_List::begin()const{
	return M_components.begin();
}

void Symbol_Component_List::clear(){
	throw std::logic_error("The method or operation is not implemented.");
}

bool Symbol_Component_List::has_value()const{
	throw std::logic_error("The method or operation is not implemented.");
}

jomike::j_value Symbol_Component_List::derived_get_value(const Arguments& i_args)const{
	assert(i_args.empty());

	//Symbol List getting the value is just getting the value of the last statement

	if(M_components.empty()){
		throw J_Symbol_Error("Trying to get value from empty Symbol List!");
	}

	J_Symbol_Scope new_scope(&symbol_scope());
	for(auto symbol : M_components){
		auto symbol_ptr = dynamic_cast<j_symbol*>(symbol);
		if(symbol_ptr){
			symbol_ptr->set_symbol_scope(&new_scope);
			symbol_ptr->get_value();
		}
		
	}
	auto last_symbol = dynamic_cast<j_symbol*>(M_components.back());

	if(!last_symbol){
		throw J_Value_Error("Trying to get value from non j_symbol type. ");
	}

	return last_symbol->get_value();
}

}




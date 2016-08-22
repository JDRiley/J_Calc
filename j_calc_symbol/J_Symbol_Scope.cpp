#include "J_Symbol_Scope.h"
//
#include "JC_Symbol_Error.h"
//
#include "j_calc_symbol.h"
//
#include <j_symbol/Specific_Symbol_List.h>
//
#include "j_declaration.h"

using std::for_each;
namespace jomike{



J_Symbol_Scope::J_Symbol_Scope(const J_Symbol_Scope* i_parent_symbol)
	:M_parent_scope(i_parent_symbol){

}

J_Symbol_Scope::J_Symbol_Scope(const Declaration_List& irk_list){
	for(auto declaration : irk_list){
		add_symbol(*declaration);
	}

}

J_Symbol_Scope::J_Symbol_Scope(const J_Symbol_Scope& irk_right): M_parent_scope(irk_right.M_parent_scope){
	for(auto f_symbol : irk_right.M_symbols){
		M_symbols.insert(f_symbol->name(), f_symbol->get_copy());
	}
}

jomike::j_calc_symbol* J_Symbol_Scope::get_symbol(const jc_string_t& irk_string)const{
	auto symbol_pos = M_symbols.find(irk_string);
	if(M_symbols.end() != symbol_pos){
		return *symbol_pos;
	} else if(M_parent_scope){
		return M_parent_scope->get_symbol(irk_string);
	}else{
		throw JC_Symbol_Error(L"Symbol With Name: " + irk_string + L" does not exists!");
	}
	
}

void J_Symbol_Scope::add_symbol(j_calc_symbol* i_symbol){
	assert(i_symbol);

	if(M_symbols.count(i_symbol->name())){
		throw JC_Symbol_Error(L"Symbol with name " + i_symbol->name() + L" already exists!");
	}

	M_symbols[i_symbol->name()] = i_symbol;
}

void J_Symbol_Scope::add_symbol(const j_calc_symbol& irk_symbol){
	add_symbol(irk_symbol.get_copy());
}

J_Symbol_Scope::~J_Symbol_Scope(){
	for(auto f_symbol : M_symbols){
		delete f_symbol;
	}
}

J_Symbol_Scope* J_Symbol_Scope::move_copy(){
	return  new J_Symbol_Scope(std::move(*this));
}

J_Symbol_Scope* J_Symbol_Scope::get_copy()const{
	return new J_Symbol_Scope(*this);
}

void J_Symbol_Scope::set_parent_scope(const J_Symbol_Scope* i_parent_scope){
	M_parent_scope = i_parent_scope;
}

}




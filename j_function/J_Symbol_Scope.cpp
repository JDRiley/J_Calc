#include "J_Symbol_Scope.h"
//
#include "J_Symbol_Error.h"
//
#include "j_symbol.h"
//
#include "Specific_Symbol_List.h"
//
#include "j_declaration.h"

using std::for_each;
namespace jomike{



J_Symbol_Scope::J_Symbol_Scope(const J_Symbol_Scope* i_parent_symbol)
	:M_parent_symbol(i_parent_symbol){

}

J_Symbol_Scope::J_Symbol_Scope(const Declaration_List& irk_list){
	for(auto declaration : irk_list){
		add_symbol(*declaration);
	}

}

J_Symbol_Scope::J_Symbol_Scope(const J_Symbol_Scope& irk_right): M_parent_symbol(irk_right.M_parent_symbol){
	for(auto f_symbol : irk_right.M_symbols){
		M_symbols.insert(f_symbol->name(), f_symbol->get_copy());
	}
}

j_symbol* J_Symbol_Scope::get_symbol(const J_UI_String& irk_string)const{
	auto symbol_pos = M_symbols.find(irk_string);
	if(M_symbols.end() != symbol_pos){
		return *symbol_pos;
	} else if(M_parent_symbol){
		return M_parent_symbol->get_symbol(irk_string);
	}else{
		throw J_Symbol_Error("Symbol With Name: " + irk_string.std_str() + " does not exists!");
	}
	
}

void J_Symbol_Scope::add_symbol(j_symbol* i_symbol){
	assert(i_symbol);

	if(M_symbols.count(i_symbol->name())){
		throw J_Symbol_Error("Symbol with name " + i_symbol->name().std_str() + " already exists!");
	}

	M_symbols[i_symbol->name()] = i_symbol;
}

void J_Symbol_Scope::add_symbol(const j_symbol& irk_symbol){
	add_symbol(irk_symbol.get_copy());
}

J_Symbol_Scope::~J_Symbol_Scope(){
	for(auto f_symbol : M_symbols){
		delete f_symbol;
	}
}

}




#include "Statement_Block.h"
//
#include "J_Symbol_Scope.h"
//
#include <j_symbol/Specific_Symbol_List.h>
//
#include "J_Symbol_Scope.h"
//
#include "j_declaration.h"
namespace jomike{



Statement_Block* Statement_Block::move_copy(){
	return  new Statement_Block(std::move(*this));
}

Statement_Block* Statement_Block::get_copy()const {
	return new Statement_Block(*this);
}

Statement_Block::Statement_Block(J_Calc_Symbol_List* i_symbol_list)
	:j_statement(i_symbol_list->location()){
	M_symbol_list = i_symbol_list;
}

Statement_Block::Statement_Block(const Statement_Block& irk_source):j_statement(irk_source){
	M_symbol_list = irk_source.M_symbol_list->get_copy();
}

Statement_Block::Statement_Block(Statement_Block&& irv_source):j_statement(std::move(irv_source)){
	M_symbol_list = irv_source.M_symbol_list->move_copy();
}

j_value Statement_Block::derived_get_value(const Arguments& i_args)const {
	J_Symbol_Scope running_scope(symbol_scope());
	

	
	j_value de_value;
	for(auto f_symbol : *M_symbol_list){
		f_symbol->set_symbol_scope(&running_scope);
		de_value = f_symbol->get_value(i_args);
	}

	return de_value;
}

bool Statement_Block::has_value()const{
	assert(M_symbol_list);
	if(M_symbol_list->empty()){
		return false;
	}

	return M_symbol_list->back().has_value();
}

void Statement_Block::clear(){
	M_symbol_list->clear();
}

Statement_Block::~Statement_Block(){
	delete M_symbol_list;
}

void Statement_Block::alert_symbol_scope_set(){
	for(auto f_symbol : *M_symbol_list){
		f_symbol->set_symbol_scope(&symbol_scope());
	}

	
}

void Statement_Block::process(){
	//As of now  statement block does no preperation
	//M_symbol_list->process();
}

}




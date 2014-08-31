#include "Statement_Block.h"
//
#include "J_Symbol_Scope.h"
//
#include "Specific_Symbol_List.h"
namespace jomike{



Statement_Block* Statement_Block::move_copy(){
	return  new Statement_Block(std::move(*this));
}

Statement_Block* Statement_Block::get_copy()const {
	return new Statement_Block(*this);
}

Statement_Block::Statement_Block(Symbol_List* i_symbol_list){
	M_symbol_list = i_symbol_list;
}

Statement_Block::Statement_Block(const Statement_Block& irk_source):j_statement(irk_source){
	M_symbol_list = irk_source.M_symbol_list->get_copy();
}

Statement_Block::Statement_Block(Statement_Block&& irv_source):j_statement(std::move(irv_source)){
	M_symbol_list = irv_source.M_symbol_list->move_copy();
}

j_value Statement_Block::derived_get_value(const Arguments& i_args)const {
	J_Symbol_Scope running_scope;
	running_scope.set_parent_scope(&symbol_scope());

	M_symbol_list->set_symbol_scope(&running_scope);

	return M_symbol_list->get_value(i_args);
}

bool Statement_Block::has_value()const{
	return M_symbol_list->has_value();
}

void Statement_Block::clear(){
	M_symbol_list->clear();
}

Statement_Block::~Statement_Block(){
	delete M_symbol_list;
}

void Statement_Block::set_symbol_scope(const J_Symbol_Scope* i_symbol_scope){
	j_statement::set_symbol_scope(i_symbol_scope);
	M_symbol_list->set_symbol_scope(i_symbol_scope);
}

}




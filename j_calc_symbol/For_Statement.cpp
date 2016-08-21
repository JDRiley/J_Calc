#include "For_Statement.h"
//
#include "j_expression.h"
//
#include "Statement_Block.h"
//
#include "J_Symbol_Scope.h"
//
#include <iostream>
//
#include <J_Utile.h>
using std::cerr;
namespace jomike{



For_Statement::For_Statement(const yy::location& irk_loc
	, j_statement* i_init_statement, j_expression* i_test_expression
	, j_expression* i_post_expression, Statement_Block* i_statement_block)
	:j_statement(irk_loc){
	M_init_statement = i_init_statement;
	M_test_expression = i_test_expression;
	M_post_expression = i_post_expression;
	M_statement_block = i_statement_block;

	assert(M_init_statement);
	assert(M_test_expression);
	assert(M_post_expression);
	assert(M_statement_block);

}

For_Statement::For_Statement(For_Statement&& irv_src):j_statement(std::move(irv_src)){
	M_init_statement = nullptr;
	M_test_expression = nullptr;
	M_post_expression = nullptr;
	M_statement_block = nullptr;

	swap(irv_src);
}

For_Statement::For_Statement(const For_Statement& irk_src):j_statement(irk_src){
	M_init_statement = irk_src.M_init_statement->get_copy();
	M_test_expression = irk_src.M_test_expression->get_copy();
	M_post_expression = irk_src.M_post_expression->get_copy();
	M_statement_block = irk_src.M_statement_block->get_copy();

}

For_Statement* For_Statement::move_copy(){
	return  new For_Statement(std::move(*this));
}

For_Statement* For_Statement::get_copy()const {
	return new For_Statement(*this);
}

void For_Statement::swap(For_Statement& ir_src){
	std::swap(M_init_statement, ir_src.M_init_statement);
	std::swap(M_test_expression, ir_src.M_test_expression);
	std::swap(M_post_expression, ir_src.M_post_expression);
	std::swap(M_statement_block, ir_src.M_statement_block);
}

For_Statement& For_Statement::operator=(For_Statement&& irv_right){
	swap(irv_right);
	return *this;
}

For_Statement& For_Statement::operator=(const For_Statement& irk_src){
	For_Statement temp(irk_src);
	swap(temp);
	return *this;
}

For_Statement::~For_Statement(){
	delete M_init_statement;
	delete M_test_expression;
	delete M_post_expression;
	delete M_statement_block;
}


void For_Statement::clear(){
	M_init_statement->clear();
	M_test_expression->clear();
	M_post_expression->clear();
	M_statement_block->clear();
}

bool For_Statement::has_value()const {
	return M_statement_block->has_value();
}

void For_Statement::alert_symbol_scope_set(){
	
}

void For_Statement::process(){



}

j_value For_Statement::derived_get_value(const Arguments& i_args)const {
	
	J_Symbol_Scope running_scope;
	running_scope.set_parent_scope(&symbol_scope());

	M_init_statement->set_symbol_scope(&running_scope);
	M_test_expression->set_symbol_scope(&running_scope);
	M_post_expression->set_symbol_scope(&running_scope);
	M_statement_block->set_symbol_scope(&running_scope);
	
	j_value value;

	

	
	M_init_statement->process();
	
	M_test_expression->process();
	
	J_Modder<100> modder;
	while(M_test_expression->get_value(i_args).as_bool()){
	
		
		M_statement_block->process();
		value = M_statement_block->get_value(i_args);
		
		if(modder.tick()){
			cerr << "\nI Value: " << running_scope.get_symbol(L"i")->get_value().as_llint() << L" "
				<< value.as_double();
		}
		M_post_expression->process();
		M_test_expression->process();


	}
	return value;
}

}




#include "Expression_Statement.h"
//
#include "j_calc_expression.h"

namespace jomike{



Expression_Statement::Expression_Statement(const Expression_Statement& irk_source)
	:j_statement(irk_source){
	M_expression = irk_source.M_expression->get_copy();
}

Expression_Statement::Expression_Statement(Expression_Statement&& irv_source)
: j_statement(std::move(irv_source)){
	M_expression = irv_source.M_expression->move_copy();
}

Expression_Statement::Expression_Statement(const yy::location& irk_loc, j_calc_expression* i_expression)
	:j_statement(irk_loc, i_expression->symbol_type()){
	M_expression = i_expression;
}

j_value Expression_Statement::derived_get_value(const Arguments& i_args)const {
	return M_expression->get_value(i_args);
}

Expression_Statement::~Expression_Statement(){
	delete M_expression;
}

Expression_Statement* Expression_Statement::move_copy(){
	return  new Expression_Statement(std::move(*this));
}

Expression_Statement* Expression_Statement::get_copy()const {
	return new Expression_Statement(*this);
}

bool Expression_Statement::has_value()const {
	return M_expression->has_value();
}

void Expression_Statement::clear(){
	M_expression->clear();
}

void Expression_Statement::alert_symbol_scope_set(){
	
	M_expression->set_symbol_scope(&symbol_scope());
}

void Expression_Statement::process(){
	M_expression->process();
}

}




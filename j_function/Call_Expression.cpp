#include "Call_Expression.h"
//
#include "J_Symbol_Identifier.h"
//
#include "Arguments.h"
//
#include "J_Symbol_Error.h"
//
#include <cassert>
namespace jomike{



bool Call_Expression::has_value()const{
	return true;
}

j_value Call_Expression::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
	(void)i_args;
	
	get_symbol()->set_symbol_scope(&symbol_scope());

	return get_symbol()->get_value(*M_args_list);
}

Call_Expression::Call_Expression(
	J_Symbol_Identifier* i_identifier, Arguments* i_args_list)
	:j_expression(Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED){
	M_identifier = i_identifier;
	M_args_list = i_args_list;

	
}

Call_Expression::Call_Expression(const Call_Expression& irk_source)
:j_expression(irk_source){
	if(irk_source.M_base_expression){
		M_base_expression = irk_source.M_base_expression->get_copy();
	}

	M_identifier = irk_source.M_identifier->get_copy();

	M_args_list = irk_source.M_args_list->get_copy();
}

Call_Expression::~Call_Expression(){
	delete M_identifier;
	delete M_args_list;
}

j_symbol* Call_Expression::get_symbol()const{
	return get_symbol_from_scope(M_identifier->identifier_name());
}

const Type_Syntax& Call_Expression::return_type_syntax()const {
	return get_symbol_from_scope(M_identifier->identifier_name())->return_type_syntax();
}

void Call_Expression::alert_symbol_scope_set(){
	
	M_args_list->set_symbol_scope(&symbol_scope());
}

void Call_Expression::process(){
	if(M_base_expression){
		M_base_expression->process();
	}



	for(auto f_arg : *M_args_list){
		f_arg->process();
	}

	try{
		auto symbol = get_symbol_from_scope(M_identifier->identifier_name());
		symbol->process();
		set_type_syntax(symbol->return_type_syntax());
	} catch(J_Symbol_Error&){

	}
}



}




#include "Call_Expression.h"

//
#include <cassert>
//
#include "J_Symbol_Identifier.h"
//
#include "Arguments.h"
namespace jomike{



bool Call_Expression::has_value()const{
	return get_j_symbol(M_identifier->identifier_name())->has_value();
}

j_value Call_Expression::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
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
	return get_j_symbol(M_identifier->identifier_name());
}

}




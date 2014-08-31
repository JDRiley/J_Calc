#include "Relational_Binary_Expression.h"
//
#include "j_value.h"

namespace jomike{



Relational_Binary_Expression::Relational_Binary_Expression(j_expression* i_left, j_expression* i_right, Operators i_operator):Binary_Expression(i_left, i_right, i_operator){

}

Relational_Binary_Expression::Relational_Binary_Expression(Relational_Binary_Expression&& irv_source) : Binary_Expression(std::move(irv_source)){

}

Relational_Binary_Expression::Relational_Binary_Expression(const Relational_Binary_Expression& irk_source) : Binary_Expression(irk_source){

}

Relational_Binary_Expression* Relational_Binary_Expression::move_copy(){
	return  new Relational_Binary_Expression(std::move(*this));
}

Relational_Binary_Expression* Relational_Binary_Expression::get_copy()const {
	return new Relational_Binary_Expression(*this);
}

j_value Relational_Binary_Expression::derived_get_value(const Arguments& i_args)const{
	j_value left_val = left_expression().get_value(i_args);
	j_value right_val = right_expression().get_value(i_args);
	switch(operator_enum()){
	case jomike::Operators::LESS:
		return left_val < right_val;
	case jomike::Operators::LESS_EQUAL:
		return left_val <= right_val;
	case jomike::Operators::GREATER:
		return left_val > right_val;
	case jomike::Operators::GREATER_EQUAL:
		return left_val >= right_val;
	case jomike::Operators::EQUAL:
		return left_val == right_val;
	case jomike::Operators::NOT_EQUAL:
		return left_val != right_val;
	case jomike::Operators::AND:
		return j_value(left_val.as_bool() && right_expression().get_value(i_args).as_bool(), J_Unit());
	case jomike::Operators::OR:
		return j_value(left_val.as_bool() || right_expression().get_value(i_args).as_bool(), J_Unit());
	default:
		assert(!"Bad Operator for Relational binary class");
		break;
	}

	assert(false);

	return j_value();
}

}




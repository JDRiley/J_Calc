#include "Unary_Negate_Expression.h"


namespace jomike{



Unary_Negate_Expression::Unary_Negate_Expression(j_expression* i_expression)
	: Unary_Operator_Expression(i_expression){

}

j_value Unary_Negate_Expression::derived_get_value(const Arguments& i_args)const {
	return -base_expression().get_value(i_args);
}

}




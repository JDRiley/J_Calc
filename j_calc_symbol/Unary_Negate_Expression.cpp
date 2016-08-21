#include "Unary_Negate_Expression.h"


namespace jomike{




j_value Unary_Negate_Expression::derived_get_value(const Arguments& i_args)const {
	return -base_expression().get_value(i_args);
}



}




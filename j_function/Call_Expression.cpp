#include "Call_Expression.h"

//
#include <cassert>
namespace jomike{



bool Call_Expression::has_value()const{
	assert(0);
	return false;
}

j_value Call_Expression::derived_get_value(const Arguments& /*i_args*/)const {
	assert(0);
	return j_value();
}

}




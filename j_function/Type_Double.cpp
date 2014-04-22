#include "Type_Double.h"
//
#include <j_type.h>

namespace jomike{


Type_Double::Type_Double():Type_Syntax("double", Symbol_Types::INT){

}

Type_Double* Type_Double::move_copy(){
	return  new Type_Double(std::move(*this));
}

Type_Double* Type_Double::get_copy()const {
	return new Type_Double(*this);
}

j_value Type_Double::convert_value(const j_value& irk_val)const{
	j_value::Value_Types type = irk_val.type();
	switch(type){
	case j_value::Value_Types::LL_INTEGER:
		return j_value(safe_cast<double>(irk_val.as_llint()), irk_val.units());
	case j_value::Value_Types::DOUBLE:
		return irk_val;
	case j_value::Value_Types::BOOL:
		return j_value(irk_val.as_bool() ? 1.0 : 0.0, irk_val.units());
	case j_value::Value_Types::STRING:
		assert(!"Cannot convert string to double");
	case j_value::Value_Types::UNDEFINIED:
		assert(!"Cannot convert undefined to double");
	default:
		assert(0);
		return j_value();
	}
}

}




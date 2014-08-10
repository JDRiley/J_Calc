#include "Type_Bool.h"
//
#include <j_type.h>

namespace jomike{



Type_Bool* Type_Bool::move_copy(){
	return  new Type_Bool(std::move(*this));
}

Type_Bool* Type_Bool::get_copy()const {
	return new Type_Bool(*this);
}

Type_Bool::Type_Bool():Type_Syntax("bool", Symbol_Types::BOOL){

}

j_value Type_Bool::convert_value(const j_value& irk_val)const{
	j_value::Value_Types type = irk_val.type();
	switch(type){
	case j_value::Value_Types::LL_INTEGER:
		return j_value(static_cast<bool>(irk_val.as_llint()), J_Unit());
	case j_value::Value_Types::DOUBLE:
		return j_value(static_cast<bool>(irk_val.as_double()), J_Unit());
	case j_value::Value_Types::BOOL:
		return irk_val;
	case j_value::Value_Types::STRING:
		throw J_Value_Error("Cannot implicitly convert string to double");
	case j_value::Value_Types::UNDEFINIED:
		throw J_Value_Error("Cannot implicitly convert undefined to double");
	default:
		assert(0);
		return j_value();
	}
}



}




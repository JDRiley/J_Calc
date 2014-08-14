#include "Type_Int.h"


namespace jomike{



Type_Int::Type_Int():Type_Syntax("int", Symbol_Types::INT){

}

Type_Int* Type_Int::move_copy(){
	return  new Type_Int(std::move(*this));
}

Type_Int* Type_Int::get_copy()const {
	return new Type_Int(*this);
}

j_value Type_Int::convert_value(const j_value& irk_val)const{
	j_value::Value_Types type = irk_val.type();
	switch(type){
	case j_value::Value_Types::LL_INTEGER:
		return irk_val;
	case j_value::Value_Types::DOUBLE:
		return j_value(static_cast<j_llint>(irk_val.as_double()), irk_val.units());;
	case j_value::Value_Types::BOOL:
		return j_value(irk_val.as_bool() ? 1ll : 0ll, irk_val.units());
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




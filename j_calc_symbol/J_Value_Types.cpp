#include "J_Value_Types.h"


namespace jomike{






bool int_upcastable(J_Value_Types i_type){
	switch(i_type){
	case j_value::Value_Types::DOUBLE:
		return true;
	default:
		return false;
	}
}

bool dbl_upcastable(J_Value_Types /*i_type*/){
	return false;
}

bool bool_upcastable(J_Value_Types /*i_type*/){
	return false;
}

bool str_upcastable(J_Value_Types /*i_type*/){
	return false;
}

}




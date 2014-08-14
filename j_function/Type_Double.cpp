#include "Type_Double.h"
//
#include <j_type.h>
//
#include "j_value.h"
namespace jomike{


Type_Double::Type_Double():Type_Syntax("double", Symbol_Types::DOUBLE){

}

Type_Double* Type_Double::move_copy(){
	return  new Type_Double(std::move(*this));
}

Type_Double* Type_Double::get_copy()const {
	return new Type_Double(*this);
}

j_value Type_Double::convert_value(const j_value& irk_val)const{
	j_value new_value(irk_val);
	new_value.convert_to_type(j_value::Value_Types::DOUBLE);
	return new_value;
}

}




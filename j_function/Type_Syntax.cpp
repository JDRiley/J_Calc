#include "Type_Syntax.h"
//
#include <J_UI/J_UI_String.h>

namespace jomike{



Type_Syntax::Type_Syntax(
	const std::string& irk_name, Symbol_Types i_symbol_type)
	:j_symbol_component(i_symbol_type), M_type_name(irk_name){

}


j_value Type_Syntax::convert_value(const j_value& /*irk_val*/)const{
	assert(!"Did Not implement conversion for this type");
	return j_value();
}

const std::string& Type_Syntax::type_name()const{
	return M_type_name;
}

bool Type_Syntax::is_same_type(const Type_Syntax& irk_right)const{
	return type_name() == irk_right.type_name();
}

bool Type_Syntax::operator==(const Type_Syntax& irk_right)const{
	return is_same_type(irk_right);
}

bool Type_Syntax::operator!=(const Type_Syntax& irk_right)const{
	return !(*this == irk_right);
}

}




#include "Type_Syntax.h"
//
#include <J_UI/J_UI_String.h>

namespace jomike{



Type_Syntax::Type_Syntax(
	const std::string& irk_name, Symbol_Types i_symbol_type)
	:j_symbol_component(i_symbol_type), M_type_name(irk_name){

}


jomike::j_value Type_Syntax::convert_value(const j_value& /*irk_val*/)const{
	assert(!"Did Not implement conversion for this type");
	return j_value();
}

}




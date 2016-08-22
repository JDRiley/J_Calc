#include "Type_Factory.h"
//
#include "Type_Int.h"
//
#include "Type_Double.h"
//
#include "Type_Bool.h"
//
#include "Type_String.h"
//
#include "Type_Void.h"
//
#include "JC_Symbol_Error.h"
//
#include <string>

using std::to_string;
namespace jomike{



Type_Syntax* make_int_type_syntax(const yy::location& irk_loc){
	return new Type_Int(irk_loc);
}

Type_Syntax* make_double_type_syntax(const yy::location& irk_loc){
	return new Type_Double(irk_loc);
}

Type_Syntax* make_bool_type_syntax(const yy::location& irk_loc){
	return new Type_Bool(irk_loc);
}

Type_Syntax* make_type_syntax(const yy::location& irk_loc, Symbol_Types i_symbol_type){
	switch(i_symbol_type){
	case Symbol_Types::INT:
		return new Type_Int(irk_loc);
	case Symbol_Types::BOOL:
		return new Type_Bool(irk_loc);
	case Symbol_Types::DOUBLE:
		return new Type_Double(irk_loc);
	case Symbol_Types::STRING:
		return new Type_String(irk_loc);
	default:
		//throw J_Symbol_Error("Cannot make Type_Syntax With this Symbol Type." + to_string(__LINE__ ) +  __FILE__);
		return nullptr;
		break;
	}
}

Type_Syntax* make_void_type_syntax(const yy::location& irk_loc){
	return new Type_Void(irk_loc);
}

Type_Syntax* make_string_type_syntax(const yy::location& irk_loc){
	return new Type_String(irk_loc);
}

}




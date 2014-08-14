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
#include "J_Symbol_Error.h"
//
#include <string>

using std::to_string;
namespace jomike{



Type_Syntax* make_int_type_syntax(){
	return new Type_Int;
}

Type_Syntax* make_double_type_syntax(){
	return new Type_Double;
}

Type_Syntax* make_bool_type_syntax(){
	return new Type_Bool;
}

Type_Syntax* make_type_syntax(Symbol_Types i_symbol_type){
	switch(i_symbol_type){
	case Symbol_Types::INT:
		return new Type_Int;
	case Symbol_Types::BOOL:
		return new Type_Bool;
	case Symbol_Types::DOUBLE:
		return new Type_Double;
	case Symbol_Types::STRING:
		return new Type_String;
	default:
		throw J_Symbol_Error("Cannot make Type_Syntax With this Symbol Type." + to_string(__LINE__ ) +  __FILE__);
		break;
	}
}

}




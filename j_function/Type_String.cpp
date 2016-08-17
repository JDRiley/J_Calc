#include "Type_String.h"


namespace jomike{



Type_String::Type_String(const yy::location& irk_loc)
	:Type_Syntax(irk_loc, "string", Symbol_Types::STRING){

}

Type_String* Type_String::move_copy(){
	return  new Type_String(std::move(*this));
}

Type_String* Type_String::get_copy()const {
	return new Type_String(*this);
}

}




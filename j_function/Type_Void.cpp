#include "Type_Void.h"


namespace jomike{


Type_Void::Type_Void():Type_Syntax("void", Symbol_Types::VOID_TYPE){

}

Type_Void* Type_Void::move_copy(){
	return  new Type_Void(std::move(*this));
}

Type_Void* Type_Void::get_copy()const {
	return new Type_Void(*this);
}

bool Type_Void::is_void()const {
	return true;
}

	
}




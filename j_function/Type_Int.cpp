#include "Type_Int.h"


namespace jomike{



Type_Int::Type_Int():Type_Syntax("Int", Symbol_Types::INT){

}

Type_Int* Type_Int::move_copy(){
	return  new Type_Int(std::move(*this));
}

Type_Int* Type_Int::get_copy()const {
	return new Type_Int(*this);
}

}




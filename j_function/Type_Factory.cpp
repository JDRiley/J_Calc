#include "Type_Factory.h"
//
#include "Type_Int.h"
//
#include "Type_Double.h"

namespace jomike{



Type_Syntax* make_int_type_syntax(){
	return new Type_Int;
}

Type_Syntax* make_double_type_syntax(){
	return new Type_Double;
}

}




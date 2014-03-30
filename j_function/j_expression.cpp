#include "j_expression.h"


namespace jomike{
j_expression::j_expression(Symbol_Types i_type):j_symbol(i_type){
	
}

j_expression* j_expression::as_expression(){
	return get_copy();
}


}




#include "j_expression.h"


namespace jomike{



j_expression* j_expression::as_expression(){
	return get_copy();
}

jc_string_t j_expression::get_display_name(){
	if(has_value()){
		return get_value().to_str();
	}
	return name();
}

j_expression::~j_expression(){

}



}




#include "j_calc_expression.h"


namespace jomike{



j_calc_expression* j_calc_expression::as_expression(){
	return get_copy();
}

jc_string_t j_calc_expression::get_display_name(){
	if(has_value()){
		return get_value().to_str();
	}
	return name();
}

j_calc_expression::~j_calc_expression(){

}



}




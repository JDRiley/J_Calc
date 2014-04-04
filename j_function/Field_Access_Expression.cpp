#include "Field_Access_Expression.h"
//
#include "J_Symbol_Identifier.h"

namespace jomike{



Field_Access_Expression::Field_Access_Expression(J_Symbol_Identifier* i_name):j_expression(Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED), M_identifier(i_name){

}

j_value Field_Access_Expression::derived_get_value(const Arguments& i_args)const {
	return get_j_symbol(M_identifier->identifier_name())
		->get_value(i_args);
}

bool Field_Access_Expression::has_value()const{
	try{
		return get_j_symbol(M_identifier->identifier_name())->has_value();
	} catch(...){
		return false;
	}
}

Field_Access_Expression* Field_Access_Expression::move_copy(){
	return  new Field_Access_Expression(std::move(*this));
}

Field_Access_Expression* Field_Access_Expression::get_copy()const {
	return new Field_Access_Expression(*this);
}

}




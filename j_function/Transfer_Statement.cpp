#include "Transfer_Statement.h"
//
#include "Custom_Routine_Symbol.h"

namespace jomike{



Transfer_Statement::Transfer_Statement(j_expression* i_expression)
	:Expression_Statement(i_expression){

}

Transfer_Statement* Transfer_Statement::move_copy(){
	return  new Transfer_Statement(std::move(*this));
}

Transfer_Statement* Transfer_Statement::get_copy()const {
	return new Transfer_Statement(*this);
}

j_value Transfer_Statement::derived_get_value(const Arguments& i_args)const {
	throw J_Routine_Transfer_Exception(Expression_Statement::derived_get_value(i_args));
}

}




#include "Type_Routine.h"
//
#include "Specific_Symbol_List.h"

namespace jomike{



Type_Routine::~Type_Routine(){
	delete M_return_type;
	delete M_argument_types;
}

Type_Routine* Type_Routine::move_copy(){
	return  new Type_Routine(std::move(*this));
}

Type_Routine* Type_Routine::get_copy()const {
	return new Type_Routine(*this);
}

Type_Routine::Type_Routine(Type_Syntax* i_return_type, Type_Syntax_List* i_arg_types)
:Type_Syntax("routine", Symbol_Types::ROUTINE){
	M_return_type = i_return_type;
	M_argument_types = i_arg_types;

	assert(M_return_type);
	assert(M_argument_types);
}

Type_Routine::Type_Routine(const Type_Routine& irk_right):Type_Syntax(irk_right){
	M_return_type = irk_right.M_return_type->get_copy();
	M_argument_types = irk_right.M_argument_types->get_copy();
}

Type_Routine::Type_Routine(Type_Routine&& irv_right) : Type_Syntax(std::move(irv_right)){
	M_return_type = irv_right.M_return_type->move_copy();
	M_argument_types = irv_right.M_argument_types->move_copy();
}

const Type_Syntax_List& Type_Routine::argument_types_list()const{
	return *M_argument_types;
}

const Type_Syntax& Type_Routine::return_type()const{
	assert(M_return_type);
	return *M_return_type;
}

}




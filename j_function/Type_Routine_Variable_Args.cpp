#include "Type_Routine_Variable_Args.h"


namespace jomike{



Type_Routine_Variable_Args* Type_Routine_Variable_Args::move_copy(){
	return  new Type_Routine_Variable_Args(std::move(*this));
}

Type_Routine_Variable_Args* Type_Routine_Variable_Args::get_copy()const {
	return new Type_Routine_Variable_Args(*this);
}

Type_Routine_Variable_Args::~Type_Routine_Variable_Args(){
	delete M_variadic_arg_type;
}

Type_Routine_Variable_Args::Type_Routine_Variable_Args(Type_Routine_Variable_Args&& irv_right):Type_Routine(std::move(irv_right)){
	M_variadic_arg_type = irv_right.M_variadic_arg_type->move_copy();
}

Type_Routine_Variable_Args::Type_Routine_Variable_Args(const Type_Routine_Variable_Args& irk_right) : Type_Routine(irk_right){
	M_variadic_arg_type = irk_right.M_variadic_arg_type->get_copy();
}

Type_Routine_Variable_Args::Type_Routine_Variable_Args(Type_Syntax* i_return_type, Type_Syntax_List* i_arg_types, Type_Syntax* i_variadic_type) : Type_Routine(i_return_type, i_arg_types){
	M_variadic_arg_type = i_variadic_type;
}

}




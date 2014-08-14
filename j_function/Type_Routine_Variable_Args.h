#ifndef TYPE_ROUTINE_VARIABLE_ARGS_H
#define TYPE_ROUTINE_VARIABLE_ARGS_H

#include "Type_Routine.h"


namespace jomike{

class Type_Routine_Variable_Args : public Type_Routine{
public:

	Type_Routine_Variable_Args(
		Type_Syntax* i_return_type, Type_Syntax_List* i_arg_types
		, Type_Syntax* i_variadic_type);

	Type_Routine_Variable_Args(const Type_Routine_Variable_Args& irk_right);

	Type_Routine_Variable_Args(Type_Routine_Variable_Args&& irv_right);

	~Type_Routine_Variable_Args();

	Type_Routine_Variable_Args* get_copy()const override;

	Type_Routine_Variable_Args* move_copy()override;
private:
	Type_Syntax* M_variadic_arg_type;
};

}

#endif //TYPE_ROUTINE_VARIABLE_ARGS_H



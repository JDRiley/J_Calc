#ifndef TYPE_ROUTINE_H
#define TYPE_ROUTINE_H

#include "Type_Syntax.h"


namespace jomike{
class Type_Routine : public Type_Syntax{
public:
	Type_Routine(Type_Syntax* i_return_type, Type_Syntax_List* i_arg_types);

	Type_Routine(const Type_Routine&); 
	Type_Routine(Type_Routine&& irv_right);
	
	Type_Routine& operator=(const Type_Routine&) = delete;
	Type_Routine& operator=(Type_Routine&&) = delete;
	
	Type_Routine* get_copy()const override;

	Type_Routine* move_copy()override;

	const Type_Syntax_List& argument_types_list()const;

	~Type_Routine();

private:
	Type_Syntax* M_return_type;
	Type_Syntax_List* M_argument_types;
};

}

#endif //TYPE_ROUTINE_H



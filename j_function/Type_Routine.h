#ifndef TYPE_ROUTINE_H
#define TYPE_ROUTINE_H

#include "Type_Syntax.h"
//
#include "Specific_Symbol_List.h"

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

	const Type_Syntax& return_type()const;

	J_UI_String arg_type_string()const{
		J_UI_String arg_string = "[";

		if(M_argument_types->empty()){
			arg_string.append("]");
			return arg_string;
		}

		auto cur_pos = M_argument_types->begin();
		auto end_pos = M_argument_types->end();
		arg_string.append((*cur_pos)->type_name());

		while(++cur_pos != end_pos){
			arg_string.append(", ");
			arg_string.append((*cur_pos)->type_name());
		}

		arg_string.append("]");

		return arg_string;
	}

	~Type_Routine();

private:
	Type_Syntax* M_return_type;
	Type_Syntax_List* M_argument_types;
};

}

#endif //TYPE_ROUTINE_H



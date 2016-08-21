#ifndef J_ROUTINE_SYMBOL_H
#define J_ROUTINE_SYMBOL_H

#include "j_declaration.h"


namespace jomike{

class j_routine_symbol : public j_declaration{
public:
	j_routine_symbol(
		const yy::location& irk_loc, Type_Routine* i_routine_type, J_Symbol_Identifier* i_identifier);
	
	j_routine_symbol* get_copy()const override = 0;
	j_routine_symbol* move_copy()override = 0;

	bool has_value()const override;

	void clear()override;

	const Type_Syntax& return_type_syntax()const override;

	const Type_Routine& routine_type_syntax()const;


	jc_string_t get_display_name()override;
protected:
	const Type_Syntax_List& argument_types_list()const;
	
private:

};




}

#endif //J_ROUTINE_SYMBOL_H


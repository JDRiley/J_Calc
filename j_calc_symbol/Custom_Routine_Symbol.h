#ifndef CUSTOM_ROUTINE_SYMBOL_H
#define CUSTOM_ROUTINE_SYMBOL_H

#include "j_routine_symbol.h"
//
#include <ex_array.h>

namespace jomike{
class Custom_Routine_Symbol : public j_routine_symbol{
public:
	Custom_Routine_Symbol(const yy::location& irk_loc
		, J_Symbol_Identifier* i_identifier, const Declaration_List& irk_static_declarations
		, const Declaration_List& irk_arg_declarations, Type_Syntax* i_return_type
		, Statement_Block* i_statement_block);

	Custom_Routine_Symbol(const Custom_Routine_Symbol& irk_right);
	Custom_Routine_Symbol(Custom_Routine_Symbol&& irv_right);


	Custom_Routine_Symbol& operator=(const Custom_Routine_Symbol&) = delete;
	Custom_Routine_Symbol& operator=(Custom_Routine_Symbol&&) = delete;

	Custom_Routine_Symbol* get_copy()const override;

	Custom_Routine_Symbol* move_copy()override;

	void process()override;

	~Custom_Routine_Symbol();

private:

	j_value derived_get_value(const Arguments& i_args)const override;

	ex_array<jc_string_t> M_arg_names;

	Statement_Block* M_statement_block;
	J_Symbol_Scope* M_running_scope;

	void alert_symbol_scope_set()override;
};

//Class Thrown when transfer encountered
class J_Routine_Transfer_Exception{
public:
	J_Routine_Transfer_Exception(j_value i_value);

	const j_value& value()const;
private:
	j_value M_value;
};

}

#endif //CUSTOM_ROUTINE_SYMBOL_H



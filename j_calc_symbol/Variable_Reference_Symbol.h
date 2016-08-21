#ifndef VARIABLE_REFERENCE_SYMBOL_H
#define VARIABLE_REFERENCE_SYMBOL_H

#include "j_declaration.h"


namespace jomike{
class Variable_Reference_Symbol : public j_declaration{
public:


	Variable_Reference_Symbol(const yy::location& irk_loc
		, Type_Syntax* i_syntax, J_Symbol_Identifier* i_identifier, j_expression* i_expression);


	//Deleting Constructors and Assignment Operators
	Variable_Reference_Symbol(const Variable_Reference_Symbol& irk_src);
	Variable_Reference_Symbol(Variable_Reference_Symbol&& irv_src);
	Variable_Reference_Symbol& operator=(const Variable_Reference_Symbol&) = delete;
	Variable_Reference_Symbol& operator=(Variable_Reference_Symbol&&) = delete;

	Variable_Reference_Symbol* move_copy()override;

	Variable_Reference_Symbol* get_copy()const override;

	jc_string_t get_display_name()override;

	bool has_value()const override;

	void clear();


	void process()override;
private:
	j_expression* M_expression = nullptr;
	j_value derived_get_value(const Arguments& i_args)const override;

	void alert_symbol_scope_set()override;
};

}

#endif //VARIABLE_REFERENCE_SYMBOL_H



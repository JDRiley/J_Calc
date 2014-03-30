#ifndef VARIABLE_REFERENCE_SYMBOL_H
#define VARIABLE_REFERENCE_SYMBOL_H

#include "j_declaration.h"


namespace jomike{
class Variable_Reference_Symbol : public j_declaration{
public:
	Variable_Reference_Symbol(
		Type_Syntax* i_syntax, J_UI_String* i_identifier);

	Variable_Reference_Symbol(
		Type_Syntax* i_syntax, J_UI_String* i_identifier, j_expression* i_expression);

	Variable_Reference_Symbol* move_copy()override;

	Variable_Reference_Symbol* get_copy()const override;

	J_UI_String get_display_name()override;

	bool has_value()const override;

	void clear();

private:
	j_expression* M_expression = nullptr;
	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //VARIABLE_REFERENCE_SYMBOL_H



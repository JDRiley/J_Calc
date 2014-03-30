#ifndef VARIABLE_SYMBOL_H
#define VARIABLE_SYMBOL_H

#include "j_declaration.h"


namespace jomike{
class Variable_Symbol : public j_declaration{
public:
	Variable_Symbol(
		Type_Syntax* i_syntax, J_UI_String* i_identifier);

	Variable_Symbol(
		Type_Syntax* i_syntax, J_UI_String* i_identifier, const j_expression& i_expression);

	Variable_Symbol* get_copy()const override{
		return new Variable_Symbol(*this);
	}

	Variable_Symbol* move_copy()override{
		return  new Variable_Symbol(std::move(*this));
	}

	void clear()override;

	bool has_value()const override;

	void set_value(j_value i_value)override;

	J_UI_String get_display_name()override;

	j_expression* as_expression()override;

private:
	j_value derived_get_value(const Arguments& i_args)const override;
	j_value M_value;
	
};

}

#endif //VARIABLE_SYMBOL_H



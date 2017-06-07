#ifndef FIELD_ACCESS_EXPRESSION_H
#define FIELD_ACCESS_EXPRESSION_H

#include "j_calc_expression.h"


namespace jomike{
class Field_Access_Expression : public j_calc_expression{
public:
	Field_Access_Expression(const yy::location& irk_loc, J_Symbol_Identifier<jc_string_t>* i_name);


	Field_Access_Expression(const Field_Access_Expression& irk_source);

	Field_Access_Expression* get_copy()const override;

	Field_Access_Expression* move_copy()override;

	void set_value(j_value i_value)override;

	jc_string_t get_display_name()override;

	bool has_value()const;

	j_calc_symbol* make_non_referenced()const override;

	~Field_Access_Expression();

	void process()override;
private:
	J_Symbol_Identifier<jc_string_t>* M_identifier;

	j_value derived_get_value(const Arguments& i_args)const override;


	void alert_symbol_scope_set()override;
};

}

#endif //FIELD_ACCESS_EXPRESSION_H



#ifndef FIELD_ACCESS_EXPRESSION_H
#define FIELD_ACCESS_EXPRESSION_H

#include "j_expression.h"


namespace jomike{
class Field_Access_Expression : public j_expression{
public:
	Field_Access_Expression(J_Symbol_Identifier* i_name);


	Field_Access_Expression(const Field_Access_Expression& irk_source);

	Field_Access_Expression* get_copy()const override;

	Field_Access_Expression* move_copy()override;

	void set_value(j_value i_value)override;

	J_UI_String get_display_name()override;

	bool has_value()const;

	j_symbol* make_non_referenced()const override;

	~Field_Access_Expression();
private:
	J_Symbol_Identifier* M_identifier;

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //FIELD_ACCESS_EXPRESSION_H



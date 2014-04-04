#ifndef FIELD_ACCESS_EXPRESSION_H
#define FIELD_ACCESS_EXPRESSION_H

#include "j_expression.h"


namespace jomike{
class Field_Access_Expression : public j_expression{
public:
	Field_Access_Expression(J_Symbol_Identifier* i_name);

	Field_Access_Expression* get_copy()const override;

	Field_Access_Expression* move_copy()override;


	bool has_value()const;
private:
	J_Symbol_Identifier* M_identifier;

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //FIELD_ACCESS_EXPRESSION_H



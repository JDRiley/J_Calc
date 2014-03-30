#ifndef SUBTRACTION_EXPRESSION_H
#define SUBTRACTION_EXPRESSION_H

#include "j_expression.h"


namespace jomike{
class Subtraction_Expression : public j_expression{
public:
	Subtraction_Expression(j_expression* i_left_expression, j_expression* i_right_expression);


	Subtraction_Expression* get_copy()const override;

	Subtraction_Expression* move_copy()override;

	bool has_value()const override;
private:
	j_expression* M_left_expression;
	j_expression* M_right_expression;

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //SUBTRACTION_EXPRESSION_H



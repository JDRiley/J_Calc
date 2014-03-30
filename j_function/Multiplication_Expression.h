#ifndef MULTIPLICATION_EXPRESSION_H
#define MULTIPLICATION_EXPRESSION_H

#include "j_expression.h"


namespace jomike{
class Multiplication_Expression : public j_expression{
public:
	Multiplication_Expression(j_expression* i_left_expression, j_expression* i_right_expression);


	Multiplication_Expression* get_copy()const override;

	Multiplication_Expression* move_copy()override;

	bool has_value()const override;
private:
	j_expression* M_left_expression;
	j_expression* M_right_expression;

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //MULTIPLICATION_EXPRESSION_H



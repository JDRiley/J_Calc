#ifndef SUBTRACTION_EXPRESSION_H
#define SUBTRACTION_EXPRESSION_H

#include "Binary_Expression.h"


namespace jomike{
class Subtraction_Expression : public Binary_Expression{
public:
	Subtraction_Expression(j_expression* i_left_expression, j_expression* i_right_expression);

	Subtraction_Expression(const Subtraction_Expression& irk_source) = default;

	Subtraction_Expression* get_copy()const override;

	Subtraction_Expression* move_copy()override;

	bool has_value()const override;
	~Subtraction_Expression();
private:

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //SUBTRACTION_EXPRESSION_H



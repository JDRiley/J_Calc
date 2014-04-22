#ifndef MULTIPLICATION_EXPRESSION_H
#define MULTIPLICATION_EXPRESSION_H

#include "Binary_Expression.h"


namespace jomike{
class Multiplication_Expression : public Binary_Expression{
public:
	Multiplication_Expression(j_expression* i_left_expression, j_expression* i_right_expression);

	Multiplication_Expression(const Multiplication_Expression& irk_source) = default;
	Multiplication_Expression* get_copy()const override;

	Multiplication_Expression* move_copy()override;

	bool has_value()const override;

	~Multiplication_Expression();
private:


	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //MULTIPLICATION_EXPRESSION_H



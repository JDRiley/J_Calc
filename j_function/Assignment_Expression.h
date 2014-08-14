#ifndef ASSIGNMENT_EXPRESSION_H
#define ASSIGNMENT_EXPRESSION_H

#include "Binary_Expression.h"


namespace jomike{
class Assignment_Expression : public Binary_Expression{
public:
	Assignment_Expression(j_expression* i_left_expression, j_expression* i_right_expression);

	Assignment_Expression(const Assignment_Expression&) = default;

	Assignment_Expression* get_copy()const override;

	Assignment_Expression* move_copy()override;

	bool has_value()const override;
private:


	j_value derived_get_value(const Arguments& i_args)const override;
};


}

#endif //ASSIGNMENT_EXPRESSION_H



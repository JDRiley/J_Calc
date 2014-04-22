#ifndef DIVISION_EXPRESSION_H
#define DIVISION_EXPRESSION_H

#include "Binary_Expression.h"


namespace jomike{
class Division_Expression : public Binary_Expression{
public:
	Division_Expression(j_expression* i_left_expression, j_expression* i_right_expression);


	Division_Expression* get_copy()const override;

	Division_Expression* move_copy()override;

	bool has_value()const override;
private:

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //DIVISION_EXPRESSION_H



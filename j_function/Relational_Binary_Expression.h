#ifndef RELATIONAL_BINARY_EXPRESSION_H
#define RELATIONAL_BINARY_EXPRESSION_H

#include "Binary_Expression.h"


namespace jomike{
class Relational_Binary_Expression : public Binary_Expression{
public:
	Relational_Binary_Expression(
		j_expression* i_left, j_expression* i_right, Operators i_operator);

	
	Relational_Binary_Expression(const Relational_Binary_Expression& irk_source);
	Relational_Binary_Expression(Relational_Binary_Expression&& irv_source);
	Relational_Binary_Expression& operator=(const Relational_Binary_Expression&) = delete;
	Relational_Binary_Expression& operator=(Relational_Binary_Expression&&) = delete;

	Relational_Binary_Expression* get_copy()const override;

	Relational_Binary_Expression* move_copy()override;


private:

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //RELATIONAL_BINARY_EXPRESSION_H



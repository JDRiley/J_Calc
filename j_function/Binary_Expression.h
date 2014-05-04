#ifndef BINARY_EXPRESSION_H
#define BINARY_EXPRESSION_H

#include "j_expression.h"


namespace jomike{

class Binary_Expression : public j_expression{
public:
	Binary_Expression(j_expression* i_left_expression, j_expression* i_right_expression);

	Binary_Expression(const Binary_Expression& irk_source);

	Binary_Expression(Binary_Expression&& irr_right);

	Binary_Expression* get_copy()const override = 0;

	Binary_Expression* move_copy()override = 0;

	bool has_value()const override;

	~Binary_Expression();
protected:
	j_expression& left_expression();

	const j_expression& left_expression()const;

	j_expression& right_expression();

	const j_expression& right_expression()const;

private:
	j_expression* M_left_expression;
	j_expression* M_right_expression;
};
}

#endif //BINARY_EXPRESSION_H


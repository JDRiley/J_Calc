#ifndef UNARY_OPERATOR_EXPRESSION_H
#define UNARY_OPERATOR_EXPRESSION_H

#include "j_expression.h"


namespace jomike{
class Unary_Operator_Expression : public j_expression{
public:
	Unary_Operator_Expression(j_expression* i_expression);

	Unary_Operator_Expression(const Unary_Operator_Expression& irk_src);

	Unary_Operator_Expression(Unary_Operator_Expression&& irv_right);

	Unary_Operator_Expression& operator=(const Unary_Operator_Expression&) = delete;

	Unary_Operator_Expression* get_copy()const override= 0;
	
	Unary_Operator_Expression* move_copy()override = 0;
	bool has_value()const override;

	~Unary_Operator_Expression();
protected:
	j_expression& base_expression();

	const j_expression& base_expression()const;
private:

	j_expression* M_expression;

};

}

#endif //UNARY_OPERATOR_EXPRESSION_H



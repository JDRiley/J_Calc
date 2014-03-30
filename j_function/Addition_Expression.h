#ifndef ADDITION_EXPRESSION_H
#define ADDITION_EXPRESSION_H

#include "j_expression.h"


namespace jomike{
class Addition_Expression : public j_expression{
public:
	Addition_Expression(j_expression* i_left_expression, j_expression* i_right_expression);


	Addition_Expression* get_copy()const override{
		return new Addition_Expression(*this);
	}

	Addition_Expression* move_copy()override{
		return  new Addition_Expression(std::move(*this));
	}

	bool has_value()const override;
private:
	j_expression* M_left_expression;
	j_expression* M_right_expression;

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //ADDITION_EXPRESSION_H



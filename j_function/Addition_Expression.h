#ifndef ADDITION_EXPRESSION_H
#define ADDITION_EXPRESSION_H

#include "Binary_Expression.h"


namespace jomike{
class Addition_Expression : public Binary_Expression{
public:
	Addition_Expression(j_expression* i_left_expression, j_expression* i_right_expression);

	Addition_Expression(const Addition_Expression&) = default;

	Addition_Expression* get_copy()const override{
		return new Addition_Expression(*this);
	}

	Addition_Expression* move_copy()override{
		return  new Addition_Expression(std::move(*this));
	}

	bool has_value()const override;
private:


	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //ADDITION_EXPRESSION_H



#ifndef UNARY_NEGATE_EXPRESSION_H
#define UNARY_NEGATE_EXPRESSION_H

#include "Unary_Expression.h"


namespace jomike{



class Unary_Negate_Expression : public Unary_Expression{
public:
	Unary_Negate_Expression(j_expression* i_expression);

	Unary_Negate_Expression* get_copy()const override{
		return new Unary_Negate_Expression(*this);
	}

	Unary_Negate_Expression* move_copy()override{
		return  new Unary_Negate_Expression(std::move(*this));
	}


private:

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //UNARY_NEGATE_EXPRESSION_H



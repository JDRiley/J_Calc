#ifndef PRE_INCREMENT_EXPRESSION_H
#define PRE_INCREMENT_EXPRESSION_H

#include "Unary_Expression.h"


namespace jomike{
class Pre_Increment_Expression : public Unary_Expression{
public:
	Pre_Increment_Expression(j_expression* i_expression);

	Pre_Increment_Expression* move_copy()override;


	Pre_Increment_Expression* get_copy()const override;
	j_symbol* make_non_referenced()const override;

	void process()override;
private:
	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //PRE_INCREMENT_EXPRESSION_H



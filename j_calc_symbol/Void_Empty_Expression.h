#ifndef EXPRESSION_WILD_H
#define EXPRESSION_WILD_H

#include "j_calc_expression.h"


namespace jomike{
class Void_Empty_Expression : public j_calc_expression{
public:
	using j_calc_expression::j_calc_expression;


	bool has_value()const override;

	Void_Empty_Expression* get_copy()const override;
	Void_Empty_Expression* move_copy()override;

	void process()override;

private:
	j_value derived_get_value(const Arguments& i_args)const override;

	void alert_symbol_scope_set()override;
};

}

#endif //EXPRESSION_WILD_H



#ifndef UNARY_OPERATOR_EXPRESSION_H
#define UNARY_OPERATOR_EXPRESSION_H

#include "j_calc_expression.h"


namespace jomike{
class Unary_Expression : public j_calc_expression{
public:
	Unary_Expression(const yy::location& irk_loc, j_calc_expression* i_expression);

	Unary_Expression(const Unary_Expression& irk_src);

	Unary_Expression(Unary_Expression&& irv_right);

	Unary_Expression& operator=(const Unary_Expression&) = delete;

	Unary_Expression* get_copy()const override= 0;
	
	Unary_Expression* move_copy()override = 0;
	bool has_value()const override;

	~Unary_Expression();

	j_calc_symbol* make_non_referenced()const override;

	

	void process()override;
protected:
	
	j_calc_expression& base_expression();


	bool has_expression()const;
	j_calc_expression& base_expression()const;

	void calculate_type();
	void alert_symbol_scope_set()override;
private:

	j_calc_expression* M_expression;

};

}

#endif //UNARY_OPERATOR_EXPRESSION_H



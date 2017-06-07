#ifndef EXPRESSION_STATEMENT_H
#define EXPRESSION_STATEMENT_H

#include "j_statement.h"


namespace jomike{
class Expression_Statement : public j_statement{
public:

	Expression_Statement(const yy::location& irk_loc, j_calc_expression* i_expression);

	Expression_Statement(const Expression_Statement& irk_source);
	Expression_Statement(Expression_Statement&& irv_source);


	Expression_Statement& operator=(const Expression_Statement&) = delete;
	Expression_Statement& operator=(Expression_Statement&&) = delete;


	Expression_Statement* get_copy()const override;

	Expression_Statement* move_copy()override;

	~Expression_Statement();

	void clear()override;

	bool has_value()const override;

	void process()override;

	
protected:
	j_value derived_get_value(const Arguments& i_args)const override;
private:

	void alert_symbol_scope_set()override;


	j_calc_expression* M_expression;
};

}

#endif //EXPRESSION_STATEMENT_H



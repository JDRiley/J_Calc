#ifndef FOR_STATEMENT_H
#define FOR_STATEMENT_H

#include "j_statement.h"


namespace jomike{
class For_Statement : public j_statement{
public:
	For_Statement(const yy::location& irk_loc
		, j_statement* i_init_statement
		, j_calc_expression* i_test_expression
		, j_calc_expression* i_post_expression
		, Statement_Block* i_statement_block);


	//Deleting Constructors and Assignment Operators
	For_Statement(const For_Statement& irk_src);
	For_Statement(For_Statement&& irv_src);
	For_Statement& operator=(const For_Statement&);
	For_Statement& operator=(For_Statement&& irv_right);

	For_Statement* get_copy()const override;
	
	For_Statement* move_copy()override;

	void clear()override;


	bool has_value()const override;

	void alert_symbol_scope_set()override;

	void process()override;

	~For_Statement();
private:
	j_statement* M_init_statement;
	j_calc_expression* M_test_expression;
	j_calc_expression* M_post_expression;
	Statement_Block* M_statement_block;

	

	void swap(For_Statement& ir_src);

	

	j_value derived_get_value(const Arguments& i_args)const override;
};

}

#endif //WHILE_STATEMENT_H



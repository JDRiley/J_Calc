#ifndef IF_STATEMENT_H
#define IF_STATEMENT_H

#include "j_statement.h"


namespace jomike{
class If_Statement : public j_statement{
public:
	If_Statement(
		j_expression* i_test_expression, Statement_Block* i_then_body
		, Statement_Block* i_else_body = nullptr);

	If_Statement(
		j_expression* i_test_expression, Statement_Block* i_then_body
		, If_Statement* i_else_body);

	If_Statement(
		j_expression* i_test_expression, j_expression* i_then_expression
		, j_expression* i_else_expression);
	
	//Deleting Constructors and Assignment Operators
	If_Statement(const If_Statement& irk_source);
	If_Statement(If_Statement&& irv_source);
	If_Statement& operator=(const If_Statement&) = delete;
	If_Statement& operator=(If_Statement&&) = delete;

	If_Statement* get_copy()const override;

	If_Statement* move_copy()override;

	void clear()override;

	bool has_value()const override;

	~If_Statement();

	//TODO: MAke this set_symbol_scope a derived function. to force implementation
	void set_symbol_scope(const J_Symbol_Scope* i_symbol_scope);
private:
	j_value derived_get_value(const Arguments& i_args)const override;


	//Members:
	j_expression* M_test_expression;
	Statement_Block* M_then_body;
	j_statement* M_else_body = nullptr;
};

}

#endif //IF_STATEMENT_H



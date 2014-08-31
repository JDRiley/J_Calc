#ifndef STATEMENT_BLOCK_H
#define STATEMENT_BLOCK_H

#include "j_statement.h"


namespace jomike{
class Statement_Block : public j_statement{
public:

	Statement_Block(Symbol_List* i_symbol_list);

	
	Statement_Block(const Statement_Block& irk_source);
	Statement_Block(Statement_Block&& irv_source);
	Statement_Block& operator=(const Statement_Block&) = delete;
	Statement_Block& operator=(Statement_Block&&) = delete;

	

	Statement_Block* get_copy()const override;	

	Statement_Block* move_copy()override;

	bool has_value()const;

	void clear();
	~Statement_Block();


	void set_symbol_scope(const J_Symbol_Scope* i_symbol_scope)override;
private:
	Symbol_List* M_symbol_list;

	j_value derived_get_value(const Arguments& i_args)const override;

	
};

}

#endif //STATEMENT_BLOCK_H



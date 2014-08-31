#ifndef J_STATEMENT_H
#define J_STATEMENT_H

#include "j_symbol.h"


namespace jomike{
class j_statement : public j_symbol{
public:
	
	j_statement();

	j_statement(J_Symbol_Identifier* i_identifier, Symbol_Types i_type);

	j_statement(const j_statement& irk_source);
	j_statement(j_statement&& irv_source);
	j_statement& operator=(const j_statement&) = delete;
	j_statement& operator=(j_statement&&) = delete;

	j_statement* get_copy()const override = 0;

	j_statement* move_copy()override = 0;
private:
};

}

#endif //J_STATEMENT_H



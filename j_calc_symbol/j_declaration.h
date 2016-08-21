#ifndef J_DECLARATION_H
#define J_DECLARATION_H

#include "j_statement.h"


namespace jomike{
class j_declaration : public j_statement{
public:
	using j_statement::j_statement;

	j_declaration(const j_declaration& irk_src);
	j_declaration(j_declaration&& irv_right);
	j_declaration* get_copy()const override = 0;

	j_declaration* move_copy()override = 0;
	~j_declaration();
private:
};

}

#endif //J_DECLARATION_H



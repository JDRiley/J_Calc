#ifndef J_DECLARATION_H
#define J_DECLARATION_H

#include "j_symbol.h"


namespace jomike{
class j_declaration : public j_symbol{
public:
	j_declaration(Type_Syntax* i_type_syntax, J_Symbol_Identifier* irk_name);


	j_declaration(const j_declaration& irk_src);
	j_declaration* get_copy()const override = 0;
	~j_declaration();
private:
};

}

#endif //J_DECLARATION_H



#ifndef J_DECLARATION_H
#define J_DECLARATION_H

#include "j_symbol.h"


namespace jomike{
class j_declaration : public j_symbol{
public:
	j_declaration(Type_Syntax* i_type_syntax, const J_UI_String& irk_name);

private:
};

}

#endif //J_DECLARATION_H



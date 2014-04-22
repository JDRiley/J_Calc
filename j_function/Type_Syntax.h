#ifndef TYPE_SYNTAX_H
#define TYPE_SYNTAX_H

#include "j_symbol_component.h"
//
#include "j_value.h"

namespace jomike{
class Type_Syntax : public j_symbol_component{
public:
	Type_Syntax(const std::string& irk_name, Symbol_Types i_symbol_type);

	Type_Syntax(const Type_Syntax&) = default;

	Type_Syntax* get_copy()const override = 0;

	Type_Syntax* move_copy()override = 0;

	virtual j_value convert_value(const j_value& irk_val)const;

private:
	std::string M_type_name;
};

}

#endif //TYPE_SYNTAX_H



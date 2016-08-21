#ifndef TYPE_SYNTAX_H
#define TYPE_SYNTAX_H

#include "j_calc_symbol_component.h"
//
#include "j_value.h"

namespace jomike{
class Type_Syntax : public j_calc_symbol_component{
public:
	Type_Syntax(
		const yy::location& irk_loc, const jc_string_t& irk_name, Symbol_Types i_symbol_type);

	Type_Syntax(const Type_Syntax&) = default;

	Type_Syntax* get_copy()const override = 0;

	Type_Syntax* move_copy()override = 0;

	virtual j_value convert_value(const j_value& irk_val)const;


	bool operator==(const Type_Syntax& irk_right)const;
	bool operator!=(const Type_Syntax& irk_right)const;
	const jc_string_t& type_name()const;

	virtual bool is_void()const;
private:
	jc_string_t M_type_name;

	virtual bool is_same_type(const Type_Syntax& irk_right)const;
};

}

#endif //TYPE_SYNTAX_H



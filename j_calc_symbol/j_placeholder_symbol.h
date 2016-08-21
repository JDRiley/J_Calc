#ifndef J_PLACEHOLDER_SYMBOL_H
#define J_PLACEHOLDER_SYMBOL_H

#include "j_expression.h"


namespace jomike{


class j_placeholder_symbol : public j_expression{
public:
	j_placeholder_symbol* get_copy()const override;

	j_placeholder_symbol* move_copy()override;
	bool has_value()const override;
	void clear()override;
	j_placeholder_symbol(const yy::location& irk_loc, j_size_t i_index);
	bool is_placeholder()const override;
	j_size_t placeholder_index()const;

	jc_string_t get_display_name()override;

	void process()override;
protected:
	j_value derived_get_value(const Arguments& i_args)const override;
private:
	j_size_t M_placeholder_index;

	void alert_symbol_scope_set()override;
};

}

#endif //J_PLACEHOLDER_SYMBOL_H


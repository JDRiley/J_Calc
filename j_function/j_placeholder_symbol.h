#ifndef J_PLACEHOLDER_SYMBOL_H
#define J_PLACEHOLDER_SYMBOL_H

#include "j_symbol.h"


namespace jomike{


class j_placeholder_symbol : public j_symbol{
public:
	j_placeholder_symbol* get_copy()const override;

	j_placeholder_symbol* move_copy()override;
	bool has_value()const override;
	void clear()override;
	j_placeholder_symbol(j_size_t i_index);
	bool is_placeholder()const override;
	j_size_t placeholder_index()const;
protected:
	j_value derived_get_value(const Arguments& i_args)const override;
private:
	j_size_t M_Placeholder_index;
};

}

#endif //J_PLACEHOLDER_SYMBOL_H


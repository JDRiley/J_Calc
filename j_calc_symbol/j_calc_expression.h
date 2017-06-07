#ifndef J_EXPRESSION_H
#define J_EXPRESSION_H

#include "j_calc_symbol.h"


namespace jomike{



class j_calc_expression : public j_calc_symbol{
public:
	using j_calc_symbol::j_calc_symbol;
	
	j_calc_expression* move_copy()override = 0;

	j_calc_expression* get_copy()const override = 0;


	void clear(){
		assert(0);
	}
	

	j_calc_expression* as_expression();
	jc_string_t get_display_name()override;


	~j_calc_expression();
protected:
	j_calc_expression(const j_calc_expression&) = default;


};


}

#endif //J_EXPRESSION_H



#ifndef J_EXPRESSION_H
#define J_EXPRESSION_H

#include "j_calc_symbol.h"


namespace jomike{



class j_expression : public j_calc_symbol{
public:
	using j_calc_symbol::j_calc_symbol;
	
	j_expression* move_copy()override = 0;

	j_expression* get_copy()const override = 0;


	void clear(){
		assert(0);
	}
	

	j_expression* as_expression();
	std::string get_display_name()override;


	~j_expression();
protected:
	j_expression(const j_expression&) = default;


};


}

#endif //J_EXPRESSION_H



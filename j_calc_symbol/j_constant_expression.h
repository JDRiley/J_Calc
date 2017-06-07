#ifndef JC_CONSTANT_SYMBOL_UTILITY_H
#define JC_CONSTANT_SYMBOL_UTILITY_H

#include "j_calc_expression.h"


namespace jomike{

class jc_constant_symbol : public j_calc_expression{
public:
	using j_calc_expression::j_calc_expression;

	jc_constant_symbol* get_copy()const override = 0;
	
	jc_constant_symbol* move_copy()override = 0;
};
template<typename Value_t>
class j_constant_expression : public jc_constant_symbol{
public:
	using jc_constant_symbol::jc_constant_symbol;
	bool has_value()const override{
		return true;
	}

	void process()override{

	}

	j_constant_expression* get_copy()const override = 0;
	
	j_constant_expression* move_copy()override = 0;
protected:
	virtual const Value_t& constant_symbol_value()const = 0;
private:

	j_value derived_get_value(const Arguments& i_args)const override{
		(void)i_args;
		return j_value(constant_symbol_value(), J_Unit());
	}


	void alert_symbol_scope_set()override{

	}


};

}

#endif //JC_CONSTANT_SYMBOL_UTILITY_H



#ifndef J_CONDITIONAL_CONSTRUCT_SYMBOL_H
#define J_CONDITIONAL_CONSTRUCT_SYMBOL_H

#include "j_construct_symbol.h"
//
#include <j_boolean_expression.h>

namespace jomike{

class j_conditional_construct_symbol : public j_construct_symbol{
public:
	
private:
	void derived_eval(const Arguments&)override;

	j_boolean_expression* M_boolean_expression = nullptr;
	ex_array<J_UI_String> M_true_statements;
	ex_array<J_UI_String> M_false_statements;
};
}

#endif //J_CONDITIONAL_CONSTRUCT_SYMBOL_H


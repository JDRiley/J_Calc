#ifndef J_BOOLEAN_EXPRESSION_H
#define J_BOOLEAN_EXPRESSION_H

#include "j_symbol.h"
//
#include "j_symbol_containers.h"
//
#include "boolean_tester_array.h"

namespace jomike{

class j_boolean_expression : public j_symbol{
public:
	j_boolean_expression();
	virtual bool has_value()const;
	j_boolean_expression* get_copy()const override;
	j_boolean_expression* move_copy()override;
	virtual void clear();

	J_UI_String get_display_name()override;
private:
	boolean_tester_array M_boolean_testers;
	j_symbol_array M_symbols;



	virtual j_value derived_get_value(const Arguments& i_args)const;

};

}

#endif //J_BOOLEAN_EXPRESSION_H


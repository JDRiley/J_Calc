#ifndef J_EXPRESSION_H
#define J_EXPRESSION_H

#include "j_symbol.h"


namespace jomike{
class j_expression : public j_symbol{
public:
	j_expression(Symbol_Types);

	j_expression* move_copy()override = 0;

	j_expression* get_copy()const override = 0;

	void clear(){
		assert(0);
	}
	

	j_expression* as_expression();
	J_UI_String get_display_name()override{
		if(has_value()){
			return get_value().to_str();
		}
			return name();
	}
private:
};


}

#endif //J_EXPRESSION_H



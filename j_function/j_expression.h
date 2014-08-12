#ifndef J_EXPRESSION_H
#define J_EXPRESSION_H

#include "j_symbol.h"


namespace jomike{



class j_expression : public j_symbol{
public:
	j_expression(Symbol_Types);
	j_expression(Symbol_Types i_symbol_type, J_Symbol_Identifier* i_identifier);

	j_expression* move_copy()override = 0;

	j_expression* get_copy()const override = 0;

	j_expression(const j_expression& irk_right);

	void clear(){
		assert(0);
	}
	

	j_expression* as_expression();
	J_UI_String get_display_name()override;

	

	~j_expression();

};


}

#endif //J_EXPRESSION_H



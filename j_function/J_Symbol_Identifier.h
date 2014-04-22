#ifndef J_SYMBOL_IDENTIFIER_H
#define J_SYMBOL_IDENTIFIER_H

#include "j_symbol_component.h"
//
#include <J_UI/J_UI_String.h>

namespace jomike{

class J_Symbol_Identifier : public j_symbol_component{
public:
	J_Symbol_Identifier(const J_UI_String& irk_name);

	J_Symbol_Identifier* get_copy()const override;
	J_Symbol_Identifier* move_copy()override;

	J_Symbol_Identifier(const J_Symbol_Identifier&) = default;
	
	const J_UI_String& identifier_name()const;

	void set_name(const J_UI_String& irk_name);

private:
	J_UI_String M_name;
};



}

#endif //J_SYMBOL_IDENTIFIER_H



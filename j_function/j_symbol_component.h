#ifndef J_SYMBOL_COMPONENT_H
#define J_SYMBOL_COMPONENT_H

#include "J_Symbol_Fwd_Decl.h"
//
#include <J_UI/J_UI_String.h>

//
#include "Symbol_Types.h"




namespace jomike{
class j_symbol_component{
public:
	j_symbol_component(Symbol_Types i_symbol_type
					   , const J_UI_String& irk_name = J_UI_String("%Unnamed"));

	

	virtual j_symbol_component* get_copy()const = 0;
	virtual j_symbol_component* move_copy() = 0;

	const J_UI_String& name()const;

	void set_name(const J_UI_String& irk_string);
	
	void set_type(Symbol_Types i_symbol_type);
	Symbol_Types symbol_type()const;

	virtual ~j_symbol_component();
private:
	const int M_ID;
	J_UI_String M_name;
	Symbol_Types M_type;
};

}


#endif //J_SYMBOL_COMPONENT_H



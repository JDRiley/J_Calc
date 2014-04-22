#ifndef J_SYMBOL_COMPONENT_H
#define J_SYMBOL_COMPONENT_H

#include "J_Symbol_Fwd_Decl.h"


//
#include "Symbol_Types.h"
//
#include <J_UI/J_UI_Fwd_Decl.h>



namespace jomike{
class j_symbol_component{
public:
	j_symbol_component(Symbol_Types i_symbol_type);

	virtual j_symbol_component* get_copy()const = 0;
	virtual j_symbol_component* move_copy() = 0;

	
	
	void set_type(Symbol_Types i_symbol_type);
	Symbol_Types symbol_type()const;

	virtual ~j_symbol_component();
private:
	const int M_ID;
	
	Symbol_Types M_type;
};

}


#endif //J_SYMBOL_COMPONENT_H



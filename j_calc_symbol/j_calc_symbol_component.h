#ifndef J_CALC_SYMBOL_COMPONENT_H
#define J_CALC_SYMBOL_COMPONENT_H

#include <j_symbol/j_symbol.h>
//
#include "Symbol_Types.h"

#ifdef WIN32
#pragma comment(lib, "j_calc_symbol" J_LIB_APPEND)
#endif


namespace jomike{
class j_calc_symbol_component : public j_symbol{
public:
	
	j_calc_symbol_component(const yy::location& irk_loc, Symbol_Types i_symbol_type);

	void set_type(Symbol_Types i_symbol_type);
	Symbol_Types symbol_type()const;

private:
	Symbol_Types M_symbol_type;

};

}

#endif //J_CALC_SYMBOL_COMPONENT_H



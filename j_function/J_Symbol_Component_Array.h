#ifndef J_SYMBOL_COMPONENT_ARRAY_H
#define J_SYMBOL_COMPONENT_ARRAY_H
//
#include "J_Symbol_Component_Allocator.h"
//
#include <ex_array.h>

namespace jomike{

template<typename Symbol_Component_t>
using J_Symbol_Component_Array
	= ex_array<Symbol_Component_t*, J_Symbol_Component_Allocator<Symbol_Component_t>>;



}


#endif //J_SYMBOL_COMPONENT_ARRAY_H



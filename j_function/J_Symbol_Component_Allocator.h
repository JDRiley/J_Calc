#ifndef J_SYMBOL_COMPONENT_ALLOCATOR_H
#define J_SYMBOL_COMPONENT_ALLOCATOR_H

#include "j_symbol_component.h"


namespace jomike{

template<typename Symbol_Component_t>
class J_Symbol_Component_Allocator{
public:
	Symbol_Component_t** allocate(j_size_t i_size);
	void deallocate(Symbol_Component_t**, std::ptrdiff_t);
	void construct(Symbol_Component_t** p, Symbol_Component_t*);
	void construct(Symbol_Component_t** p, const Symbol_Component_t&);
	void destroy(Symbol_Component_t**);
};

template<typename Symbol_Component_t>
Symbol_Component_t** J_Symbol_Component_Allocator<Symbol_Component_t>
	::allocate(j_size_t i_size){
	return static_cast<Symbol_Component_t**>(
		::operator new(sizeof(Symbol_Component_t*)*i_size));
}

template<typename Symbol_Component_t>
void J_Symbol_Component_Allocator<Symbol_Component_t>
	::deallocate(Symbol_Component_t** i_ptr, std::ptrdiff_t ){
	::operator delete(i_ptr);
}

template<typename Symbol_Component_t>
void J_Symbol_Component_Allocator<Symbol_Component_t>
	::destroy(Symbol_Component_t** i_ptr){
	delete *i_ptr;
}

template<typename Symbol_Component_t>
void J_Symbol_Component_Allocator<Symbol_Component_t>
	::construct(Symbol_Component_t** i_dest, Symbol_Component_t* i_ptr){
	*i_dest = i_ptr->get_copy();
}


template<typename Symbol_Component_t>
void J_Symbol_Component_Allocator<Symbol_Component_t>
::construct(Symbol_Component_t** i_dest, const Symbol_Component_t& irk_val){
	*i_dest = irk_val.get_copy();
}



}

#endif //J_SYMBOL_COMPONENT_ARRAY_H



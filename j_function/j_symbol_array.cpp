#include "j_symbol_containers.h"
//
#include "j_symbol.h"

namespace jomike{

j_symbol** j_symbol_allocator::allocate(j_size_t i_size){
	return static_cast<j_symbol**>(::operator new(sizeof(j_symbol*)*i_size));
}

void j_symbol_allocator::deallocate(j_symbol** i_ptr, ptrdiff_t){
::operator delete(i_ptr);
}

void j_symbol_allocator::destroy(j_symbol** i_ptr){
	delete *i_ptr;
}

void j_symbol_allocator::construct(j_symbol** i_dest, j_symbol* i_ptr){
	*i_dest = i_ptr->get_copy();
}

}




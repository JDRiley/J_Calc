#include "boolean_tester_array.h"
//
#include "Boolean_Testers.h"

namespace jomike{

Boolean_Tester** Boolean_Tester_Allocator::allocate(j_size_t i_size){
	return ::operator new(sizeof(Boolean_Tester*)*i_size);
}

void Boolean_Tester_Allocator::deallocate(Boolean_Tester** i_ptr, ptrdiff_t){
	::operator delete(i_ptr);
}

void Boolean_Tester_Allocator::destroy(Boolean_Tester** i_ptr){
	delete *i_ptr;
}

void Boolean_Tester_Allocator::construct(Boolean_Tester** i_dest, Boolean_Tester* i_src){
	*i_dest = i_src->get_copy();
}

}




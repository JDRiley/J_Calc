//
#include <j_symbol/restrictive_ptr.h>
//
#include "../J_Constant_Symbol.h"
//
#include <j_symbol/Specific_Symbol_List.h>
//
#include <j_symbol/J_Symbol_Identifier.h>


//Put necessary class definition includes here
// so the destructors and copy/move constructors are defined

namespace jomike{

template<typename St, typename Parent_t>
St* restrictive_ptr<St, Parent_t>::copy_ptr()const{
	return M_ptr ? M_ptr->get_copy() : nullptr;
}

template<typename St, typename Parent_t>
void restrictive_ptr<St, Parent_t>::swap(base_restrictive_ptr& ir_right){
	restrictive_ptr<St, Parent_t>* src_ptr = dynamic_cast<restrictive_ptr<St, Parent_t>*>(&ir_right);

	assert(src_ptr);
	swap(*src_ptr);
}


template<typename St, typename Parent_t>
void restrictive_ptr<St, Parent_t>::swap(
	restrictive_ptr<St, Parent_t>& ir_right){
	std::swap(M_ptr, ir_right.M_ptr);
}

template<typename St, typename Parent_t>
restrictive_ptr<St, Parent_t>::~restrictive_ptr(){
	destroy();
}


template<typename St, typename Parent_t>
void restrictive_ptr<St, Parent_t>::destroy(){
	delete M_ptr;
	M_ptr = nullptr;
}

}
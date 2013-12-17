#ifndef J_GADGET_CLASSES_H
#define J_GADGET_CLASSES_H

#include <J_Fwd_Decl.h>
//
#include <algorithm>
//
#include <functional>
//
#include <RB_Tree.h>
namespace jomike{

template<typename Val_t, typename Comp_t = std::less<Val_t>>
class Delimiter_Handler{
public:
	template<typename Iter>
	Delimiter_Handler(Iter, Iter);
	bool is_delim(Val_t)const;
	void set_delim_status(const Val_t& irk_val, bool i_status);
private:
	RB_Tree<Val_t, Val_t, Comp_t> M_delims;
};

template<typename Val_t, typename Comp_t>
bool Delimiter_Handler<Val_t, Comp_t>::is_delim(Val_t i_val)const{
	return M_delims.end() != M_delims.find(i_val);
}


template<typename Val_t, typename Comp_t>
void Delimiter_Handler<Val_t, Comp_t>::set_delim_status(const Val_t& irk_val, bool i_status){
	if(i_status){
		M_delims.insert(irk_val);
	} else{
		M_delims.erase(irk_val);
	}
}

template<typename Val_t, typename Comp_t>
template<typename Iter>
Delimiter_Handler<Val_t, Comp_t>::Delimiter_Handler(Iter i_delim_begin, Iter i_delim_end){
	std::for_each(i_delim_begin, i_delim_end
		, std::bind(&Delimiter_Handler<Val_t>::set_delim_status
			, this, std::placeholders::_1, true));

}

}

#endif //J_GADGET_CLASSES_H


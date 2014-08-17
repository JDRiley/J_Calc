#ifndef J_GADGET_CLASSES_H
#define J_GADGET_CLASSES_H

#include <J_Fwd_Decl.h>
//
#include <algorithm>
//
#include <functional>
//
#include <j_tree.h>
//
#include <initializer_list>
//
#include <ex_array.h>
namespace jomike{

template<typename Val_t, typename Comp_t = std::less<Val_t>>
class Delimiter_Handler{
public:
	template<typename Iter>
	Delimiter_Handler(Iter, Iter);
	bool is_delim(Val_t)const;
	void set_delim_status(const Val_t& irk_val, bool i_status);

	template<typename St>
	Delimiter_Handler(std::initializer_list<St> i_list);
private:
	j_tree<Val_t, Val_t, Comp_t> M_delims;
};

template<typename Val_t, typename Comp_t /*= std::less<Val_t>*/>
template<typename St>
Delimiter_Handler<Val_t, Comp_t>::Delimiter_Handler(std::initializer_list<St> i_list)
	: Delimiter_Handler(i_list.begin(), i_list.end()){}

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


template<typename Array_t>
class Array_Modifier_Manger{
public:


	Array_Modifier_Manger(const std::function<void()>& irk_func)
		:M_notification_function(irk_func){}
	Array_t& operator*(){
		M_notification_function();
		return M_array;
	}

	typename Array_t::value_type& operator[](j_size_t i_index){
		return operator->()->operator[](i_index);
	}

	Array_t* operator->(){
		M_notification_function();
		return &M_array;
	}

	const Array_t* operator->()const{
		M_notification_function();
		return &M_array;
	}


	j_size_t size()const{
		return M_array.size();
	}
private:
	Array_t M_array;
	std::function<void()> M_notification_function;
};



}

#endif //J_GADGET_CLASSES_H


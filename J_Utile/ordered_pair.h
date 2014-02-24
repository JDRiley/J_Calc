#ifndef ORDERED_PAIR_H
#define ORDERED_PAIR_H

#include "J_Fwd_Decl.h"
//
#include <functional>

namespace jomike{


template<typename St, typename Comp_t = std::less<St>>
class ordered_pair{
public:
	ordered_pair();
	ordered_pair(const St& irk_val, const St& irk_val2);
	St& first();
	St& second();

	j_size_t diff_size()const;

	const St& first()const;
	const St& second()const;

	void set_first_unordered_elem(const St&);
	void set_second_unordered_elem(const St&);

private:
	St M_datums_unordered[2];
	St* M_first, * M_second;

	void set_pointers();
};

template<typename St, typename Comp_t /*= std::less<St>*/>
j_size_t ordered_pair<St, Comp_t>::diff_size()const{
	return second() - first();
}

template<typename St, typename Comp_t /*= std::less<St>*/>
ordered_pair<St, Comp_t>::ordered_pair(){
	set_pointers();
}

template<typename St, typename Comp_t /*= std::less<St>*/>
void ordered_pair<St, Comp_t>::set_pointers(){
	M_first = M_datums_unordered[0] > M_datums_unordered[1] 
		? M_datums_unordered + 1 : M_datums_unordered;


	M_second = M_first == M_datums_unordered 
		? M_datums_unordered + 1 : M_datums_unordered;
}

template<typename St, typename Comp_t /*= std::less<St>*/>
void ordered_pair<St, Comp_t>::set_first_unordered_elem(const St& irk_val){
	M_datums_unordered[0] = irk_val;
	set_pointers();
}

template<typename St, typename Comp_t /*= std::less<St>*/>
void ordered_pair<St, Comp_t>::set_second_unordered_elem(const St& irk_val){
	M_datums_unordered[1] = irk_val;
	set_pointers();
}

template<typename St, typename Comp_t /*= std::less<St>*/>
const St& ordered_pair<St, Comp_t>::second()const{
	return *M_second;
}

template<typename St, typename Comp_t /*= std::less<St>*/>
const St& ordered_pair<St, Comp_t>::first()const{
	return *M_first;
}

template<typename St, typename Comp_t /*= std::less<St>*/>
ordered_pair<St, Comp_t>::ordered_pair(const St& irk_val, const St& irk_val2){
	M_datums_unordered[0] = irk_val;
	M_datums_unordered[1] = irk_val2;

	set_pointers();
}

template<typename St, typename Comp_t /*= std::less<St>*/>
St& ordered_pair<St, Comp_t>::first(){
	return *M_first;
}

template<typename St, typename Comp_t /*= std::less<St>*/>
St& ordered_pair<St, Comp_t>::second(){
	return *M_second;
}

}

#endif //ORDERED_PAIR_H


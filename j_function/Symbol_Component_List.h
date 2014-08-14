#ifndef SYMBOL_LIST_H
#define SYMBOL_LIST_H

#include "j_symbol.h"
//
#include "J_Symbol_Component_Array.h"

namespace jomike{

class Symbol_Component_List : public j_symbol{
public:
	


	template <typename Symbol_Component_t>
	class iterator_type;

	typedef J_Symbol_Component_Array<j_symbol_component>::const_iterator symbol_iterator;

	symbol_iterator begin()const;
	symbol_iterator end()const;

	j_symbol_component& operator[](j_size_t i_index){
		return *M_components[i_index];
	}

	const j_symbol_component& operator[](j_size_t i_index)const{
		return *M_components[i_index];
	}

	Symbol_Component_List* get_copy()const override = 0;
	Symbol_Component_List* move_copy()override = 0;
protected:
	void add_symbol_component(const j_symbol_component& irk_symbol_component);
	void add_symbol_component(j_symbol_component* i_symbol_component);

	virtual void clear();

	virtual bool has_value()const;

	virtual j_value derived_get_value(const Arguments& i_args)const;



private:
	J_Symbol_Component_Array<j_symbol_component> M_components;


};


template<typename Symbol_Component_t>
class Symbol_Component_List::iterator_type
	: public Symbol_Component_List::symbol_iterator{
private:
	typedef Symbol_Component_List::symbol_iterator M_Base_t;
public:

	iterator_type(const Symbol_Component_List::symbol_iterator& irk_base);

	const Symbol_Component_t* operator*()const;

	const Symbol_Component_t** operator->()const;
};

template<typename Symbol_Component_t>
const Symbol_Component_t** Symbol_Component_List::iterator_type<Symbol_Component_t>::operator->()const{
	const Symbol_Component_t** syntax_type
		= dynamic_cast<const Symbol_Component_t**>(M_Base_t::operator->());
	assert(syntax_type);
	return syntax_type;
}


template<typename Symbol_Component_t>
const Symbol_Component_t* Symbol_Component_List::iterator_type<Symbol_Component_t>::operator*()const{
	const Symbol_Component_t* syntax_type
		= dynamic_cast<const Symbol_Component_t*>(M_Base_t::operator*());
	assert(syntax_type);
	return syntax_type;
}


template<typename Symbol_Component_t>
Symbol_Component_List::iterator_type<Symbol_Component_t>::iterator_type(const Symbol_Component_List::symbol_iterator& irk_base):Symbol_Component_List::symbol_iterator(irk_base){

}


}

#endif //SYMBOL_LIST_H



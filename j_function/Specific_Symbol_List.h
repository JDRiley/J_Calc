#ifndef SPECIFIC_SYMBOL_LIST_H
#define SPECIFIC_SYMBOL_LIST_H

#include "Symbol_Component_List.h"


namespace jomike{

template<typename Symbol_t>
class Specific_Symbol_List : public Symbol_Component_List{
public:
	
	Specific_Symbol_List(const std::initializer_list<Symbol_t*> irk_list){
		for(auto symbol_ptr : irk_list){
			add_symbol(symbol_ptr);
		}
	}

	Specific_Symbol_List(){}

	void add_symbol(Symbol_t* i_symbol);
	void add_symbol(const Symbol_t& irk_symbol);
	Specific_Symbol_List* get_copy()const override{
		return new Specific_Symbol_List(*this);
	}

	Specific_Symbol_List* move_copy()override{
		return  new Specific_Symbol_List(std::move(*this));
	}

	Symbol_t& operator[](j_size_t i_index);

	const Symbol_t& operator[](j_size_t i_index)const;

	typedef Symbol_Component_List::iterator_type<Symbol_t> iterator;

	iterator begin()const{ return iterator(Symbol_Component_List::begin()); }
	iterator end()const{ return iterator(Symbol_Component_List::end()); }

private:
};



template<typename Symbol_t>
void Specific_Symbol_List<Symbol_t>::add_symbol(const Symbol_t& irk_symbol){
	add_symbol(irk_symbol.get_copy());
}

template<typename Symbol_t>
void Specific_Symbol_List<Symbol_t>::add_symbol(Symbol_t* i_symbol){
		add_symbol_component(i_symbol);
}

template<typename Symbol_t>
Symbol_t& Specific_Symbol_List<Symbol_t>::operator[](j_size_t i_index){
	auto symbol = dynamic_cast<Symbol_t*>(&Symbol_Component_List::operator[](i_index));
	assert(symbol);
	return *symbol;
}

template<typename Symbol_t>
const Symbol_t& Specific_Symbol_List<Symbol_t>::operator[](j_size_t i_index)const{
	auto symbol = dynamic_cast<const Symbol_t*>(&Symbol_Component_List::operator[](i_index));
	assert(symbol);
	return *symbol;
}

}

#endif //SPECIFIC_SYMBOL_LIST_H



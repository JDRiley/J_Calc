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
	typedef Symbol_Component_List::const_iterator_type<Symbol_t> const_iterator;

	const_iterator begin()const{ return const_iterator(Symbol_Component_List::begin()); }
	const_iterator end()const{ return const_iterator(Symbol_Component_List::end()); }

	iterator begin(){ return iterator(Symbol_Component_List::begin()); }
	iterator end(){ return iterator(Symbol_Component_List::end()); }

	bool has_value()const override;

	void process()override;

private:

	void alert_symbol_scope_set()override;
};

template<typename St>
typename std::enable_if<std::is_base_of<j_symbol, St>::value, bool>::type
has_value_helper(const Specific_Symbol_List<St>* ik_symbol_list){
	return std::all_of(ik_symbol_list->begin(), ik_symbol_list->end()
					   , std::mem_fn(&St::has_value));
}


template<typename St>
typename std::enable_if<!std::is_base_of<j_symbol, St>::value, bool>::type
has_value_helper(const Specific_Symbol_List<St>* /*ik_symbol_list*/){
	return false;
}
template<typename Symbol_t>
bool Specific_Symbol_List<Symbol_t>::has_value()const {
	return has_value_helper(this);

}



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




template<typename St>
typename std::enable_if<std::is_base_of<j_symbol, St>::value, void>::type
set_symbol_scope_helper(
Specific_Symbol_List<St>* ik_symbol_list, J_Symbol_Scope* ik_scope){
	for(auto f_symbol : *ik_symbol_list){
		f_symbol->set_symbol_scope(ik_scope);
	}
}


template<typename St>
typename std::enable_if<!std::is_base_of<j_symbol, St>::value, void>::type
set_symbol_scope_helper(Specific_Symbol_List<St>* /*ik_symbol_list*/
, const J_Symbol_Scope*){
	
}

template<typename Symbol_t>
void jomike::Specific_Symbol_List<Symbol_t>::alert_symbol_scope_set(){
	set_symbol_scope_helper(this, &symbol_scope());
}


template<typename St>
typename std::enable_if<std::is_base_of<j_symbol, St>::value, void>::type
process_helper(Specific_Symbol_List<St>* i_symbol_list){
	for(auto f_symbol : *i_symbol_list){
		f_symbol->process();
	}
}


template<typename St>
typename std::enable_if<!std::is_base_of<j_symbol, St>::value, void>::type
process_helper(Specific_Symbol_List<St>* /*i_symbol_list*/){

}

template<typename Symbol_t>
void jomike::Specific_Symbol_List<Symbol_t>::process(){
	process_helper(this);
}



}

#endif //SPECIFIC_SYMBOL_LIST_H



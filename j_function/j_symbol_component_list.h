#ifndef J_SYMBOL_LIST_H
#define J_SYMBOL_LIST_H

#include "j_symbol.h"
//
#include "J_Symbol_Component_Array.h"

namespace jomike{


class j_symbol_component_list : public j_symbol_component{
public:
	j_symbol_component_list();


	template <typename Symbol_Component_t>
	class iterator_type;

	typedef J_Symbol_Component_Array<j_symbol_component>::const_iterator syntax_iterator;

	syntax_iterator begin()const{ return M_components.begin(); }
	syntax_iterator end()const {
		return M_components.end();
	}



protected:
	void add_syntax_component(const j_symbol_component& i_component);

private:
	J_Symbol_Component_Array<j_symbol_component> M_components;

	
};


template<typename Symbol_Component_t>
class j_symbol_component_list::iterator_type 
	: public j_symbol_component_list::syntax_iterator{
private:
	typedef j_symbol_component_list::syntax_iterator M_Base_t;
public:

	iterator_type(const j_symbol_component_list::syntax_iterator& irk_base)
		:j_symbol_component_list::syntax_iterator(irk_base){}

	const Symbol_Component_t* operator*()const{
		const Symbol_Component_t* syntax_type
			= dynamic_cast<const Symbol_Component_t*>(M_Base_t::operator*());
		assert(syntax_type);
		return syntax_type;
	}

	const Symbol_Component_t** operator->()const{
		const Symbol_Component_t** syntax_type
			= dynamic_cast<const Symbol_Component_t**>(M_Base_t::operator->());
		assert(syntax_type);
		return syntax_type;
	}
};

}

#endif //J_SYMBOL_LIST_H



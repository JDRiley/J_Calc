#ifndef EXPRESSION_LIST_H
#define EXPRESSION_LIST_H

#include "j_symbol_component_list.h"


namespace jomike{
class Expression_List : public j_symbol_component_list{
public:
	Expression_List();
	
	
	Expression_List* get_copy()const override{
		return new Expression_List(*this);
	}

	Expression_List* move_copy()override{
		return  new Expression_List(std::move(*this));
	}
	
	typedef j_symbol_component_list::iterator_type<j_expression> iterator;

	iterator begin()const;
	iterator end() const;


	void add_expression(const j_expression& irk_declaration);
private:

};
}

#endif //EXPRESSION_LIST_H



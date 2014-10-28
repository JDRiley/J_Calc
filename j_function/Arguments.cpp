#include "Arguments.h"
//
#include "j_function.h"
//
#include "j_expression.h"
//Algorithm
#include <algorithm>
//
#include <functional>
//
#include "j_placeholder_symbol.h"
//
#include <utility>

using std::for_each; using std::mem_fn; using std::transform;

namespace jomike{
//Arguments Function
Arguments::Arguments():j_symbol_component(Symbol_Types::ARGUMENTS){}







/*Arguments(Arguments&&)*/
Arguments::Arguments(Arguments&& irr_src)
:j_symbol_component(std::move(irr_src)){}

Arguments::Arguments(j_size_t i_size): j_symbol_component(Symbol_Types::ARGUMENTS){
	
	for(int i = 0; i < i_size; i++){
		M_arg_symbols.emplace_back(j_placeholder_symbol(i));
	}
}

template<typename Iter>
Arguments::Arguments(Iter i_first, Iter i_last){
	while(i_first != i_last){
		push_back(**i_first);
		++i_first;
	}
}

Arguments::Arguments(const Arguments& irk_right)
	:j_symbol_component(irk_right)
	, M_arg_symbols(irk_right.M_arg_symbols){

}

/*void swap(Arguments& ir_src)*/
void Arguments::swap(Arguments& ir_src){M_arg_symbols.swap(ir_src.M_arg_symbols);}


/*Arguments& operator=(const Arguments&)*/
Arguments& Arguments::operator=(const Arguments& irk_src){
	Arguments temp(irk_src);
	swap(temp);
	return *this;
}

/*Arguments& operator=(Arguments&&)*/
Arguments& Arguments::operator=(Arguments&& rr_src){swap(rr_src); return *this;}

/*~Arguments()*/
Arguments::~Arguments(){}

void Arguments::clear(){
	Arguments empty_arguments;
	swap(empty_arguments);
}

/*int size()const*/
j_size_t Arguments::size()const{return M_arg_symbols.size();}

/*void push_back(const j_symbol*)*/
void Arguments::push_back(const j_expression& i_sym){
	M_arg_symbols.emplace_back(i_sym);
}

void Arguments::push_back(j_expression* i_symbol){
	M_arg_symbols.emplace_back(i_symbol);
	delete i_symbol;
}

/*void set_argument(int index, const j_symbol*)*/
void Arguments::set_argument(j_size_t i_index, const j_expression* i_symbol_ptr){
	assert(i_index < size());

	delete M_arg_symbols[i_index];
	M_arg_symbols[i_index] = i_symbol_ptr->get_copy();

}

/*j_symbol** arguments()*/
j_expression* const* Arguments::arguments(){return M_arg_symbols.data();}

/*const j_symbol* const* arguments()const*/
const j_expression* const* Arguments::arguments()const{return &M_arg_symbols[0];}

/*const j_symbol& operator[](j_size_t i_index)*/
j_expression& Arguments::operator[](j_size_t i_index){return *M_arg_symbols[i_index];}

/*const j_symbol& operator[](j_size_t i_index)const*/
const j_expression& Arguments::operator[](j_size_t i_index)const{return *M_arg_symbols[i_index];}

bool Arguments::empty()const{
	return M_arg_symbols.empty();
}

Arguments::const_iterator Arguments::begin()const{
	return M_arg_symbols.begin();
}

Arguments::const_iterator Arguments::end()const{
	return M_arg_symbols.end();
}


Arguments* Arguments::move_copy(){
	return  new Arguments(std::move(*this));
}

Arguments* Arguments::get_copy()const {
	return new Arguments(*this);
}

void Arguments::set_symbol_scope(J_Symbol_Scope* i_scope){
	M_arg_symbols.apply([i_scope](j_expression* y_expression){
		y_expression->set_symbol_scope(i_scope);
	});
}

}
#include "Arguments.h"
//
#include "j_function.h"
//
#include "j_calc_expression.h"
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







Arguments::Arguments(const yy::location& irk_loc, j_size_t i_size)
	: j_calc_symbol_component(irk_loc, Symbol_Types::ARGUMENTS){
	
	for(int i = 0; i < i_size; i++){
		M_arg_symbols.emplace_back(j_placeholder_symbol(irk_loc, i));
	}
}

template<typename Iter>
Arguments::Arguments(Iter i_first, Iter i_last){
	while(i_first != i_last){
		push_back(**i_first);
		++i_first;
	}
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
	M_arg_symbols.clear();
}

/*int size()const*/
j_size_t Arguments::size()const{return M_arg_symbols.size();}

/*void push_back(const j_symbol*)*/
void Arguments::push_back(const j_calc_expression& i_sym){
	M_arg_symbols.emplace_back(i_sym);
}

void Arguments::push_back(j_calc_expression* i_symbol){
	M_arg_symbols.emplace_back(i_symbol);
	delete i_symbol;
}

/*void set_argument(int index, const j_symbol*)*/
void Arguments::set_argument(j_size_t i_index, const j_calc_expression* i_symbol_ptr){
	assert(i_index < size());

	delete M_arg_symbols[i_index];
	M_arg_symbols[i_index] = i_symbol_ptr->get_copy();

}

/*j_symbol** arguments()*/
j_calc_expression* const* Arguments::arguments(){return M_arg_symbols.data();}

/*const j_symbol* const* arguments()const*/
const j_calc_expression* const* Arguments::arguments()const{return &M_arg_symbols[0];}

/*const j_symbol& operator[](j_size_t i_index)*/
j_calc_expression& Arguments::operator[](j_size_t i_index){return *M_arg_symbols[i_index];}

/*const j_symbol& operator[](j_size_t i_index)const*/
const j_calc_expression& Arguments::operator[](j_size_t i_index)const{return *M_arg_symbols[i_index];}

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
	M_arg_symbols.apply([i_scope](j_calc_expression* y_expression){
		y_expression->set_symbol_scope(i_scope);
	});
}

}
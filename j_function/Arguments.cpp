#include "Arguments.h"
//
#include "j_function.h"

//Algorithm
#include <algorithm>
#include <functional>

//
#include <utility>
using std::for_each; using std::mem_fn; using std::transform;

namespace jomike{
//Arguments Function
Arguments::Arguments(){}





/*Arguments(const Arguments&)*/
Arguments::Arguments(const Arguments& irk_src):M_arg_symbols(irk_src.size()){
	transform(irk_src.M_arg_symbols.begin(), irk_src.M_arg_symbols.end()
		, M_arg_symbols.begin(), mem_fn(&j_symbol::get_copy));
}

/*Arguments(Arguments&&)*/
Arguments::Arguments(Arguments&& rr_src):j_symbol_component(Symbol_Types::ARGUMENTS){swap(rr_src);}

Arguments::Arguments(j_size_t i_size){
	
	for(int i = 0; i < i_size; i++){
		M_arg_symbols.push_back(new j_placeholder_symbol(i));
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
Arguments::~Arguments(){
	for_each(M_arg_symbols.begin(), M_arg_symbols.end()
		, [](j_symbol* y_sym_ptr){delete y_sym_ptr;});
}

void Arguments::clear(){
	Arguments empty_arguments;
	swap(empty_arguments);
}

/*int size()const*/
j_size_t Arguments::size()const{return M_arg_symbols.size();}

/*void push_back(const j_symbol*)*/
void Arguments::push_back(const j_symbol& i_sym){
	M_arg_symbols.push_back(i_sym.get_copy());
}

/*void set_argument(int index, const j_symbol*)*/
void Arguments::set_argument(j_size_t i_index, const j_symbol* i_symbol_ptr){
	assert(i_index < size());

	delete M_arg_symbols[i_index];
	M_arg_symbols[i_index] = i_symbol_ptr->get_copy();

}

/*j_symbol** arguments()*/
j_symbol* const* Arguments::arguments(){return M_arg_symbols.data();}

/*const j_symbol* const* arguments()const*/
const j_symbol* const* Arguments::arguments()const{return &M_arg_symbols[0];}

/*const j_symbol& operator[](j_size_t i_index)*/
j_symbol& Arguments::operator[](j_size_t i_index){return *M_arg_symbols[i_index];}

/*const j_symbol& operator[](j_size_t i_index)const*/
const j_symbol& Arguments::operator[](j_size_t i_index)const{return *M_arg_symbols[i_index];}

bool Arguments::empty()const{
	return M_arg_symbols.empty();
}

Arguments::const_iterator Arguments::begin()const{
	return M_arg_symbols.begin();
}

Arguments::const_iterator Arguments::end()const{
	return M_arg_symbols.end();
}

jomike::J_UI_String Arguments::get_display_name(){
	return name();
}

Arguments* Arguments::move_copy(){
	return  new Arguments(std::move(*this));
}

Arguments* Arguments::get_copy()const {
	return new Arguments(*this);
}

}
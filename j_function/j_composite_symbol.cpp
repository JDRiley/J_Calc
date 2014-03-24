#include "j_composite_symbol.h"
//
#include <functional>
//
#include <algorithm>
//
#include "Add_Symbol_Joiner.h"
//
#include <string>
using std::mem_fn; using std::transform;
using std::to_string;

namespace jomike{


//j_composite_symbol******************************
j_composite_symbol* j_composite_symbol::get_copy()const{ return new j_composite_symbol(*this); }

//Constructor
j_composite_symbol::j_composite_symbol():j_symbol(Symbol_Types::COMPOSITE){}

/*j_composite_symbol(const j_composite_symbol&)*/
j_composite_symbol::j_composite_symbol(const j_composite_symbol& irk_src): j_symbol(irk_src){
	j_size_t src_size = irk_src.size();
	M_symbol_joiners.resize(src_size);
	M_symbols.resize(src_size);

	transform(irk_src.M_symbol_joiners.begin(), irk_src.M_symbol_joiners.end()
		, M_symbol_joiners.begin(), mem_fn(&Symbol_Joiner::get_copy));



	transform(irk_src.M_symbols.begin(), irk_src.M_symbols.end(), M_symbols.begin()
		, mem_fn(&j_symbol::get_copy));
}
/*j_value get_value(Arguments)const */
j_value j_composite_symbol::derived_get_value(const Arguments& i_arguments)const{
	//Assuming only plus joiner for now
	if(M_symbols.empty()){
		return j_value(0.0, J_Unit());
	}
	j_value value = M_symbols[0]->get_value(i_arguments);

	for(int i = 1; i < M_symbols.size(); i++){
		value += M_symbols[i]->get_value(i_arguments);
	}

	return value;
}

/*bool has_value()const*/
bool j_composite_symbol::has_value()const{
	return all_of(M_symbols.begin(), M_symbols.end(), mem_fn(&j_symbol::has_value));
}

J_UI_String j_composite_symbol::get_display_name(){
	J_UI_String display_name;

	if(has_value()){
		display_name.push_back(' ');
		return display_name + to_string(get_value(Arguments()).as_double());
	}

	for(auto symbol : M_symbols){
		display_name.append(symbol->get_display_name()).push_back(' ');
	}
	return display_name;
}


/*j_composite_symbol* append(Join_Type, const j_symbol*)*/
void j_composite_symbol::append(Join_Type i_join_type, const j_symbol& irk_symbol){
	Symbol_Joiner* symbol_joiner_ptr;
	switch(i_join_type){
	case Join_Type::ADD:
		symbol_joiner_ptr = new Add_Symbol_Joiner;
		break;
	case Join_Type::DIVIDE:
		throw J_Argument_Error("divide joiner not implemented");
		break;
	case Join_Type::MULTIPLY:
		throw J_Argument_Error("multiply joiner not implemented");
		break;
	case Join_Type::SUBTRACT:
		throw J_Argument_Error("subtract joiner not implemented");
		break;
	}
	M_symbol_joiners.push_back(symbol_joiner_ptr);
	M_symbols.push_back(irk_symbol.get_copy());
}

j_size_t j_composite_symbol::size()const{ return M_symbols.size(); }
j_composite_symbol::Join_Type get_composite_symbol_join_type(J_UI_Char i_char){
	j_composite_symbol::Join_Type join_type = j_composite_symbol::Join_Type::ERROR_TYPE;
	switch(i_char.charcode()){
	case '+': join_type = j_composite_symbol::Join_Type::ADD; break;

	case '/': join_type = j_composite_symbol::Join_Type::DIVIDE; break;

	case '*': join_type = j_composite_symbol::Join_Type::MULTIPLY; break;
	case '-': join_type = j_composite_symbol::Join_Type::SUBTRACT; break;
	default:
		;
	}
	return join_type;
}
/*void clear()*/
void j_composite_symbol::clear(){
	j_size_t num_elems = size();
	for(int i = 0; i < num_elems; i++){
		delete M_symbol_joiners[i];
		delete M_symbols[i];
	}
	M_symbol_joiners.clear();
	M_symbols.clear();

}



j_composite_symbol::~j_composite_symbol(){ clear(); }

j_symbol& j_composite_symbol::operator[](j_size_t i_index){
	assert(between_inclusive(i_index, J_SIZE_T_ZERO, M_symbols.size()));
	return *M_symbols[i_index];
}

const j_symbol& j_composite_symbol::operator[](j_size_t i_index)const{
	assert(between_inclusive(i_index, J_SIZE_T_ZERO, M_symbols.size()));
	return *M_symbols[i_index];
}

j_composite_symbol* j_composite_symbol::move_copy(){
	return  new j_composite_symbol(std::move(*this));
}



}




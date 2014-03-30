#include "j_routine_symbol.h"
//
#include "J_Symbol_Error.h"
using std::string;

using std::map; using std::function;
using std::bind; using std::for_each;
namespace jomike{



void j_routine_wrapper_symbol::clear(){}

bool j_routine_wrapper_symbol::has_value()const {
	return true;
}

j_routine_wrapper_symbol::j_routine_wrapper_symbol(
	const string& irk_name, Symbol_Types i_symbol_type):j_symbol(irk_name, i_symbol_type){

}

bool j_routine_symbol
	::symbol_name_availability_status(const J_UI_String& irk_symbol_name){
	return !M_scope_symbols.count(irk_symbol_name);
}

j_routine_symbol* j_routine_symbol::get_copy()const {
	return new j_routine_symbol(*this);
}

void j_routine_symbol::clear(){
	for(auto f_symbol_pair : M_scope_symbols){
		f_symbol_pair.second->clear();
	}
}

bool j_routine_symbol::has_value()const {
	return all_of(M_scope_symbols.begin()
		, M_scope_symbols.end()
		, [](const Symbol_Cont_t::value_type& irk_pair){
		return irk_pair.second->has_value();
	});
}



j_value j_routine_symbol::derived_get_value(const Arguments& )const {
	map<J_UI_String, j_symbol*> M_function_scope_symbol;

	assert(!"The method or operation is not yet check.");
	return j_value(0.0, J_Unit());
}

void j_routine_symbol::add_scope_symbol(const j_symbol& irk_symbol){
	if(M_scope_symbols.count(irk_symbol.name())){
		throw J_Syntax_Error("Routine Symbol Already Has Scope Symbol with this name");
	}

	M_scope_symbols[irk_symbol.name()] = irk_symbol.get_copy();
}


void j_routine_symbol::add_arg_symbol(const j_symbol& irk_symbol){
	if(M_arg_symbols.count(irk_symbol.name())){
		throw J_Symbol_Error("Routine Symbol Already Has Argument with this name");
	}

	M_arg_symbols[irk_symbol.name()] = irk_symbol.get_copy();
}


void j_routine_symbol::add_interface_symbol(const j_symbol& irk_symbol){
	if(M_interface_symbols.count(irk_symbol.name())){
		throw J_Syntax_Error("Routine Symbol Already Has Interface Symbol with this name");
	}

	M_interface_symbols[irk_symbol.name()] = irk_symbol.get_copy();
}

void j_routine_symbol::set_return_symbol(const j_symbol& irk_symbol){
	delete M_return_symbol;
	M_return_symbol = irk_symbol.get_copy();
}

void j_routine_symbol::add_statement(const J_UI_String& irk_string){
	M_statements.push_back(irk_string);
}

j_routine_symbol::~j_routine_symbol(){
	auto map_deleter = [](Symbol_Cont_t& yr_cont){
		for_each(make_pair_iter(yr_cont.begin()), make_pair_iter(yr_cont.end())
			, [](j_symbol* y_symbol){delete y_symbol; });
	};
	map_deleter(M_arg_symbols);
	map_deleter(M_interface_symbols);
	map_deleter(M_scope_symbols);
	delete M_return_symbol;
}

j_routine_symbol::j_routine_symbol(const j_routine_symbol& irk_src): j_symbol(irk_src){
	auto map_copier = [](const Symbol_Cont_t& y_src, Symbol_Cont_t& y_dest){
		for(auto f_pair : y_src){
			f_pair.second = f_pair.second->get_copy();
			y_dest.insert(f_pair);
		}
	};

	map_copier(irk_src.M_arg_symbols, M_arg_symbols);
	map_copier(irk_src.M_scope_symbols, M_scope_symbols);
	map_copier(irk_src.M_interface_symbols, M_interface_symbols);
	M_return_symbol = irk_src.M_return_symbol ? irk_src.M_return_symbol->get_copy()
		: nullptr;
}

j_routine_symbol::j_routine_symbol(j_routine_symbol&& irr_src): j_symbol(irr_src){
	M_arg_symbols.swap(irr_src.M_arg_symbols);
	M_scope_symbols.swap(irr_src.M_scope_symbols);
	M_interface_symbols.swap(irr_src.M_interface_symbols);
	assert(!M_return_symbol);
	std::swap(M_return_symbol, irr_src.M_return_symbol);
}

j_routine_symbol::j_routine_symbol(
	const string& irk_name, Symbol_Types i_symbol_type):j_symbol(irk_name, i_symbol_type){

}

j_routine_symbol::j_routine_symbol() : j_symbol(Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED){

}



j_routine_symbol* j_routine_symbol::move_copy(){
	return  new j_routine_symbol(std::move(*this));
}

}




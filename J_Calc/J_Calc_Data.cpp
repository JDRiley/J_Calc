#include "J_Calc_Data.h"
//
#include "J_Calc_View.h"
//
#include <j_function.h>
//
#include "Math_Input_Box.h"
//
#include <J_Math.h>
//Utilities
#include <cassert>
//
#include <j_routine_symbol.h>
//
#include <Binary_Function_Chain_Symbol.h>
//
#include <Unary_Function_Symbol.h>
//
#include <J_Symbol_Identifier.h>
//
#include <Arguments.h>
using std::dynamic_pointer_cast;

using std::string; using std::accumulate;

namespace jomike{

void add_user_symbol(j_symbol* i_symbol){
	static Instance_Pointer<J_Calc_Data> s_data;
	return s_data->add_user_symbol(i_symbol);
}

j_symbol* get_j_symbol_from_model(const J_UI_String& irk_name){
	static Instance_Pointer<J_Calc_Data> s_data;
	return s_data->get_symbol(irk_name);
}



static void init_reserve_keywords(j_tree<J_UI_String>*);


J_Calc_Data::J_Calc_Data(){
	init_reserve_keywords(&M_reserved_words);
	init_reserved_symbols();
}

void init_reserve_keywords(j_tree<J_UI_String>* ir_dest_set){
	ir_dest_set->clear();
	auto add_keyword = [ir_dest_set](const char* y_name){
		ir_dest_set->insert(y_name);
	};

	ir_dest_set->insert("Dbl");
	ir_dest_set->insert("dbl");
	ir_dest_set->insert("int");
	ir_dest_set->insert("Alias");
	ir_dest_set->insert("J_Calc");
	ir_dest_set->insert("Int");
	ir_dest_set->insert("Routine");
	ir_dest_set->insert("gcd");
	ir_dest_set->insert("lcm");
	add_keyword("is_print");
	add_keyword("does_divide");
	add_keyword("parse_int");

	ir_dest_set->insert("least_common_multiple");
}

J_Calc_Data& J_Calc_Data::get_instance(){
	static J_Calc_Data j_calc_data;
	return j_calc_data;
}

void J_Calc_Data::attach_view(J_Calc_View_Shared_t i_new_view){
	bool inserted = M_calc_views.insert(i_new_view).second;

	if(!inserted){
		throw J_Argument_Error("View Already A Member of J_Calc_Data");
	}
}


/*void add_user_symbol(J_Symbol_Shared_t)*/
void J_Calc_Data::add_user_symbol(j_symbol* i_symbol_ptr){
	if(M_user_symbols.count(i_symbol_ptr->name())){
		delete i_symbol_ptr;
		throw J_Syntax_Error("Symbol With This Name Already Exists");
	}
	M_user_symbols[i_symbol_ptr->name()] = i_symbol_ptr;

}



static Instance_Pointer<J_Calc_Data> s_calc_data;
bool is_reserved_symbol(const J_UI_String& irk_src){
	
	return s_calc_data->is_reserved_symbol(irk_src);
}

j_symbol* get_reserved_symbol(const J_UI_String& irk_string){
	
	return s_calc_data->get_reserved_symbol(irk_string);
}

j_symbol* J_Calc_Data::get_reserved_symbol(const J_UI_String& irk_string)const{
	assert(is_reserved_symbol(irk_string));
	return *M_reserved_symbols.find(irk_string);
}

bool J_Calc_Data::is_reserved_symbol(const J_UI_String& irk_src)const{return M_reserved_symbols.count(irk_src);}
bool J_Calc_Data::is_reserved_word(const J_UI_String& irk_src)const{return M_reserved_words.count(irk_src);}

j_symbol* get_symbol(const J_UI_String& irk_string){
	return s_calc_data->get_symbol(irk_string);
}

j_symbol* J_Calc_Data::get_symbol(const J_UI_String& irk_string)const{
	if(is_reserved_symbol(irk_string)){
		return get_reserved_symbol(irk_string);
	}else if(is_user_symbol(irk_string)){
		return get_user_symbol(irk_string);
	}else{
		throw J_Syntax_Error("Symbol: " + irk_string.std_str() + " is undefined!");
	}
}

bool J_Calc_Data::is_user_symbol(const J_UI_String& irk_string)const{return M_user_symbols.count(irk_string);}

j_symbol* J_Calc_Data::get_user_symbol(const J_UI_String& irk_string)const{
	if(!is_user_symbol(irk_string)){
		throw J_Argument_Error("No User Symbol with that name");
	}

	return *M_user_symbols.find(irk_string);
}

void J_Calc_Data::remove_user_symbol(const J_UI_String& irk_string){
	if(!is_user_symbol(irk_string)){
		throw J_Argument_Error("No User Symbol with that name to remove");
	}

	auto symbol_pos = M_user_symbols.find(irk_string);
	assert(M_user_symbols.end() != symbol_pos);

	M_user_symbols.erase(symbol_pos);
}

/*bool symbol_name_availability_status(const J_UI_String&)*/
bool J_Calc_Data::symbol_name_availability_status(const J_UI_String& irk_string){
	if(is_reserved_word(irk_string)){
		return false;
	}

	if(is_user_symbol(irk_string)){
		return false;
	}

	return true;
}

J_Calc_Data::~J_Calc_Data(){
	clear_user_data();
	clear_reserved();
}

void J_Calc_Data::clear_reserved(){
	for(auto f_reserved_symbol : M_reserved_symbols){
		delete f_reserved_symbol;
	}
}

void J_Calc_Data::clear_user_data(){
	M_calc_views.clear();

	for(auto f_user_symbol : M_user_symbols){
		delete f_user_symbol;
	}

	M_user_symbols.clear();
}

void J_Calc_Data::remove_view(J_Calc_View_Shared_t i_view){
	M_calc_views.erase(i_view);
}

void J_Calc_Data::init_reserved_symbols(){
	M_reserved_symbols["gcd"] = new LLint_Binary_Function_Symbol(greatest_common_divisor_simp
		, "gcd");
	M_reserved_symbols["lcm"] 
		= new LLint_Binary_Function_Symbol(least_common_multiple
		, "least_common_multiple");

	M_reserved_symbols["is_prime"]
		= new Unary_Function_Symbol<bool, j_llint>(is_prime, "is_prime");


	using std::stoll; 
	M_reserved_symbols["parse_int"]
		= new Unary_Function_Symbol<j_llint, const string&>(
		bind(static_cast<j_llint(*)(const string&, size_t*, int)>(stoll), _1, nullptr, 10), "parse_int");


}

void J_Calc_Data::update(){

}

}
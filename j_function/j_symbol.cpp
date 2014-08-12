#include "j_symbol.h"
//
#include "Type_Syntax.h"
//
#include "J_Symbol_Scope.h"
//
#include "J_Symbol_Error.h"
//
#include "J_Symbol_Identifier.h"
//
#include <J_UI/J_UI_String.h>
//
#include "j_placeholder_symbol.h"
//
#include "Arguments.h"
//
#include <j_tree.h>
namespace jomike{
static int s_symbol_ids = 0;

class J_Default_Symbol_Scope : public J_Symbol_Scope{


	j_symbol* get_symbol(const J_UI_String& irk_string)const override{
		return get_j_symbol(irk_string);
	}

	void add_symbol(j_symbol* i_symbol)override{
		add_symbol(i_symbol);
	}

};


extern const J_Symbol_Scope* gk_default_symbol_scope = new J_Default_Symbol_Scope();

void j_symbol::set_symbol_scope(const J_Symbol_Scope* i_symbol_scope){
	M_symbol_scope = i_symbol_scope;
}

j_symbol* j_symbol::get_symbol_from_scope(const J_UI_String& irk_string)const{
	assert(M_symbol_scope);
	return M_symbol_scope->get_symbol(irk_string);
}


J_Sym_Argument_Error::J_Sym_Argument_Error(const std::string& ik_message):J_Error(ik_message){}


//Destructor
j_symbol::~j_symbol(){
	delete M_type;
	delete M_name;
	delete M_arguments;
}





j_dbl j_symbol::value(const Arguments& i_args)const{
	return get_value(i_args).as_double();
}

j_symbol* j_symbol::reduce()const{
	return get_copy();
}

j_symbol::j_symbol(J_Symbol_Identifier* irk_name, Symbol_Types i_symbol_type)
	:j_symbol_component(i_symbol_type){
	M_name = irk_name;
	M_arguments = empty_arguments().get_copy();
	M_type = nullptr;
	assert(M_name);
}

j_symbol::j_symbol(Symbol_Types i_symbol_type) : j_symbol_component(i_symbol_type){
	M_name = new J_Symbol_Identifier("%Unnamed");
	M_type = nullptr;
	M_arguments = empty_arguments().get_copy();
}

j_symbol::j_symbol(const j_symbol& irk_symbol):j_symbol_component(irk_symbol){
	if(irk_symbol.M_type){
		M_type = irk_symbol.M_type->get_copy();
	}

	M_name = irk_symbol.M_name->get_copy();

	M_arguments = irk_symbol.M_arguments->get_copy();
}

j_symbol::j_symbol(j_symbol&& irr_symbol)
:j_symbol_component(std::move(irr_symbol)){
	if(irr_symbol.M_type){
		M_type = irr_symbol.M_type->move_copy();
	}

	M_name = irr_symbol.M_name->move_copy();

	M_arguments = irr_symbol.M_arguments->move_copy();
}


Type_Syntax& j_symbol::type_syntax()const{
	assert(M_type);
	return *M_type;
}

void j_symbol::set_type_syntax(const Type_Syntax& irk_type_syntax){
	delete M_type;
	M_type = irk_type_syntax.get_copy();
}

void j_symbol::set_type_syntax(Type_Syntax* i_type_syntax){
	M_type = i_type_syntax;
}



const J_UI_String& j_symbol::name()const{
	return M_name->identifier_name();
}

void j_symbol::set_name(const J_UI_String& irk_string){
	M_name->set_name(irk_string);
}


j_expression* j_symbol::as_expression(){
	throw J_Symbol_Error(name().std_str() + " cannot be used as an expression!");
}

void j_symbol::set_value(j_value /*i_value*/){
	assert(!"Cannot Set Value of This type Symbol");
}

J_UI_String j_symbol::get_display_name(){
	return name();
}




/*void set_args(int index, Arguments)*/
void j_symbol::set_args(const Arguments& i_args){
	delete M_arguments;
	M_arguments= i_args.get_copy();
}

/*void set_args(int index, Arguments)*/
void j_symbol::set_args(Arguments&& i_args){
	M_arguments->swap(i_args);
}

j_value j_symbol::get_value(const Arguments& i_args)const{
	Arguments args(*M_arguments);
	j_tree<j_size_t> removed_indices;
	for(int i = 0; i < args.size(); i++){
		if(args[i].is_placeholder()){
			j_placeholder_symbol* place_holder_symbol
				= dynamic_cast<j_placeholder_symbol*>(&args[i]);

			assert(place_holder_symbol);
			j_size_t index = place_holder_symbol->placeholder_index();
			removed_indices.insert(index);
			if(index >= i_args.size()){
				throw J_Sym_Argument_Error("Not Enough Arguments to Symbol: " + name().std_str());
			}
			args.set_argument(index, i_args.arguments()[index]);
		}
	}

	for(int i = 0; i < i_args.size(); i++){
		if(!removed_indices.count(i)){
			args.push_back(i_args[i]);
		}
	}

	return derived_get_value(args);
}

bool j_symbol::is_placeholder()const{ return false; }

const Arguments& empty_arguments(){
	static Arguments empty_args;
	return empty_args;
}


}




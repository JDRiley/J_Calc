#include "j_calc_symbol.h"
//
#include "Type_Syntax.h"
//
#include "J_Symbol_Scope.h"
//
#include "JC_Symbol_Error.h"
//
#include <j_symbol/J_Symbol_Identifier.h>

//
#include "j_placeholder_symbol.h"
//
#include "Arguments.h"
//
#include "Type_Factory.h"
//
#include <j_tree.h>
namespace jomike{



j_calc_symbol* get_j_symbol_from_model(const j_string_t& irk_name);

class J_Default_Symbol_Scope : public J_Symbol_Scope{
public:
	j_calc_symbol* get_symbol(const jc_string_t& irk_string)const override{
		return get_j_symbol_from_model(irk_string);
	}

	void add_symbol(j_calc_symbol* i_symbol)override{
		add_user_symbol(i_symbol);
	}

	J_Default_Symbol_Scope* get_copy()const override{
		return new J_Default_Symbol_Scope(*this);
	}

	J_Default_Symbol_Scope* move_copy()override{
		return  new J_Default_Symbol_Scope(std::move(*this));
	}

};

static J_Symbol_Scope* default_symbol_scope(){
	static J_Default_Symbol_Scope default_scope;
	return &default_scope;
}

extern J_Symbol_Scope* gk_default_symbol_scope = default_symbol_scope();

void j_calc_symbol::set_symbol_scope(J_Symbol_Scope* i_symbol_scope){
	M_symbol_scope = i_symbol_scope;
	alert_symbol_scope_set();
}

j_calc_symbol* j_calc_symbol::get_symbol_from_scope(const jc_string_t& irk_string)const{
	assert(M_symbol_scope);
	return M_symbol_scope->get_symbol(irk_string);
}


JC_Sym_Argument_Error::JC_Sym_Argument_Error(const jc_string_t& ik_message):J_Error(ik_message){}


//Destructor
j_calc_symbol::~j_calc_symbol(){
	delete M_type;
	delete M_name;
	delete M_arguments;
}





j_dbl j_calc_symbol::value(const Arguments& i_args)const{
	return get_value(i_args).as_double();
}

j_calc_symbol* j_calc_symbol::reduce()const{
	return get_copy();
}



j_calc_symbol::j_calc_symbol(const yy::location& irk_loc
	, J_Symbol_Identifier<jc_string_t>* irk_name, Symbol_Types i_symbol_type)
	:j_calc_symbol_component(irk_loc, i_symbol_type){
	M_name = irk_name;
	M_arguments = empty_arguments().get_copy();
	M_type = make_type_syntax(irk_loc, i_symbol_type);
	assert(M_name);
	
}

j_calc_symbol::j_calc_symbol(
	const yy::location& irk_loc, Type_Syntax* i_type_syntax
	, J_Symbol_Identifier<jc_string_t>* irk_name)
:j_calc_symbol_component(irk_loc, i_type_syntax->symbol_type())
	, M_type(i_type_syntax), M_name(irk_name){
	M_arguments = empty_arguments().get_copy();
}



j_calc_symbol::j_calc_symbol(const yy::location& irk_loc, Symbol_Types i_symbol_type)
	: j_calc_symbol_component(irk_loc, i_symbol_type){
	M_name = new J_Symbol_Identifier<jc_string_t>(L"%Unvalued");
	M_type = make_type_syntax(irk_loc, i_symbol_type);
	M_arguments = empty_arguments().get_copy();
}

j_calc_symbol::j_calc_symbol(const j_calc_symbol& irk_symbol)
	:j_calc_symbol_component(irk_symbol){
	if(irk_symbol.M_type){
		M_type = irk_symbol.M_type->get_copy();
	}

	M_name = irk_symbol.M_name->get_copy();

	M_arguments = irk_symbol.M_arguments->get_copy();
	M_symbol_scope = irk_symbol.M_symbol_scope;
}



j_calc_symbol::j_calc_symbol(j_calc_symbol&& irr_symbol)
: j_calc_symbol_component(std::move(irr_symbol)){
	if(irr_symbol.M_type){
		M_type = irr_symbol.M_type->move_copy();
	}

	M_name = irr_symbol.M_name->move_copy();

	M_arguments = irr_symbol.M_arguments->move_copy();
	M_symbol_scope = irr_symbol.M_symbol_scope;
}


Type_Syntax& j_calc_symbol::type_syntax()const{
	assert(M_type);
	return *M_type;
}

void j_calc_symbol::set_type_syntax(const Type_Syntax& irk_type_syntax){
	delete M_type;
	M_type = irk_type_syntax.get_copy();
}

void j_calc_symbol::set_type_syntax(Type_Syntax* i_type_syntax){
	delete M_type;
	M_type = i_type_syntax;
}



const jc_string_t& j_calc_symbol::name()const{
	return M_name->identifier_name();
}

void j_calc_symbol::set_name(const jc_string_t& irk_string){
	M_name->set_name(irk_string);
}


j_expression* j_calc_symbol::as_expression(){
	throw JC_Symbol_Error(name() + L" cannot be used as an expression!");
}

void j_calc_symbol::set_value(j_value /*i_value*/){
	assert(!"Cannot Set Value of This type Symbol");
}

jc_string_t j_calc_symbol::get_display_name(){
	return name();
}




/*void set_args(int index, Arguments)*/
void j_calc_symbol::set_args(const Arguments& i_args){
	delete M_arguments;
	M_arguments= i_args.get_copy();
}

/*void set_args(int index, Arguments)*/
void j_calc_symbol::set_args(Arguments&& i_args){
	M_arguments->swap(i_args);
}

j_value j_calc_symbol::get_value(const Arguments& i_args)const{
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
				throw JC_Sym_Argument_Error(L"Not Enough Arguments to Symbol: " + name());
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

bool j_calc_symbol::is_placeholder()const{ return false; }

j_calc_symbol* j_calc_symbol::make_non_referenced()const{
	return get_copy();
}

j_calc_symbol* j_calc_symbol::convert_to_type(const Type_Syntax& /*irk_type*/)const{
	throw JC_Symbol_Error(L"Cannot Convert type of symbol: " + name()
						 + L" with type: " + type_syntax().type_name());
}

const Type_Syntax& j_calc_symbol::return_type_syntax()const{
	assert(M_type);
	return *M_type;
}



J_Symbol_Scope& j_calc_symbol::symbol_scope()const{
	assert(M_symbol_scope);
	return *M_symbol_scope;
}


void j_calc_symbol::process(){
	get_value();
}

bool j_calc_symbol::has_type_syntax()const{
	return M_type;
}

const Arguments& empty_arguments(){
	static yy::location loc;
	static Arguments empty_args(loc);

	return empty_args;
}


}




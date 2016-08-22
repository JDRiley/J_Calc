#include "Custom_Routine_Symbol.h"
//
#include <j_symbol/Specific_Symbol_List.h>
//
#include "J_Symbol_Scope.h"
//
#include "Type_Routine.h"
//
#include "j_declaration.h"
//
#include "Arguments.h"
//
#include <algorithm>
//
#include "JC_Symbol_Error.h"
//
#include "Statement_Block.h"
//
#include "Value_Expression.h"
//

using std::transform;

using std::to_wstring;
namespace jomike{


static Type_Syntax_List* make_type_syntax_list(const Declaration_List& irk_list){
	auto syntax_list = new Type_Syntax_List(irk_list.location());
	for(auto declaration : irk_list){
		syntax_list->add_symbol(declaration->type_syntax());
	}
	return syntax_list;
}


Custom_Routine_Symbol::Custom_Routine_Symbol(const yy::location& irk_loc
	, J_Symbol_Identifier<jc_string_t>* i_identifier, const Declaration_List& irk_static_declarations
	, const Declaration_List& irk_arg_declarations, Type_Syntax* i_return_type
	, Statement_Block* i_statement_list)
:j_routine_symbol(irk_loc
	, new Type_Routine(yy::location(), i_return_type, make_type_syntax_list(irk_arg_declarations))
	, i_identifier){
	M_statement_block = i_statement_list;
	M_running_scope = new J_Symbol_Scope(irk_static_declarations);

	M_running_scope->set_parent_scope(&symbol_scope());
	transform(irk_arg_declarations.begin(), irk_arg_declarations.end()
			 , back_inserter(M_arg_names), [](const j_declaration* y_decl){return y_decl->name(); });

	type_syntax().set_location(
		irk_static_declarations.location() + irk_arg_declarations.location());
}

Custom_Routine_Symbol::Custom_Routine_Symbol(const Custom_Routine_Symbol& irk_right)
:j_routine_symbol(irk_right), M_arg_names(irk_right.M_arg_names)
, M_statement_block(irk_right.M_statement_block->get_copy())
, M_running_scope(irk_right.M_running_scope->get_copy()){

}

Custom_Routine_Symbol::Custom_Routine_Symbol(Custom_Routine_Symbol&& irv_right)
:j_routine_symbol(std::move(irv_right)), M_arg_names(std::move(irv_right.M_arg_names))
, M_statement_block(irv_right.M_statement_block->move_copy())
, M_running_scope(irv_right.M_running_scope->move_copy()){

}

Custom_Routine_Symbol::~Custom_Routine_Symbol(){
	delete M_statement_block;
	delete M_running_scope;
}

Custom_Routine_Symbol* Custom_Routine_Symbol::move_copy(){
	return  new Custom_Routine_Symbol(std::move(*this));
}

Custom_Routine_Symbol* Custom_Routine_Symbol::get_copy()const{
	return new Custom_Routine_Symbol(*this);
}

j_value Custom_Routine_Symbol::derived_get_value(const Arguments& irk_args)const {
	if(irk_args.size() != M_arg_names.size()){
		throw JC_Symbol_Error(
			L"Improper number of Args to function: " + name()
			+ L" expected: " + to_wstring(M_arg_names.size()) + L" args. Given: " + to_wstring(irk_args.size()));
	}

	J_Symbol_Scope_Unique_t running_scope(new J_Symbol_Scope(M_running_scope));

	for(int i = 0; i < irk_args.size(); ++i){
		//j_symbol_unique_t arg_symbol(irk_args[i].make_non_referenced());
		//arg_symbol->set_name(M_arg_names[i]);
		//if(arg_symbol->return_type_syntax() != argument_types_list()[i]){
		//arg_symbol = j_symbol_unique_t(arg_symbol->convert_to_type(argument_types_list()[i]));
		//}

		j_calc_symbol_unique_t arg_symbol(new Value_Expression(
			irk_args[i].location(), irk_args[i].get_value()));

		arg_symbol->set_name(M_arg_names[i]);
		running_scope->add_symbol(arg_symbol.release());

	}


	M_statement_block->set_symbol_scope(running_scope.get());

	j_value return_val;

	try{
		M_statement_block->get_value();
	} catch(J_Routine_Transfer_Exception& e){
		return_val = e.value();
	}

	if(return_type_syntax().is_void()){
		return j_value::void_type();
	}

	if(j_value::Value_Types::UNDEFINIED == return_val.type()){
		throw JC_Symbol_Error(L"Function Did Not Return A Value");
	}
	return return_type_syntax().convert_value(return_val);

}

void Custom_Routine_Symbol::process(){
	symbol_scope().add_symbol(get_copy());
}

void Custom_Routine_Symbol::alert_symbol_scope_set(){

}


const j_value& J_Routine_Transfer_Exception::value()const{
	return M_value;
}

J_Routine_Transfer_Exception::J_Routine_Transfer_Exception(j_value i_value): M_value(i_value){

}

}




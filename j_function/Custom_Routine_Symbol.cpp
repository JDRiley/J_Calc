#include "Custom_Routine_Symbol.h"
//
#include "Specific_Symbol_List.h"
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
#include <J_Symbol_Error.h>
using std::transform;

using std::to_string;
namespace jomike{


static Type_Syntax_List* make_type_syntax_list(const Declaration_List& irk_list){
	auto syntax_list = new Type_Syntax_List;
	for(auto declaration : irk_list){
		syntax_list->add_symbol(declaration->get_copy()->type_syntax());
	}
	return syntax_list;
}


Custom_Routine_Symbol::Custom_Routine_Symbol(
	J_Symbol_Identifier* i_identifier, const Declaration_List& irk_static_declarations
	, const Declaration_List& irk_arg_declarations, Type_Syntax* i_return_type
	, Symbol_List* i_statement_list)
:j_routine_symbol(i_identifier, new Type_Routine(i_return_type, make_type_syntax_list(irk_arg_declarations))){
	M_statement_list = i_statement_list;
	M_running_scope = new J_Symbol_Scope(irk_static_declarations);



	transform(irk_arg_declarations.begin(), irk_arg_declarations.end()
			 , back_inserter(M_arg_names), [](const j_declaration* y_decl){return y_decl->name(); });
}

Custom_Routine_Symbol::Custom_Routine_Symbol(const Custom_Routine_Symbol& irk_right):j_routine_symbol(irk_right), M_arg_names(irk_right.M_arg_names)
, M_statement_list(irk_right.M_statement_list->get_copy())
, M_running_scope(irk_right.M_running_scope->get_copy()){

}

Custom_Routine_Symbol::Custom_Routine_Symbol(Custom_Routine_Symbol&& irv_right)
:j_routine_symbol(std::move(irv_right)), M_arg_names(std::move(irv_right.M_arg_names))
, M_statement_list(irv_right.M_statement_list->move_copy())
, M_running_scope(irv_right.M_running_scope->move_copy()){

}

Custom_Routine_Symbol::~Custom_Routine_Symbol(){
	delete M_statement_list;
	delete M_running_scope;
}

Custom_Routine_Symbol* Custom_Routine_Symbol::move_copy(){
	return  new Custom_Routine_Symbol(std::move(*this));
}

Custom_Routine_Symbol* Custom_Routine_Symbol::get_copy()const {
	return new Custom_Routine_Symbol(*this);
}

j_value Custom_Routine_Symbol::derived_get_value(const Arguments& irk_args)const {
	if(irk_args.size() != M_arg_names.size()){
		throw J_Symbol_Error(
			"Improper number of Args to function: " + name().std_str()
			+ " expected: " + to_string(M_arg_names.size()) + " args. Given: " + to_string(irk_args.size()));
	}

	J_Symbol_Scope_Unique_t running_scope(new J_Symbol_Scope(M_running_scope));

	for(int i = 0; i < irk_args.size(); ++i){
		j_symbol_unique_t arg_symbol(irk_args[i].make_non_referenced());
		arg_symbol->set_name(M_arg_names[i]);
		if(arg_symbol->type_syntax() != argument_types_list()[i]){

			arg_symbol = j_symbol_unique_t(arg_symbol->convert_to_type(argument_types_list()[i]));

		}

		running_scope->add_symbol(arg_symbol.release());

	}


	M_statement_list->set_symbol_scope(running_scope.get());
	return M_statement_list->get_value();

}

}



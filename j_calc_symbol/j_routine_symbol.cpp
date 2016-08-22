#include "j_routine_symbol.h"
//
#include "JC_Symbol_Error.h"
//
#include "Type_Routine.h"


using std::function;
using std::bind; using std::for_each;
namespace jomike{

j_routine_symbol::j_routine_symbol(const yy::location& irk_loc
	, Type_Routine* i_routine_type, J_Symbol_Identifier<jc_string_t>* i_identifier)
	: j_declaration(irk_loc, i_routine_type, i_identifier){

}

const Type_Syntax_List& j_routine_symbol::argument_types_list()const{
	
	return routine_type_syntax().argument_types_list();
}

bool j_routine_symbol::has_value()const {
	return false;
}

void j_routine_symbol::clear(){

}

const Type_Syntax& j_routine_symbol::return_type_syntax()const {
	return routine_type_syntax().return_type();
}

const Type_Routine& j_routine_symbol::routine_type_syntax()const{
	auto routine_type
		= dynamic_cast<Type_Routine*>(&type_syntax());
	assert(routine_type);
	return *routine_type;
}

jc_string_t j_routine_symbol::get_display_name(){
	jc_string_t return_string =  L"Routine: " + name() + routine_type_syntax().arg_type_string()
	+ L"->" + return_type_syntax().type_name();
	
	

	
	return return_string;
}




}




#include "j_routine_symbol.h"
//
#include "J_Symbol_Error.h"
//
#include "Type_Routine.h"
using std::string;

using std::function;
using std::bind; using std::for_each;
namespace jomike{

j_routine_symbol::j_routine_symbol(
	J_Symbol_Identifier* i_identifier, Type_Routine* i_routine_type) 
	: j_declaration(i_routine_type, i_identifier){

}

const Type_Syntax_List& j_routine_symbol::argument_types_list()const{
	auto routine_type
		= dynamic_cast<Type_Routine*>(&type_syntax());
	assert(routine_type);

	return routine_type->argument_types_list();
}




}




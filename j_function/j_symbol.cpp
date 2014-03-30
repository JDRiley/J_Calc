#include "j_symbol.h"
//
#include "Type_Syntax.h"

//
#include "J_Symbol_Error.h"
namespace jomike{
static int s_symbol_ids = 0;

J_Sym_Argument_Error::J_Sym_Argument_Error(const char* const ik_message):J_Error(ik_message){}

//Constructors


//Destructor
j_symbol::~j_symbol(){}





j_dbl j_symbol::value(const Arguments& i_args)const{
	return get_value(i_args).as_double();
}

j_symbol* j_symbol::reduce()const{
	return get_copy();
}

j_symbol::j_symbol(const J_UI_String& irk_name, Symbol_Types i_symbol_type)
	:j_symbol_component(i_symbol_type, irk_name){}

j_symbol::j_symbol(Symbol_Types i_symbol_type) : j_symbol_component(i_symbol_type){

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

j_expression* j_symbol::as_expression(){
	throw J_Error(name().std_str() + " cannot be used as an expression!");
}

void j_symbol::set_value(j_value /*i_value*/){
	assert(!"Cannot Set Value of This type Symbol");
}





}




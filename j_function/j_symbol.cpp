#include "j_symbol.h"



namespace jomike{
static int s_symbol_ids = 0;

J_Sym_Argument_Error::J_Sym_Argument_Error(const char* const ik_message):J_Error(ik_message){}

//Constructors


//Destructor
j_symbol::~j_symbol(){}



/*const J_UI_String& display_name()const*/
J_UI_String j_symbol::get_display_name(){ return name(); }

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


}




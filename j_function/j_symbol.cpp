#include "j_symbol.h"


namespace jomike{
static int s_symbol_ids = 0;

J_Sym_Argument_Error::J_Sym_Argument_Error(const char* const ik_message):J_Error(ik_message){}

//Constructors
j_symbol::j_symbol() : M_ID(++s_symbol_ids){}

j_symbol::j_symbol(const J_UI_String& irk_string) : M_name(irk_string), M_ID(s_symbol_ids++){}


//Destructor
j_symbol::~j_symbol(){}

/*const J_UI_String& name()const*/
const J_UI_String& j_symbol::name()const{ return M_name; }

/*void set_name(const J_UI_String& )*/
void j_symbol::set_name(const J_UI_String& irk_string){ M_name = irk_string; }

/*const J_UI_String& display_name()const*/
J_UI_String j_symbol::get_display_name(){ return M_name; }

j_dbl j_symbol::value(const Arguments& i_args)const{
	return get_value(i_args).value();
}

j_symbol* j_symbol::reduce()const{
	return get_copy();
}


}




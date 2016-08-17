#include "j_number_symbol.h"
//
#include "Type_Factory.h"
//
#include "J_Symbol_Error.h"
//
#include <j_symbol/J_Symbol_Identifier.h>
//
#include "Arguments.h"
using std::to_string;
using std::string;

namespace jomike{
//j_number_symbol****************************************

//Constructors
j_number_symbol::j_number_symbol(const yy::location& irk_loc, Dbl_t i_val)
: Variable_Symbol(irk_loc, make_double_type_syntax(irk_loc), new J_Symbol_Identifier("#unnamed"))
,M_value_status(true)
	, M_value(j_value(i_val, J_Unit())){
}

j_number_symbol::j_number_symbol(const yy::location& irk_loc, const std::string& irk_string)
	: Variable_Symbol(irk_loc, make_double_type_syntax(irk_loc), new J_Symbol_Identifier(irk_string))
	, M_value_status(false){}

j_number_symbol* j_number_symbol::get_copy()const{ return new j_number_symbol(*this); }

/*bool has_value()const*/
bool j_number_symbol::has_value()const{ return M_value_status; }

j_value j_number_symbol::derived_get_value(const Arguments& irk_args)const{
	if(!M_value_status){
		throw J_Value_Error("Number Symbol Has Invalid Value");
	}

	if(irk_args.size()){
		throw J_Sym_Argument_Error("Too many arguments passed in");
	}
	return j_value(M_value, J_Unit());
}

/*const J_UI_String& get_display_name()*/
std::string j_number_symbol::get_display_name(){
	string display_name(name() + "->");

	if(has_value()){
		display_name.push_back(' ');
		return display_name + M_value.to_str();
	}
	return name();
}

/*void clear()*/
void j_number_symbol::clear(){ M_value_status = false; M_value = j_value();}

j_number_symbol* j_number_symbol::move_copy(){
	return  new j_number_symbol(std::move(*this));
}


}




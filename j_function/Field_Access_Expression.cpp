#include "Field_Access_Expression.h"
//
#include "J_Symbol_Identifier.h"

namespace jomike{

J_FWD_DECL(J_Symbol_Identifier)

Field_Access_Expression::Field_Access_Expression(J_Symbol_Identifier* i_name)
:j_expression(Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED){

	assert(i_name);
	M_identifier = i_name;
}

Field_Access_Expression::Field_Access_Expression(const Field_Access_Expression& irk_source) : j_expression(irk_source){
	M_identifier = irk_source.M_identifier->get_copy();
}

j_value Field_Access_Expression::derived_get_value(const Arguments& i_args)const {
	return get_j_symbol(M_identifier->identifier_name())
		->get_value(i_args);
}

bool Field_Access_Expression::has_value()const{
	try{
		return get_symbol_from_scope(M_identifier->identifier_name())->has_value();
	} catch(...){
		return false;
	}
}

Field_Access_Expression* Field_Access_Expression::move_copy(){
	return  new Field_Access_Expression(std::move(*this));
}

Field_Access_Expression* Field_Access_Expression::get_copy()const {
	return new Field_Access_Expression(*this);
}

void Field_Access_Expression::set_value(j_value i_value){
	get_symbol_from_scope(M_identifier->identifier_name())->set_value(i_value);
}

J_UI_String Field_Access_Expression::get_display_name(){
	return get_symbol_from_scope(M_identifier->identifier_name())->get_display_name();
}

Field_Access_Expression::~Field_Access_Expression(){
	delete M_identifier;
}

}




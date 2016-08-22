#include "Field_Access_Expression.h"
//
#include <j_symbol/J_Symbol_Identifier.h>
//
#include "JC_Symbol_Error.h"
namespace jomike{



Field_Access_Expression::Field_Access_Expression(
	const yy::location& irk_loc, J_Symbol_Identifier<jc_string_t>* i_name)
:j_expression(irk_loc, Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED){

	assert(i_name);
	M_identifier = i_name;
}

Field_Access_Expression::Field_Access_Expression(const Field_Access_Expression& irk_source) : j_expression(irk_source){
	M_identifier = irk_source.M_identifier->get_copy();
}

j_value Field_Access_Expression::derived_get_value(const Arguments& i_args)const {
	return get_symbol_from_scope(M_identifier->identifier_name())
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

jc_string_t Field_Access_Expression::get_display_name(){
	return get_symbol_from_scope(M_identifier->identifier_name())->get_display_name();
}

Field_Access_Expression::~Field_Access_Expression(){
	delete M_identifier;
}

j_calc_symbol* Field_Access_Expression::make_non_referenced()const{
	auto symbol = get_symbol_from_scope(M_identifier->identifier_name());
	return symbol->get_copy();
}

void Field_Access_Expression::process(){
	try{
		auto symbol = get_symbol_from_scope(M_identifier->identifier_name());

		if(symbol->symbol_type() != Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED){
			set_type_syntax(symbol->type_syntax());
		}

	}catch(JC_Symbol_Error&){
		return;
	} catch(J_Syntax_Error<jc_string_t::value_type>&){
		return;
	}
}

void Field_Access_Expression::alert_symbol_scope_set(){
	
}

}




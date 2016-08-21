#include "J_Constant_Symbol.h"

//
#include "j_value.h"
//
#include "Arguments.h"
//
#include "Type_Syntax.h"
//
#include "J_Symbol_Error.h"

//
#include "Type_Factory.h"
namespace jomike{


void J_Constant_Symbol::process(){

}

void J_Constant_Symbol::alert_symbol_scope_set(){

}




jomike::j_value Int_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
	(void)i_args;
	return j_value(safe_long_long_cast(M_value), M_unit);
}

bool Int_Constant_Symbol::has_value()const {
	return true;
}

void Int_Constant_Symbol::clear(){
	M_value = 0;
}

Int_Constant_Symbol* Int_Constant_Symbol::move_copy(){
	return  new Int_Constant_Symbol(std::move(*this));
}

Int_Constant_Symbol* Int_Constant_Symbol::get_copy()const {
	return new Int_Constant_Symbol(*this);
}

Int_Constant_Symbol::Int_Constant_Symbol(const yy::location& irk_loc, int i_val)
	:J_Constant_Symbol(irk_loc, Symbol_Types::INT){
	M_value = i_val;
}

jc_string_t Int_Constant_Symbol::get_display_name(){
	return derived_get_value(empty_arguments()).to_str();
}

j_calc_symbol* Int_Constant_Symbol::convert_to_type(const Type_Syntax& irk_type)const {
	switch(irk_type.symbol_type()){
	case Symbol_Types::DOUBLE:{
		auto dbl_symbol = new Dbl_Constant_Symbol(location(), static_cast<j_dbl>(M_value));
		dbl_symbol->set_name(name());
		return dbl_symbol;
	}
	default:
		throw J_Symbol_Error(L"Cannot Convert Int Constant to Type: " + irk_type.type_name() + L" implicitly.");
	}
}




Bool_Constant_Symbol::Bool_Constant_Symbol(const yy::location& i_loc, bool i_val)
:J_Constant_Symbol(i_loc, Symbol_Types::BOOL){
	M_value = i_val;
}





jomike::j_value Bool_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
	(void)i_args;
	return j_value(M_value, M_unit);
}

bool Bool_Constant_Symbol::has_value()const {
	return true;
}

void Bool_Constant_Symbol::clear(){
	M_value = false;
}

Bool_Constant_Symbol* Bool_Constant_Symbol::move_copy(){
	return  new Bool_Constant_Symbol(std::move(*this));
}

Bool_Constant_Symbol* Bool_Constant_Symbol::get_copy()const {
	return new Bool_Constant_Symbol(*this);
}

jc_string_t Bool_Constant_Symbol::get_display_name(){
	return derived_get_value(empty_arguments()).to_str();
}


jomike::j_value Dbl_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
	(void)i_args;
	return j_value(M_value, M_unit);
}

bool Dbl_Constant_Symbol::has_value()const {
	return true;
}

void Dbl_Constant_Symbol::clear(){
	M_value = 0.0;
}

Dbl_Constant_Symbol* Dbl_Constant_Symbol::move_copy(){
	return  new Dbl_Constant_Symbol(std::move(*this));
}

Dbl_Constant_Symbol* Dbl_Constant_Symbol::get_copy()const {
	return new Dbl_Constant_Symbol(*this);
}

Dbl_Constant_Symbol::Dbl_Constant_Symbol(const yy::location& i_loc, j_dbl i_val)
	:J_Constant_Symbol(i_loc, Symbol_Types::DOUBLE){
	M_value = i_val;
}

jc_string_t Dbl_Constant_Symbol::get_display_name(){
	return get_value().to_str();
}


j_value String_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
	(void)i_args;
	return j_value(M_value, M_unit);
}

bool String_Constant_Symbol::has_value()const {
	return true;
}

void String_Constant_Symbol::clear(){
	M_value.clear();
}

String_Constant_Symbol* String_Constant_Symbol::move_copy(){
	return  new String_Constant_Symbol(std::move(*this));
}

String_Constant_Symbol* String_Constant_Symbol::get_copy()const {
	return new String_Constant_Symbol(*this);
}

String_Constant_Symbol::String_Constant_Symbol(
	const yy::location& i_loc, const jc_string_t& irk_val)
	:J_Constant_Symbol(i_loc, Symbol_Types::STRING){
	M_value = irk_val;
}

jc_string_t String_Constant_Symbol::get_display_name(){
	assert(M_value == derived_get_value(empty_arguments()).to_str());
	return M_value;
}

}



#include "Constant_Symbol.h"

using std::to_string;
namespace jomike{

Constant_Symbol::Constant_Symbol(
	Symbol_Types i_symbol_type, const Lexer_Location& /*i_loc*/):j_symbol(i_symbol_type){

}




jomike::j_value Int_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
	return j_value(M_value, M_unit);
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

Int_Constant_Symbol::Int_Constant_Symbol(int i_val, const Lexer_Location& i_loc)
	:Constant_Symbol(Symbol_Types::INT, i_loc){
	M_value = i_val;
}

jomike::J_UI_String Int_Constant_Symbol::get_display_name(){
	return to_string(M_value);
}




Bool_Constant_Symbol::Bool_Constant_Symbol(bool i_val, const Lexer_Location& i_loc)
:Constant_Symbol(Symbol_Types::BOOL, i_loc){
	M_value = i_val;
}





jomike::j_value Bool_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
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


jomike::j_value Dbl_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
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

Dbl_Constant_Symbol::Dbl_Constant_Symbol(j_dbl i_val, const Lexer_Location& i_loc)
	:Constant_Symbol(Symbol_Types::DOUBLE, i_loc){
	M_value = i_val;
}


j_value String_Constant_Symbol::derived_get_value(const Arguments& i_args)const {
	assert(i_args.empty());
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
	const std::string& irk_val, const Lexer_Location& i_loc)
	:Constant_Symbol(Symbol_Types::STRING, i_loc){
	M_value = irk_val;
}

}



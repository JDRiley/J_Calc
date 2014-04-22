//
#include "Math_Parser.h"
//
#include "Math_Lexer.h"
//
#include <sstream>
//
#include <iostream>
//
#include "J_Calc_Error.h"
//
#include <J_Symbol_Error.h>
//
#include <Arguments.h>
//
#include <Constant_Symbol.h>
//
#include <j_declaration.h>
//
#include <Type_Syntax.h>
//
#include <algorithm>
//
#include <functional>
//
#include <utility>


using std::mem_fn; using std::bind; using std::for_each; using std::transform;
using namespace std::placeholders;
using std::stringstream;
using std::string;

int j_semantic_type::base_restrictive_ptr::Ms_restrictive_ptr_ID = 0;

namespace jomike{

token_t Math_Parser::lex(semantic_t* yylval){
	return M_lexer->yylex(yylval);
}

j_symbol* Math_Parser::parse(const std::string& irk_string){
	j_symbol* symbol = nullptr;
	yy::Math_Parsing_Unit parsing_unit(&symbol, this);
	stringstream str_stream(irk_string);
	M_lexer->yyrestart(&str_stream);

	parsing_unit.parse();
	
	
	return symbol;

}

Math_Parser::Math_Parser(){
	M_lexer = new Math_Lexer;
}

Math_Parser::~Math_Parser(){
	delete M_lexer;
}

}

yy::Math_Parsing_Unit::token_type yylex(
	yy::Math_Parsing_Unit::semantic_type* yylval, jtl::Math_Parser* i_parser){
	return i_parser->lex(yylval);
}


void yy::Math_Parsing_Unit::error(const location_type& , const std::string& irk_string){
	std::cerr << "Error Encountered: " << irk_string << "\n";
	throw jtl::J_Syntax_Error("[Syntax_Error]");
}


j_semantic_type::j_semantic_type()
:identifier(this)
, symbol_component(this)
, constant_symbol(this)
, declaration(this)
, type_syntax(this)
, expression(this)
, arguments(this)
, symbol(this){
	set_pointer_map();
	
	 
	
}

void j_semantic_type::set_pointer_map(){
	M_ptrs = {
		{identifier.ID(), &identifier}
		, {symbol_component.ID(), &symbol_component}
		, {constant_symbol.ID(), &constant_symbol}
		, {declaration.ID(), &declaration}
		, {type_syntax.ID(), &type_syntax}
		, {expression.ID(), &expression}
		, {arguments.ID(), &arguments}
		, {symbol.ID(), &symbol}
	};
}

j_semantic_type::j_semantic_type(const j_semantic_type& irk_right)
:identifier(this)
, symbol_component(this)
, constant_symbol(this)
, declaration(this)
, type_syntax(this)
, expression(this)
, arguments(this)
, symbol(this){
	set_pointer_map();


	copy_data(irk_right);

}

j_semantic_type::j_semantic_type(j_semantic_type&& irr_right)
:identifier(this)
, symbol_component(this)
, constant_symbol(this)
, declaration(this)
, type_syntax(this)
, expression(this)
, arguments(this)
, symbol(this){
	set_pointer_map();
	swap(irr_right);
}
template<typename St>
j_semantic_type::restrictive_ptr<St>::~restrictive_ptr(){
	destroy();
}


template<typename St>
void j_semantic_type::restrictive_ptr<St>::destroy(){
	delete M_ptr;
	M_ptr = nullptr;
}

j_semantic_type::~j_semantic_type(){

}

j_semantic_type& j_semantic_type::operator=(const j_semantic_type& irk_right){
	j_semantic_type temp(irk_right);
	swap(temp);
	return *this;
}

class Move_Ptr{
public:
	template<typename St>
	St* operator()(j_semantic_type::restrictive_ptr<St>& irk_src)const{
		return irk_src.take();
	}
};

void j_semantic_type::move_data(j_semantic_type& ir_right){

	get_data(ir_right, Move_Ptr());
}


template<typename Semantic_t, typename Ptr_Retrieval_t>
typename jtl::enable_if_same_non_qualified_type<
	Semantic_t, j_semantic_type, void>::type  j_semantic_type::get_data(
	Semantic_t& irk_right, Ptr_Retrieval_t i_func){

	int num_set = 0;

	if(auto ptr = i_func(irk_right.identifier)){
		++num_set;
		identifier = ptr;
	}

	if(auto ptr = i_func(irk_right.constant_symbol)){
		++num_set;
		constant_symbol = ptr;
	}

	if(auto ptr = i_func(irk_right.declaration)){
		++num_set;
		declaration = ptr;
	}

	if(auto ptr = i_func(irk_right.type_syntax)){
		++num_set;
		type_syntax = ptr;
	}

	if(auto ptr = i_func(irk_right.expression)){
		++num_set;
		expression = ptr;
	}

	if(auto ptr = i_func(irk_right.arguments)){
		++num_set;
		arguments = ptr;
	}

	if(auto ptr = i_func(irk_right.symbol)){
		++num_set;
		symbol = ptr;
	}

	


	assert((num_set < 2) || "Num Set Should Be only 1 max");
}

j_semantic_type& j_semantic_type::operator==(j_semantic_type&& irr_right){
	set_all_null();
	swap(irr_right);
	return *this;
}
class Copy_Ptr{
public:
	template<typename St>
	St* operator()(const j_semantic_type::restrictive_ptr<St>& irk_src)const{
		return irk_src.copy_ptr();
	}
};

void j_semantic_type::copy_data(const j_semantic_type &irk_right){

	get_data(irk_right, Copy_Ptr());
}

void j_semantic_type::swap(j_semantic_type& irk_right){
	assert(M_ptrs.size() == irk_right.M_ptrs.size());
	auto src_pos = irk_right.M_ptrs.begin();
	for(auto f_res_ptr : M_ptrs){
		f_res_ptr->swap(**src_pos++);
	}
}

template<typename St>
St* j_semantic_type::restrictive_ptr<St>::copy_ptr()const{
	return M_ptr ? M_ptr->get_copy() : nullptr;
}

template<typename St>
void j_semantic_type::restrictive_ptr<St>::swap(base_restrictive_ptr& ir_right){
	restrictive_ptr<St>* src_ptr = dynamic_cast<restrictive_ptr<St>*>(&ir_right);

	assert(src_ptr);
	swap(*src_ptr);
}


template<typename St>
void j_semantic_type::restrictive_ptr<St>::swap(
	j_semantic_type::restrictive_ptr<St>& ir_right){
	std::swap(M_ptr, ir_right.M_ptr);
}

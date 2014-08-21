//
#include "Math_Parser.h"
//
#include "Math_Lexer.h"
//
#include <sstream>
//
#include <iostream>
//
#include <J_Symbol_Error.h>
//
#include "../Arguments.h"
//
#include "../Constant_Symbol.h"
//
#include "../j_declaration.h"
//
#include <Type_Syntax.h>
//
#include <algorithm>
//
#include <functional>
//
#include <utility>
//
#include "../Specific_Symbol_List.h"

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
	//parsing_unit.set_debug_level(1);
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

#define MEMBER_INITIALIZATION \
	identifier(this)\
, symbol_component(this)\
, constant_symbol(this)\
, declaration(this)\
, type_syntax(this)\
, expression(this)\
, arguments(this)\
, symbol(this)\
, symbol_list(this)\
, symbol_scope(this)\
, declaration_list(this)

j_semantic_type::j_semantic_type()
:MEMBER_INITIALIZATION {
	set_pointer_map();
	
	 
	
}
j_semantic_type::j_semantic_type(const j_semantic_type& irk_right)
:MEMBER_INITIALIZATION{
	set_pointer_map();


	copy_data(irk_right);

}

j_semantic_type::j_semantic_type(j_semantic_type&& irr_right)
: MEMBER_INITIALIZATION{
	set_pointer_map();
	swap(irr_right);
}

#undef MEMBER_INITIALIZATION


void j_semantic_type::set_pointer_map(){
#define SET_ID_TO_PTR(ptr) {ptr.ID(), &ptr}

	M_ptrs = {
		SET_ID_TO_PTR(identifier)
		, SET_ID_TO_PTR(symbol_component)
		, SET_ID_TO_PTR(constant_symbol)
		, SET_ID_TO_PTR(declaration)
		, SET_ID_TO_PTR(type_syntax)
		, SET_ID_TO_PTR(expression)
		, SET_ID_TO_PTR(arguments)
		, SET_ID_TO_PTR(symbol)
		, SET_ID_TO_PTR(symbol_list)
		, SET_ID_TO_PTR(symbol_scope)
		, SET_ID_TO_PTR(declaration_list)
	};
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



template<typename St, typename Ptr_Retrieval_t>
static void assign_ptr(
	int& i_num_set, j_semantic_type::restrictive_ptr<St>& i_left
	, j_semantic_type::restrictive_ptr<St>& i_right, Ptr_Retrieval_t& i_func){
	if(auto ptr = i_func(i_right)){
		++i_num_set;
		i_left = ptr;
	}
}

template<typename St, typename Ptr_Retrieval_t>
static void assign_ptr(
	int& i_num_set, j_semantic_type::restrictive_ptr<St>& i_left
	, const j_semantic_type::restrictive_ptr<St>& irk_right, Ptr_Retrieval_t& i_func){
	if(auto ptr = i_func(irk_right)){
		++i_num_set;
		i_left = ptr;
	}
}

template<typename Semantic_t, typename Ptr_Retrieval_t>
typename jtl::enable_if_same_non_qualified_type<
	Semantic_t, j_semantic_type, void>::type  j_semantic_type::get_data(
	Semantic_t& irk_right, Ptr_Retrieval_t i_func){

	int num_set = 0;



#define PTR_ASSIGN(i_ptr) assign_ptr(num_set, i_ptr, irk_right.i_ptr, i_func);

	PTR_ASSIGN(identifier);
	PTR_ASSIGN(symbol_component);
	PTR_ASSIGN(constant_symbol);
	PTR_ASSIGN(declaration);
	PTR_ASSIGN(type_syntax);
	PTR_ASSIGN(expression);
	PTR_ASSIGN(arguments);
	PTR_ASSIGN(symbol);
	PTR_ASSIGN(symbol_list);
	PTR_ASSIGN(symbol_scope);
	PTR_ASSIGN(declaration_list);

#undef PTR_ASSIGN

	

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

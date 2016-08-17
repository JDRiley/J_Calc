//
#include "J_Calc_Parser.h"
//
#include "J_Calc_Lexer.h"
//
#include <j_symbol/J_Symbol_Error.h>
//
#include <j_symbol/Specific_Symbol_List.h>
//
#include <j_symbol/J_Symbol_Identifier.h>
//
#include "../j_declaration.h"
//
#include "../Type_Syntax.h"
//
#include "../j_expression.h"
//
#include "../Arguments.h"
//
#include "../J_Symbol_Scope.h"
//
#include "../j_statement.h"
//
#include "../Statement_Block.h"
//
#include "../If_Statement.h"
//
#include <sstream>
//
#include <iostream>
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



namespace jomike{

token_t J_Calc_Parser::lex(semantic_t* yylval, yy::location* i_loc){
	return M_lexer->yylex(yylval, i_loc, this);
}

j_calc_symbol* J_Calc_Parser::parse(const std::string& irk_string){
	j_calc_symbol* symbol = nullptr;
	yy::J_Calc_Parsing_Unit parsing_unit(&symbol, this);


	
	//parsing_unit.set_debug_level(1);
	stringstream str_stream(irk_string);
	M_lexer->yyrestart(&str_stream);

	parsing_unit.parse();
	
	
	return symbol;

}

std::string J_Calc_Parser::get_line(int i_line){
	return M_lexer->get_line(i_line);
}

J_Calc_Parser::J_Calc_Parser(){
	M_lexer = new J_Calc_Lexer;
}

J_Calc_Parser::~J_Calc_Parser(){
	delete M_lexer;
}

}

yy::J_Calc_Parsing_Unit::token_type yylex(
	yy::J_Calc_Parsing_Unit::semantic_type* yylval, yy::location* i_loc, jtl::J_Calc_Parser* i_parser){

	
	return  i_parser->lex(yylval, i_loc);

}


void yy::J_Calc_Parsing_Unit::error(const location_type& irk_location, const std::string& irk_string){
	std::cerr << "\nError Encountered: " << irk_string << " at location " << irk_location << "\n";
	std::stringstream o_str;
	o_str << irk_location;

	std::cerr << "\nLines:\n";
	for(unsigned i = irk_location.begin.line - 1; i < irk_location.end.line; ++i){
		std::cerr << "\n" << i_parser->get_line(i);
	}

	throw jtl::J_Syntax_Error("[Syntax_Error] " + o_str.str());
}


#define COMMA ,
#define SEMI_COLON ;
#define FOR_ALL(m_macro, seperator)\
	m_macro(identifier) seperator\
	m_macro(symbol_component) seperator\
	m_macro(constant_symbol) seperator\
	m_macro(declaration) seperator\
	m_macro(type_syntax) seperator\
	m_macro(expression) seperator\
	m_macro(arguments) seperator\
	m_macro(calc_symbol) seperator\
	m_macro(symbol_list) seperator\
	m_macro(symbol_scope) seperator\
	m_macro(declaration_list) seperator\
	m_macro(statement) seperator\
	m_macro(statement_block) seperator\
	m_macro(if_statement)


#define INITIALIZE_MEMBER(name) name(this)

#define MEMBER_INITIALIZATION FOR_ALL(INITIALIZE_MEMBER, COMMA)

J_Calc_Semantic::J_Calc_Semantic()
:MEMBER_INITIALIZATION {
	set_pointer_map();
	
	 
	
}
J_Calc_Semantic::J_Calc_Semantic(const J_Calc_Semantic& irk_right)
:MEMBER_INITIALIZATION{
	set_pointer_map();


	copy_data(irk_right);

}

J_Calc_Semantic::J_Calc_Semantic(J_Calc_Semantic&& irr_right)
: MEMBER_INITIALIZATION{
	set_pointer_map();
	swap(irr_right);
}

#undef MEMBER_INITIALIZATION


void J_Calc_Semantic::set_pointer_map(){
#define SET_ID_TO_PTR(ptr) {ptr.ID(), &ptr}

	M_ptrs = {
		FOR_ALL(SET_ID_TO_PTR, COMMA)
	};
#undef SET_ID_TO_PTR
}



J_Calc_Semantic::~J_Calc_Semantic(){

}

J_Calc_Semantic& J_Calc_Semantic::operator=(const J_Calc_Semantic& irk_right){
	J_Calc_Semantic temp(irk_right);
	swap(temp);
	return *this;
}

class Move_Ptr{
public:
	template<typename St, typename Parent_t>
	St* operator()(jtl::restrictive_ptr<St, Parent_t>& irk_src)const{
		return irk_src.take();
	}
};

void J_Calc_Semantic::move_data(J_Calc_Semantic& ir_right){

	get_data(ir_right, Move_Ptr());
}



template<typename St, typename Parent_t, typename Ptr_Retrieval_t>
static void assign_ptr(
	int& i_num_set, jtl::restrictive_ptr<St, Parent_t>& i_left
	, jtl::restrictive_ptr<St, Parent_t>& i_right, Ptr_Retrieval_t& i_func){
	if(auto ptr = i_func(i_right)){
		++i_num_set;
		i_left = ptr;
	}
}

template<typename St, typename Parent_t, typename Ptr_Retrieval_t>
static void assign_ptr(
	int& i_num_set, jtl::restrictive_ptr<St, Parent_t>& i_left
	, const jtl::restrictive_ptr<St, Parent_t>& irk_right, Ptr_Retrieval_t& i_func){
	if(auto ptr = i_func(irk_right)){
		++i_num_set;
		i_left = ptr;
	}
}

template<typename Semantic_t, typename Ptr_Retrieval_t>
typename jtl::enable_if_same_non_qualified_type<
	Semantic_t, J_Calc_Semantic, void>::type  J_Calc_Semantic::get_data(
	Semantic_t& irk_right, Ptr_Retrieval_t i_func){

	int num_set = 0;



#define PTR_ASSIGN(i_ptr) assign_ptr(num_set, i_ptr, irk_right.i_ptr, i_func);
	FOR_ALL(PTR_ASSIGN, SEMI_COLON);


#undef PTR_ASSIGN

	

	assert((num_set < 2) || "Num Set Should Be only 1 max");
}

J_Calc_Semantic& J_Calc_Semantic::operator==(J_Calc_Semantic&& irr_right){
	set_all_null();
	swap(irr_right);
	return *this;
}
class Copy_Ptr{
public:
	template<typename St, typename Parent_t>
	St* operator()(const jtl::restrictive_ptr<St, Parent_t>& irk_src)const{
		return irk_src.copy_ptr();
	}
};

void J_Calc_Semantic::copy_data(const J_Calc_Semantic &irk_right){

	get_data(irk_right, Copy_Ptr());
}

void J_Calc_Semantic::swap(J_Calc_Semantic& ir_right){
	assert(M_ptrs.size() == ir_right.M_ptrs.size());
	auto src_pos = ir_right.M_ptrs.begin();
	for(auto f_res_ptr : M_ptrs){
		f_res_ptr->swap(**src_pos++);
	}
}

#include "restrive_ptr.hpp"
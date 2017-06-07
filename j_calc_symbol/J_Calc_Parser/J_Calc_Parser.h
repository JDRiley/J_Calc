#ifndef J_CALC_PARSER_PARSER_H
#define J_CALC_PARSER_PARSER_H

//
#include "../J_Calc_Symbol_Fwd_Decl.h"
//
#include <j_symbol/J_Symbol_Fwd_Decl.h>
//
#include <j_map.h>
//
#include <j_symbol/restrictive_ptr.h>

namespace jomike{
J_FWD_DECL(J_Calc_Parser)
J_FWD_DECL(J_Calc_Lexer)

}



//Class just so a this pointer exists upon construction of derived classes
struct j_base{

};
struct J_Calc_Semantic : public j_base {

	J_Calc_Semantic();
	J_Calc_Semantic(const J_Calc_Semantic& irk_right);
	J_Calc_Semantic(J_Calc_Semantic&& irr_right);
	

	J_Calc_Semantic& operator=(const J_Calc_Semantic& irk_right);
	J_Calc_Semantic& operator==(J_Calc_Semantic&& irr_right);
	~J_Calc_Semantic();



	void copy_data(const J_Calc_Semantic& irk_right);
	void move_data(J_Calc_Semantic& irk_right);

	template<typename St>
	using restrictive_ptr_t = jtl::restrictive_ptr<St, J_Calc_Semantic>;

	restrictive_ptr_t<jtl::J_Symbol_Identifier<jtl::jc_string_t>>	identifier;
	restrictive_ptr_t<jtl::j_symbol_component>	symbol_component;
	restrictive_ptr_t<jtl::jc_constant_symbol>		constant_symbol;
	restrictive_ptr_t<jtl::j_declaration>			declaration;
	restrictive_ptr_t<jtl::Type_Syntax>			type_syntax;
	restrictive_ptr_t<jtl::j_calc_expression>			expression;
	restrictive_ptr_t<jtl::Arguments>				arguments;
	restrictive_ptr_t<jtl::j_calc_symbol>			calc_symbol;
	restrictive_ptr_t<jtl::J_Calc_Symbol_List>	symbol_list;
	restrictive_ptr_t<jtl::J_Symbol_Scope>		symbol_scope;
	restrictive_ptr_t<jtl::Declaration_List>		declaration_list;
	restrictive_ptr_t<jtl::j_statement>			statement;
	restrictive_ptr_t<jtl::Statement_Block>		statement_block;
	restrictive_ptr_t<jtl::If_Statement>			if_statement;

	void set_all_null(){
		for(auto f_ptrs : M_ptrs){
			f_ptrs->destroy();
		}
	}


	jtl::j_map<int, jtl::base_restrictive_ptr*> M_ptrs;
	void set_pointer_map();
	void swap(J_Calc_Semantic& irk_right);
	template<typename Semantic_t, typename Ptr_Retrieval_t>
	typename jtl::enable_if_same_non_qualified_type<
		Semantic_t, J_Calc_Semantic, void>::type 
		get_data(Semantic_t& irk_right, Ptr_Retrieval_t i_func);
};


#define YYSTYPE J_Calc_Semantic
#define YY_NO_UNISTD_H
#ifdef _DEBUG


#define YYDEBUG 1
#endif

//
#include "J_Calc_Parser.tab.h"



typedef yy::J_Calc_Parsing_Unit::token_type token_t;
typedef yy::J_Calc_Parsing_Unit::semantic_type semantic_t;
yy::J_Calc_Parsing_Unit::token_type
yylex(yy::J_Calc_Parsing_Unit::semantic_type* yylval, yy::location* ,jtl::J_Calc_Parser* i_parser);

namespace jomike{
class J_Calc_Parser{
public:
	J_Calc_Parser();
	token_t lex(semantic_t* yylval, yy::location* i_loc);
	j_calc_symbol* parse(const jc_string_t& irk_string);

	jc_string_t get_line(int i_line);
	~J_Calc_Parser();
private:
	J_Calc_Lexer* M_lexer;
};


}



#endif //J_CALC_PARSER_PARSER_H



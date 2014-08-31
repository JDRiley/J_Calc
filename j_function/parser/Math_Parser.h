#ifndef MATH_PARSER_H
#define MATH_PARSER_H

#include "J_Symbol_Fwd_Decl.h"
//
#include <j_map.h>
//Class just so a this pointer exists upon construction of derived classes
struct j_base{

};
struct j_semantic_type : public j_base {

	j_semantic_type();
	j_semantic_type(const j_semantic_type& irk_right);
	j_semantic_type(j_semantic_type&& irr_right);
	

	j_semantic_type& operator=(const j_semantic_type& irk_right);
	j_semantic_type& operator==(j_semantic_type&& irr_right);
	~j_semantic_type();




	class base_restrictive_ptr{
	public:
		base_restrictive_ptr():M_ID(++Ms_restrictive_ptr_ID){}

		virtual void swap(base_restrictive_ptr& ir_right) = 0;
		int ID()const{ return M_ID; }
		virtual ~base_restrictive_ptr(){}
		virtual void destroy() = 0;
	private:


		friend struct j_semantic_type;
		static int Ms_restrictive_ptr_ID;
		int M_ID;
	};


	void set_all_null(){
		for(auto f_ptrs : M_ptrs){
			f_ptrs->destroy();
		}
	}

	template<typename St>
	class restrictive_ptr : public base_restrictive_ptr{
	public:
		restrictive_ptr(j_semantic_type* i_parent):M_parent(i_parent), M_ptr(nullptr){assert(M_parent);}


		j_semantic_type* parent()const{
			return M_parent;
		}

		void swap(base_restrictive_ptr& ir_right)override;

		void swap(restrictive_ptr& ir_right);

		~restrictive_ptr();

		restrictive_ptr& operator=(const restrictive_ptr& irk_right){
			if(M_parent == irk_right.M_parent){
				return *this;
			}
			M_parent->set_all_null();

			M_ptr = irk_right.copy_ptr();



			return *this;
		}
		
		template<typename Other_t>
		restrictive_ptr& operator=(const restrictive_ptr<Other_t>& irk_right){
			assert(!M_ptr);
			M_parent->set_all_null();

			M_ptr = irk_right.copy_ptr();


		
			return *this;
		}

		void destroy()override;

		restrictive_ptr& operator=(St* i_ptr){
			assert(i_ptr);
			
			M_parent->set_all_null();
			M_ptr = i_ptr;
			return *this;
		}
		operator St*(){
			assert(M_ptr);
			St* temp = M_ptr;
			M_ptr = nullptr;
			return temp;
		}
		St* operator->(){
			return M_ptr;
		}

		St& operator*(){
			return *M_ptr;
		}

		const St* operator->()const{
			return M_ptr;
		}

		const St& operator*()const{
			return *M_ptr;
		}

		St* copy_ptr()const;
		St* take(){
			St* temp = M_ptr;
			M_ptr = nullptr;
			return temp;
		}
	private:
		j_semantic_type* M_parent;

		mutable St* M_ptr;


		template<typename Other_t>
		friend class j_semantic_type::restrictive_ptr;

		friend struct j_semantic_type;
		
		St* take()const{
			St* temp = M_ptr;
			M_ptr = nullptr;
			return temp;
		}
		
	};


	void copy_data(const j_semantic_type& irk_right);
	void move_data(j_semantic_type& irk_right);

	restrictive_ptr<jtl::J_Symbol_Identifier>	identifier;
	restrictive_ptr<jtl::j_symbol_component>	symbol_component;
	restrictive_ptr<jtl::Constant_Symbol>		constant_symbol;
	restrictive_ptr<jtl::j_declaration>			declaration;
	restrictive_ptr<jtl::Type_Syntax>			type_syntax;
	restrictive_ptr<jtl::j_expression>			expression;
	restrictive_ptr<jtl::Arguments>				arguments;
	restrictive_ptr<jtl::j_symbol>				symbol;
	restrictive_ptr<jtl::Symbol_List>			symbol_list;
	restrictive_ptr<jtl::J_Symbol_Scope>		symbol_scope;
	restrictive_ptr<jtl::Declaration_List>		declaration_list;
	restrictive_ptr<jtl::j_statement>			statement;
	restrictive_ptr<jtl::Statement_Block>		statement_block;
	restrictive_ptr<jtl::If_Statement>			if_statement;

	jtl::j_map<int, base_restrictive_ptr*> M_ptrs;
	void set_pointer_map();
	void swap(j_semantic_type& irk_right);
	template<typename Semantic_t, typename Ptr_Retrieval_t>
	typename jtl::enable_if_same_non_qualified_type<
		Semantic_t, j_semantic_type, void>::type 
		get_data(Semantic_t& irk_right, Ptr_Retrieval_t i_func);
};


#define YYSTYPE j_semantic_type
//
#include "math_parsing_unit.hh"



typedef yy::Math_Parsing_Unit::token_type token_t;
typedef yy::Math_Parsing_Unit::semantic_type semantic_t;
yy::Math_Parsing_Unit::token_type
yylex(yy::Math_Parsing_Unit::semantic_type* yylval, jtl::Math_Parser* i_parser);

namespace jomike{
class Math_Parser{
public:
	Math_Parser();
	token_t lex(semantic_t* yylval);
	j_symbol* parse(const std::string& irk_string);

	~Math_Parser();
private:
	Math_Lexer* M_lexer;
};



}

#endif //MATH_PARSER_H



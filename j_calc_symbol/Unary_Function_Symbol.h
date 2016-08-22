#ifndef UNARY_FUNCTION_SYMBOL_H
#define UNARY_FUNCTION_SYMBOL_H

#include "j_routine_symbol.h"
//
#include <string>
//
#include <functional>
//
#include "JC_Symbol_Error.h"
//
#include <j_symbol/J_Symbol_Identifier.h>
namespace jomike{

template<typename Ret_t, typename Arg_t>
class Unary_Function_Symbol : public j_routine_symbol{
public:
	template<typename Func_t>
	Unary_Function_Symbol(
		const yy::location& irk_loc, Func_t i_function, const jc_string_t& irk_string);

	Unary_Function_Symbol* move_copy()override{
		return  new Unary_Function_Symbol(std::move(*this));
	}

	Unary_Function_Symbol* get_copy()const override{
		return new Unary_Function_Symbol(*this);
	}


	void process()override;
private:
	std::function<Ret_t(Arg_t)> M_function;
	jc_string_t M_name;

	j_value derived_get_value(const Arguments& i_args)const override;

	void alert_symbol_scope_set()override;
};

template<typename Ret_t, typename Arg_t>
void jomike::Unary_Function_Symbol<Ret_t, Arg_t>::alert_symbol_scope_set(){

}

template<typename Ret_t, typename Arg_t>
void jomike::Unary_Function_Symbol<Ret_t, Arg_t>::process(){

}

template<typename Ret_t, typename Arg_t>
template<typename Func_t>
Unary_Function_Symbol<Ret_t, Arg_t>::Unary_Function_Symbol(
	const yy::location& irk_loc, Func_t i_function, const jc_string_t& irk_name)
	:j_routine_symbol(irk_loc
	 , new Type_Routine(
		irk_loc, make_type_syntax(irk_loc, CPP_To_Symbol_Type<Ret_t>::type())
		, new Type_Syntax_List(irk_loc,{make_type_syntax(irk_loc, CPP_To_Symbol_Type<Arg_t>::type())})
	 )
		, new J_Symbol_Identifier<jc_string_t>(irk_name))
	, M_function(i_function), M_name(irk_name){
	set_args(Arguments(irk_loc,1));
}


template<typename Ret_t, typename Arg_t>
j_value Unary_Function_Symbol<Ret_t, Arg_t>
	::derived_get_value(const Arguments& i_args)const {
		if(1 != i_args.size()){
			throw JC_Sym_Argument_Error(L"To many Arguments to: " + name());
		}
		return j_value(M_function(i_args[0].get_value().as_type<Arg_t>()), J_Unit());
}


}

#endif //UNARY_FUNCTION_SYMBOL_H



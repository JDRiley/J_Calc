#ifndef BINARY_FUNCTION_CHAIN_SYMBOL_H
#define BINARY_FUNCTION_CHAIN_SYMBOL_H


#include "j_routine_symbol.h"
//
#include "j_numeric.h"
//
#include "Type_Routine_Variable_Args.h"
//
#include <j_symbol/Specific_Symbol_List.h>
//
#include "Type_Factory.h"
namespace jomike{

template<typename Binary_Function_t, typename Num_t = j_dbl>
class Binary_Function_Chain_Symbol : public j_routine_symbol{
public:
	Binary_Function_Chain_Symbol(const yy::location& irk_loc, const Binary_Function_t&, const jc_string_t&);
	Binary_Function_Chain_Symbol* get_copy()const override;
	jc_string_t get_display_name()override;

	Binary_Function_Chain_Symbol* move_copy()override{
		return  new Binary_Function_Chain_Symbol(std::move(*this));
	}

	void process()override;

protected:
	j_value derived_get_value(const Arguments&)const override;
private:
	Binary_Function_t M_function;
	jc_string_t M_name;

	void alert_symbol_scope_set()override;
};

template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
void jomike::Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>::alert_symbol_scope_set(){

}

template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
void jomike::Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>::process(){

}


#define BINARY_FUNCTION_CHAIN_SYMBOL_NUM_DECL(num_type, name_prefix)\
	typedef Binary_Function_Chain_Symbol<num_type(*)(num_type, num_type), num_type>\
		name_prefix##_Binary_Function_Symbol;


BINARY_FUNCTION_CHAIN_SYMBOL_NUM_DECL(j_llint, LLint)
BINARY_FUNCTION_CHAIN_SYMBOL_NUM_DECL(j_dbl, Dbl)
BINARY_FUNCTION_CHAIN_SYMBOL_NUM_DECL(int, Int)
BINARY_FUNCTION_CHAIN_SYMBOL_NUM_DECL(long, Lint)


template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
j_value Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>
	::derived_get_value(const Arguments& irk_args)const{
		if(irk_args.empty()){
			return j_value(static_cast<Num_t>(0), J_Unit());
		}


		Num_t answer = irk_args[0].get_value().as_type<Num_t>();

		

		answer = std::accumulate(irk_args.begin() + 1, irk_args.end(), answer
			, [&](Num_t i_val, j_calc_symbol* i_symbol){
			return M_function(i_val, i_symbol->get_value().as_type<Num_t>());
		});

		return j_value(answer, J_Unit());
	}


template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>*
	Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>::get_copy()const {

		return new Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>(*this);
	}


template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
jc_string_t Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>::get_display_name(){
	return M_name;
}


template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>
	::Binary_Function_Chain_Symbol(const yy::location& irk_loc
		, const Binary_Function_t& irk_function
	, const jc_string_t& irk_name)
	: j_routine_symbol(irk_loc
	, new Type_Routine_Variable_Args(irk_loc, make_type_syntax(irk_loc, CPP_To_Symbol_Type<Num_t>::type())	
		, new Type_Syntax_List(irk_loc), make_type_syntax(irk_loc, CPP_To_Symbol_Type<Num_t>::type()))
	, new J_Symbol_Identifier<jc_string_t>(irk_name))
	, M_function(irk_function), M_name(irk_name){
		set_args(Arguments(irk_loc,2));
	}


}

#endif //BINARY_FUNCTION_CHAIN_SYMBOL_H


#ifndef BINARY_FUNCTION_CHAIN_SYMBOL_H
#define BINARY_FUNCTION_CHAIN_SYMBOL_H


#include "j_routine_symbol.h"
//
#include "j_numeric.h"

namespace jomike{

template<typename Binary_Function_t, typename Num_t = j_dbl>
class Binary_Function_Chain_Symbol : public j_routine_wrapper_symbol{
public:
	Binary_Function_Chain_Symbol(const Binary_Function_t&, const std::string&);
	Binary_Function_Chain_Symbol* get_copy()const override;
	J_UI_String get_display_name()override;

	Binary_Function_Chain_Symbol* move_copy()override{
		return  new Binary_Function_Chain_Symbol(std::move(*this));
	}

protected:
	j_value derived_get_value(const Arguments&)const override;
private:
	Binary_Function_t M_function;
	J_UI_String M_name;
};

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
			return j_value(0, J_Unit());
		}


		Num_t answer = static_cast<j_llint>(irk_args[0].value(Arguments()));

		answer = std::accumulate(irk_args.begin() + 1, irk_args.end(), answer
			, [&](Num_t i_val, j_symbol* i_symbol){
			return M_function(i_val, static_cast<Num_t>(i_symbol->value()));
		});

		return j_value(static_cast<j_dbl>(answer), J_Unit());
	}


template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>*
	Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>::get_copy()const {

		return new Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>(*this);
	}


template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
J_UI_String Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>::get_display_name(){
	return M_name;
}


template<typename Binary_Function_t, typename Num_t /*= j_dbl*/>
Binary_Function_Chain_Symbol<Binary_Function_t, Num_t>
	::Binary_Function_Chain_Symbol(const Binary_Function_t& irk_function
	, const std::string& irk_name): j_routine_wrapper_symbol(irk_name, CPP_To_Symbol_Type<Num_t>::type())
	, M_function(irk_function), M_name(irk_name){
		set_args(Arguments(2));
	}


}

#endif //BINARY_FUNCTION_CHAIN_SYMBOL_H


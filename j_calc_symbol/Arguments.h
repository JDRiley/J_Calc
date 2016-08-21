#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <J_Fwd_Decl.h>
//
#include <j_symbol/j_symbol.h>

//
#include "j_expression.h"
//
#include <j_symbol/Specific_Symbol_List.h>
namespace jomike{
class j_calc_symbol;




class Arguments : public j_calc_symbol_component{
public:
	//Constructors

	Arguments(const yy::location& irk_loc, j_size_t i_size = J_SIZE_T_ZERO);



	template<typename Iter>
	Arguments(Iter i_first, Iter i_last);

	Arguments& operator=(const Arguments&);
	Arguments& operator=(Arguments&&);

	j_expression& operator[](j_size_t);
	const j_expression& operator[](j_size_t)const;

	void clear();

	Arguments* get_copy()const override;

	Arguments* move_copy()override;


	//Destructor
	~Arguments();
	//Mutation
	void push_back(const j_expression&);
	void push_back(j_expression*);
	void set_argument(j_size_t index, const j_expression*);
	
	bool empty()const;

	j_size_t size()const;
	j_expression* const* arguments();
	const j_expression* const* arguments()const;

	void swap(Arguments&);

	virtual void set_symbol_scope(J_Symbol_Scope* i_scope);
	Arguments(Arguments&&) = default;
	Arguments(const Arguments&) = default;

	typedef J_Symbol_Component_Array<j_expression>::const_iterator const_iterator;
	const_iterator begin()const;
	const_iterator end()const;
private:
	J_Symbol_Component_Array<j_expression> M_arg_symbols;
};




}


#endif
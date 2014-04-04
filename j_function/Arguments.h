#ifndef J_FUNCTION_UTILE
#define J_FUNCTION_UTILE

#include <J_Fwd_Decl.h>
//
#include "j_symbol_component.h"
//
#include <J_UI/J_UI_String.h>
//
#include <ex_array.h>

namespace jomike{
class j_symbol;




class Arguments : public j_symbol_component{
public:
	//Constructors
	Arguments();
	Arguments(const Arguments&);
	Arguments(j_size_t);

	Arguments(Arguments&&);

	template<typename Iter>
	Arguments(Iter i_first, Iter i_last);

	Arguments& operator=(const Arguments&);
	Arguments& operator=(Arguments&&);

	j_symbol& operator[](j_size_t);
	const j_symbol& operator[](j_size_t)const;

	void clear();

	Arguments* get_copy()const override;

	Arguments* move_copy()override;

	J_UI_String get_display_name()override;

	//Destructor
	~Arguments();
	//Mutation
	void push_back(const j_symbol&);
	void set_argument(j_size_t index, const j_symbol*);
	
	bool empty()const;

	j_size_t size()const;
	j_symbol* const* arguments();
	const j_symbol* const* arguments()const;

	void swap(Arguments&);

	typedef ex_array<j_symbol*>::const_iterator const_iterator;
	const_iterator begin()const;
	const_iterator end()const;
private:
	ex_array<j_symbol*> M_arg_symbols;
};




}


#endif
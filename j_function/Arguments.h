#ifndef J_FUNCTION_UTILE
#define J_FUNCTION_UTILE

#include <J_Fwd_Decl.h>
//
#include <J_UI/J_UI_String.h>
//
#include <ex_array.h>

namespace jomike{
class j_symbol;




class Arguments{
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

template<typename Iter>
Arguments::Arguments(Iter i_first, Iter i_last){
	while(i_first != i_last){
		push_back(**i_first);
		++i_first;
	}
}




}


#endif
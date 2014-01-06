#ifndef J_ROUTINE_SYMBOL_H
#define J_ROUTINE_SYMBOL_H

#include "j_symbol.h"
//
#include <map>

namespace jomike{

class j_routine_symbol : public j_symbol{
public:
	j_routine_symbol();
	j_routine_symbol* get_copy()const override;
	j_routine_symbol(const j_routine_symbol&);
	j_routine_symbol(j_routine_symbol&&);

	bool symbol_name_availability_status(const J_UI_String&);
	j_symbol* get_scope_symbol(const J_UI_String&);
	void add_scope_symbol(const j_symbol&);
	void add_arg_symbol(const j_symbol&);
	void add_interface_symbol(const j_symbol&);
	void set_return_symbol(const j_symbol&);
	void clear()override;
	bool has_value()const override;
	void add_statement(const J_UI_String& irk_string);

	~j_routine_symbol();
protected:
	
private:
	typedef std::map<J_UI_String, j_symbol*> Symbol_Cont_t;
	Symbol_Cont_t M_scope_symbols;
	Symbol_Cont_t M_arg_symbols;
	Symbol_Cont_t M_interface_symbols;
	j_symbol* M_return_symbol = nullptr;
	ex_array<J_UI_String> M_statements;
	j_value derived_get_value(const Arguments&)const override;
};


class j_routine_wrapper_symbol : public j_symbol{
public:
	void clear()override;
	bool has_value()const override;
private:
};



}

#endif //J_ROUTINE_SYMBOL_H


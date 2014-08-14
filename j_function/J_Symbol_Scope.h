#ifndef J_SYMBOL_SCOPE_H
#define J_SYMBOL_SCOPE_H

#include "J_Symbol_Fwd_Decl.h"
//
#include <j_map.h>
//
#include <J_UI/J_UI_String.h>
namespace jomike{
	
class J_Symbol_Scope{
public:
	J_Symbol_Scope(const J_Symbol_Scope* i_parent_symbol = nullptr);
	J_Symbol_Scope(const Declaration_List& irk_list);
	virtual j_symbol* get_symbol(const J_UI_String& irk_string)const;

	J_Symbol_Scope(const J_Symbol_Scope& irk_right);

	virtual void add_symbol(j_symbol* i_symbol);
	virtual void add_symbol(const j_symbol& irk_symbol);
	virtual J_Symbol_Scope* get_copy()const;

	virtual J_Symbol_Scope* move_copy();

	virtual ~J_Symbol_Scope();

	void set_parent_scope(const J_Symbol_Scope* i_parent_scope);

private:
	const J_Symbol_Scope* M_parent_scope;

	typedef j_map<J_UI_String, j_symbol*> Symbol_Map_t;
	
	Symbol_Map_t M_symbols;
};

}

#endif //J_SYMBOL_SCOPE_H



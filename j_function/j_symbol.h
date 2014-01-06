#ifndef J_SYMBOL_H
#define J_SYMBOL_H

#include "J_Symbol_Fwd_Decl.h"
//
#include "j_value.h"
//
#include "Arguments.h"
//
#include <J_UI\J_UI_String.h>

namespace jomike{

class j_symbol{
public:
	j_symbol();
	j_symbol(const J_UI_String&);

	//j_symbol(const j_symbol& irk_symbol);


	//Copy Construction
	virtual j_symbol* get_copy()const = 0;
	virtual j_symbol* reduce()const;

	virtual j_value get_value(const Arguments& i_args = Arguments())const;
	j_dbl value(const Arguments& i_args = Arguments())const;
	virtual void clear() = 0;
	virtual bool has_value()const = 0;
	const J_UI_String& name()const;
	void set_name(const J_UI_String&);
	virtual J_UI_String get_display_name();
	virtual void set_args(const Arguments&);
	virtual void set_args(Arguments&&);
	virtual ~j_symbol();

	virtual bool is_placeholder()const;
protected:
	virtual j_value derived_get_value(const Arguments& i_args)const = 0;
private:
	J_UI_String M_name;
	const int M_ID;
	Arguments M_arguments;
};

class J_Sym_Argument_Error : public J_Error{
public:
	J_Sym_Argument_Error(const char * const ik_message = "invalid args");
private:
};


}

#endif //J_SYMBOL_H


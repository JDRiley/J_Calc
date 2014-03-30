#ifndef J_SYMBOL_H
#define J_SYMBOL_H

#include "J_Symbol_Fwd_Decl.h"
//
#include "j_value.h"
//
#include "Arguments.h"
//
#include <J_UI\J_UI_String.h>
#include "j_symbol_component.h"
//
#include "j_symbol_component.h"
#include "Constant_Symbol_Factory.h"
namespace jomike{

class j_symbol : public j_symbol_component{
public:
	j_symbol(const J_UI_String& irk_name, Symbol_Types i_symbol_type);
	j_symbol(Symbol_Types i_symbol_type = Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED);
	
	//j_symbol(const j_symbol& irk_symbol);


	//Copy Construction
	virtual j_symbol* get_copy()const = 0;
	virtual j_symbol* reduce()const;

	virtual j_value get_value(const Arguments& i_args = Arguments())const;
	j_dbl value(const Arguments& i_args = Arguments())const;
	virtual void clear() = 0;
	virtual bool has_value()const = 0;
	virtual void set_value(j_value i_value);
	J_UI_String get_display_name()override =0;
	virtual void set_args(const Arguments&);
	virtual void set_args(Arguments&&);
	virtual ~j_symbol();
	virtual j_expression* as_expression();
	Type_Syntax& type_syntax()const;
	void set_type_syntax(const Type_Syntax& irk_type_syntax);
	void set_type_syntax(Type_Syntax* i_type_syntax);
	virtual bool is_placeholder()const;
protected:
	virtual j_value derived_get_value(const Arguments& i_args)const = 0;
private:
	Arguments M_arguments;
	Type_Syntax* M_type;
};


}

#endif //J_SYMBOL_H


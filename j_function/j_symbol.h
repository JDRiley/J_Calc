#ifndef J_SYMBOL_H
#define J_SYMBOL_H

#include "j_symbol_component.h"
//
#include "j_value.h"
//
#include <J_UI\J_UI_String.h>

#include "Constant_Symbol_Factory.h"
namespace jomike{

//So Created to prevent including Arguments.h in this header.
//Empty arguments can then be passed as a default parameter
const Arguments& empty_arguments();

//returns symbol from get_j_symbol(const J_UI_String& ) function
extern const J_Symbol_Scope* gk_default_symbol_scope;

class j_symbol : public j_symbol_component{
public:
	j_symbol(J_Symbol_Identifier* irk_name, Symbol_Types i_symbol_type);
	j_symbol(Symbol_Types i_symbol_type = Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED);
	

	//Copy Construction
	j_symbol* get_copy()const override = 0;
	virtual j_symbol* reduce()const;

	const J_UI_String& name()const;

	void set_name(const J_UI_String& irk_string);

	virtual j_value get_value(const Arguments& i_args = empty_arguments())const;
	j_dbl value(const Arguments& i_args = empty_arguments())const;
	virtual void clear() = 0;
	virtual bool has_value()const = 0;
	virtual void set_value(j_value i_value);
	virtual J_UI_String get_display_name();
	virtual void set_args(const Arguments&);
	virtual void set_args(Arguments&&);
	virtual ~j_symbol();
	virtual j_expression* as_expression();
	Type_Syntax& type_syntax()const;
	void set_type_syntax(const Type_Syntax& irk_type_syntax);
	void set_type_syntax(Type_Syntax* i_type_syntax);

	virtual j_symbol* convert_to_type(const Type_Syntax& irk_type)const;

	virtual bool is_placeholder()const;


	virtual j_symbol* make_non_referenced()const;

	j_symbol(const j_symbol& irk_symbol);
	j_symbol(j_symbol&& irr_symbol);

	virtual void set_symbol_scope(const J_Symbol_Scope* i_symbol_scope);

	virtual void process();

	virtual const Type_Syntax& return_type_syntax()const;
protected:
	virtual j_value derived_get_value(const Arguments& i_args)const = 0;


	const J_Symbol_Scope& symbol_scope()const;

	j_symbol* get_symbol_from_scope(const J_UI_String& irk_string)const;
private:
	const J_Symbol_Scope* M_symbol_scope = gk_default_symbol_scope;

	Arguments* M_arguments;
	Type_Syntax* M_type = nullptr;
	J_Symbol_Identifier* M_name;
};



}

#endif //J_SYMBOL_H


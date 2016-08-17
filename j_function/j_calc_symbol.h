#ifndef J_CALC_SYMBOL_H
#define J_CALC_SYMBOL_H

#include "j_calc_symbol_component.h"
//
#include "j_value.h"
//
#include <J_UI\J_UI_String.h>
//
#include "Symbol_Types.h"

namespace jomike{

//So Created to prevent including Arguments.h in this header.
//Empty arguments can then be passed as a default parameter
const Arguments& empty_arguments();

//returns symbol from get_j_symbol(const J_UI_String& ) function
extern J_Symbol_Scope* gk_default_symbol_scope;

void add_user_symbol(j_calc_symbol*);


class j_calc_symbol : public j_calc_symbol_component{
public:
	j_calc_symbol(
		const yy::location& irk_loc, J_Symbol_Identifier* irk_name, Symbol_Types i_symbol_type);
	
	j_calc_symbol(
		const yy::location& irk_loc, Type_Syntax* i_type_syntax
		, J_Symbol_Identifier* irk_name);

	
	j_calc_symbol(
		const yy::location& irk_loc, Symbol_Types i_symbol_type = Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED);
	

	//Copy Construction
	j_calc_symbol* get_copy()const override = 0;
	virtual j_calc_symbol* reduce()const;

	const std::string& name()const;

	void set_name(const std::string& irk_string);

	virtual j_value get_value(const Arguments& i_args = empty_arguments())const;
	j_dbl value(const Arguments& i_args = empty_arguments())const;
	virtual void clear() = 0;
	virtual bool has_value()const = 0;
	virtual void set_value(j_value i_value);
	virtual std::string get_display_name();
	virtual void set_args(const Arguments&);
	virtual void set_args(Arguments&&);
	virtual ~j_calc_symbol();
	virtual j_expression* as_expression();
	Type_Syntax& type_syntax()const;
	void set_type_syntax(const Type_Syntax& irk_type_syntax);
	void set_type_syntax(Type_Syntax* i_type_syntax);

	virtual j_calc_symbol* convert_to_type(const Type_Syntax& irk_type)const;

	virtual bool is_placeholder()const;

	bool has_type_syntax()const;

	virtual j_calc_symbol* make_non_referenced()const;

	

	void set_symbol_scope(J_Symbol_Scope* i_symbol_scope);

	virtual void process() = 0;

	virtual const Type_Syntax& return_type_syntax()const;
	
	J_Symbol_Scope& symbol_scope()const;



protected:
	virtual j_value derived_get_value(const Arguments& i_args)const = 0;
	
	j_calc_symbol(const j_calc_symbol& irk_symbol);
	j_calc_symbol(j_calc_symbol&& irr_symbol);


	j_calc_symbol* get_symbol_from_scope(const J_UI_String& irk_string)const;
private:
	J_Symbol_Scope* M_symbol_scope = gk_default_symbol_scope;


	



	
	virtual void alert_symbol_scope_set() = 0;

	Arguments* M_arguments;
	Type_Syntax* M_type = nullptr;
	J_Symbol_Identifier* M_name;
};



}

#endif //J_CALC_SYMBOL_H


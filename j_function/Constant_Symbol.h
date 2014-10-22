#ifndef CONSTANT_SYMBOL_H
#define CONSTANT_SYMBOL_H

#include "j_expression.h"
//
#include "parser/location.hh"

namespace jomike{
class Constant_Symbol : public j_expression{
public:
	Constant_Symbol(Symbol_Types i_symbol_type, const yy::location& i_loc);
	
	Constant_Symbol* get_copy()const override = 0;
	Constant_Symbol* move_copy()override = 0;

	void process()override;
private:

	void alert_symbol_scope_set()override;
};


class Int_Constant_Symbol : public Constant_Symbol{
public:
	Int_Constant_Symbol(int i_val, const yy::location& i_loc);

	Int_Constant_Symbol* get_copy()const override;

	Int_Constant_Symbol* move_copy()override;

	void clear()override;
	bool has_value()const override;

	J_UI_String get_display_name()override;

	j_symbol* convert_to_type(const Type_Syntax& irk_type)const override;

	void set_value(j_value i_value)override{
		M_value = i_value.as_llint();
	}

private:
	j_llint M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};


class Bool_Constant_Symbol : public Constant_Symbol{
public:
	Bool_Constant_Symbol(bool i_val, const yy::location& i_loc);

	Bool_Constant_Symbol* get_copy()const override;

	Bool_Constant_Symbol* move_copy()override;

	J_UI_String get_display_name()override;

	void clear()override;
	bool has_value()const override;

private:
	bool M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};



class Dbl_Constant_Symbol : public Constant_Symbol{
public:
	Dbl_Constant_Symbol(j_dbl i_val, const yy::location& i_loc);

	Dbl_Constant_Symbol* get_copy()const override;

	Dbl_Constant_Symbol* move_copy()override;

	J_UI_String get_display_name()override;

	void clear()override;
	bool has_value()const override;

private:
	j_dbl M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};

class String_Constant_Symbol : public Constant_Symbol{
public:
	String_Constant_Symbol(const std::string& irk_val, const yy::location& i_loc);

	String_Constant_Symbol* get_copy()const override;

	String_Constant_Symbol* move_copy()override;

	J_UI_String get_display_name()override;

	void clear()override;
	bool has_value()const override;

private:
	std::string M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};

}

#endif //CONSTANT_SYMBOL_H



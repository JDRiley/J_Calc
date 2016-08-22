#ifndef J_CONSTANT_SYMBOL_H
#define J_CONSTANT_SYMBOL_H

#include "j_expression.h"


namespace jomike{
class J_Constant_Symbol : public j_expression{
public:
	using j_expression::j_expression;
	
	J_Constant_Symbol* get_copy()const override = 0;
	J_Constant_Symbol* move_copy()override = 0;

	void process()override;
private:

	void alert_symbol_scope_set()override;
};


class Int_Constant_Symbol : public J_Constant_Symbol{
public:
	Int_Constant_Symbol(const yy::location& i_loc, int i_val);

	Int_Constant_Symbol* get_copy()const override;

	Int_Constant_Symbol* move_copy()override;

	void clear()override;
	bool has_value()const override;

	jc_string_t get_display_name()override;

	j_calc_symbol* convert_to_type(const Type_Syntax& irk_type)const override;

	void set_value(j_value i_value)override{
		M_value = i_value.as_llint();
	}

private:
	j_llint M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};


class Bool_Constant_Symbol : public J_Constant_Symbol{
public:
	Bool_Constant_Symbol(const yy::location& i_loc, bool i_val);

	Bool_Constant_Symbol* get_copy()const override;

	Bool_Constant_Symbol* move_copy()override;

	jc_string_t get_display_name()override;

	void clear()override;
	bool has_value()const override;

private:
	bool M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};



class Dbl_Constant_Symbol : public J_Constant_Symbol{
public:
	Dbl_Constant_Symbol(const yy::location& i_loc, j_dbl i_val);

	Dbl_Constant_Symbol* get_copy()const override;

	Dbl_Constant_Symbol* move_copy()override;

	jc_string_t get_display_name()override;

	void clear()override;
	bool has_value()const override;

private:
	j_dbl M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};

class jcs_string_constant : public J_Constant_Symbol{
public:
	jcs_string_constant(const yy::location& i_loc, const jc_string_t& irk_val);

	jcs_string_constant* get_copy()const override;

	jcs_string_constant* move_copy()override;

	jc_string_t get_display_name()override;

	void clear()override;
	bool has_value()const override;

private:
	jc_string_t M_value;
	J_Unit M_unit;
	j_value derived_get_value(const Arguments& i_args)const override;

};

}

#endif //J_CONSTANT_SYMBOL_H



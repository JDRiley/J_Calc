#ifndef J_COMPOSITE_SYMBOL_H
#define J_COMPOSITE_SYMBOL_H

#include "j_symbol.h"


namespace jomike{
class Symbol_Joiner;
class j_composite_symbol : public j_symbol{
public:
	j_composite_symbol(const j_composite_symbol&);
	j_composite_symbol();

	bool has_value()const override;
	enum class Join_Type{ ADD, MULTIPLY, DIVIDE, SUBTRACT, ERROR_TYPE };
	void append(Join_Type, const j_symbol&);

	j_size_t size()const;

	J_UI_String get_display_name()override;
	j_composite_symbol* get_copy()const override;

	j_symbol& operator[](j_size_t i_index);

	const j_symbol& operator[](j_size_t i_index)const;

	//Destruction
	~j_composite_symbol();
	void clear()override;
protected:
	j_value derived_get_value(const Arguments&)const override;
private:
	ex_array<Symbol_Joiner*> M_symbol_joiners;
	ex_array<j_symbol*> M_symbols;

};

//Useful Functions
j_composite_symbol::Join_Type get_composite_symbol_join_type(J_UI_Char i_char);


}

#endif //J_COMPOSITE_SYMBOL_H


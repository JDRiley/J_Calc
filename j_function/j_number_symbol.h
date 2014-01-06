#ifndef J_NUMBER_SYMBOL_H
#define J_NUMBER_SYMBOL_H

#include "j_symbol.h"


namespace jomike{
class j_number_symbol : public j_symbol{
public:
	//Constructors
	j_number_symbol(Dbl_t);
	j_number_symbol(const J_UI_String&);

	j_number_symbol* get_copy()const override;

	//Status
	bool has_value()const override;

	J_UI_String get_display_name()override;

	//Reset
	void clear()override;
protected:
	j_value derived_get_value(const Arguments&)const override;
private:
	bool M_value_status;
	Dbl_t M_value;
};


}

#endif //J_NUMBER_SYMBOL_H


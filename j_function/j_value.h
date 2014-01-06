#ifndef J_VALUE_H
#define J_VALUE_H

#include "J_Symbol_Fwd_Decl.h"
//
#include "j_unit.h"
//
#include <J_Error.h>

namespace jomike{

class j_value{
public:
	//Constructors
	j_value(Dbl_t, J_Unit);
	j_value(const j_value&, J_Unit);

	const j_value& operator+(const j_value&)const;

	Dbl_t value()const;

	bool value_status()const;

	//Math
	j_value& operator+=(const j_value&);

	bool operator==(const j_value&)const;
	bool operator!=(const j_value&)const;
	bool is_convertible(J_Unit)const;
	j_value& convert_units(J_Unit);
private:
	J_Unit M_units;
	Dbl_t M_value;
	bool M_value_status = false;
};

j_dbl unit_conversion(const j_value&, J_Unit);

class J_Value_Error : public J_Error{
public:
	J_Value_Error(const char * const ik_message = "NO Value in symbol");
private:
};

const j_value J_VALUE_TRUE_BOOLEAN = j_value(1, J_Unit(J_Unit_Types::BOOLEAN));
const j_value J_VALUE_FALSE_BOOLEAN = j_value(0, J_Unit(J_Unit_Types::BOOLEAN));

}

#endif //J_VALUE_H


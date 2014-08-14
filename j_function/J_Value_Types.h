#ifndef J_VALUE_TYPES_H
#define J_VALUE_TYPES_H

#include "j_value.h"


namespace jomike{

typedef j_value::Value_Types J_Value_Types;

bool int_upcastable(J_Value_Types i_type);
bool dbl_upcastable(J_Value_Types i_type);
bool bool_upcastable(J_Value_Types i_type);
bool str_upcastable(J_Value_Types i_type);
}

#endif //J_VALUE_TYPES_H



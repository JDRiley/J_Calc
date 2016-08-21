#ifndef UNIT_CONVERTER_H
#define UNIT_CONVERTER_H

#include <J_Fwd_Decl.h>
//
#include "j_unit.h"

namespace jomike{

class Unit_Converter{
public:
	virtual j_dbl convert_time(j_dbl, J_Unit)const  = 0;
	virtual j_dbl convert_scalar(j_dbl, J_Unit)const = 0;
	virtual j_dbl convert_mass(j_dbl, J_Unit)const = 0;
	virtual j_dbl convert_distance(j_dbl, J_Unit)const = 0;
private:
};


Unit_Converter* get_converter(J_Unit);

}

#endif //UNIT_CONVERTER_H


#include "Unit_Converter.h"
//
#include <cassert>

namespace jomike{


class Unit_Converter_Impl : public Unit_Converter{
public:
	virtual j_dbl convert_time(j_dbl, J_Unit)const;
	virtual j_dbl convert_scalar(j_dbl, J_Unit)const;
	virtual j_dbl convert_mass(j_dbl, J_Unit)const;
	virtual j_dbl convert_distance(j_dbl, J_Unit)const;
private:
	J_Unit M_units;

};

j_dbl Unit_Converter_Impl::convert_time(j_dbl, J_Unit)const{
	assert(0);
	return 0;
}

j_dbl Unit_Converter_Impl::convert_scalar(j_dbl, J_Unit)const{
	assert(!"The method or operation is not implemented.");
	assert(!"The method or operation is not yet check.");
	assert(!"The method or operation is not yet check.");
	return 0;
}

j_dbl Unit_Converter_Impl::convert_mass(j_dbl, J_Unit)const{
	assert(!"The method or operation is not implemented.");
	assert(!"The method or operation is not yet check.");
	assert(!"The method or operation is not yet check.");
	return 0;
}

j_dbl Unit_Converter_Impl::convert_distance(j_dbl, J_Unit)const{
	assert(!"The method or operation is not implemented.");
	assert(!"The method or operation is not yet check.");
	assert(!"The method or operation is not yet check.");
	return 0;
}

}




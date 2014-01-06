#include "j_value.h"
#include "Unit_Converter.h"


namespace jomike{
	

j_value::j_value(Dbl_t i_val, J_Unit i_units):M_value(i_val), M_units(i_units)
	, M_value_status(true){}

j_value::j_value(const j_value& irk_source, J_Unit i_unit):M_units(i_unit)
, M_value(irk_source.M_value), M_value_status(irk_source.value_status()){
	convert_units(M_units);
}



j_value& j_value::operator+=(const j_value& i_val){
	//Need to do unit like things here
	M_value += i_val.M_value;
	return *this;
}

Dbl_t j_value::value()const{ 
	if(!M_value_status){
		throw J_Value_Error("J_Value is invalid");
	}

	return M_value; 
}


bool j_value::operator==(const j_value& irk_right)const{
	return value() == irk_right.value();
}

bool j_value::value_status()const{
	return M_value_status;
}

bool j_value::operator!=(const j_value& irk_right)const{
	return !(*this == irk_right);
}

j_value& j_value::convert_units(J_Unit i_unit){
	if(!M_value_status){
		M_units = i_unit;
		return *this;
	}

	if(!is_convertible(i_unit)){
		throw J_Argument_Error("Cannot Convert Units");
	}

	M_value *= (M_units.total_factor() / i_unit.total_factor());
	
	M_units = i_unit;

	return *this;
}

bool j_value::is_convertible(J_Unit i_unit)const{
	return M_units.is_convertible(i_unit);
}

J_Value_Error::J_Value_Error(const char* const ik_message):J_Error(ik_message){}




}




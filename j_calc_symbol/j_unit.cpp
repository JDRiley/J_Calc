#include "j_unit.h"
//
#include <cassert>

namespace jomike{



J_Unit::J_Unit(J_Unit_Types i_unit_type){
	M_scalar_unit = J_Unit::Scalar_Unit::ONE;
	M_time_unit = J_Unit::Time_Unit::NONE;
	M_distance_unit = J_Unit::Distance_Unit::NONE;
	M_mass_unit = J_Unit::Mass_Unit::NONE;

	switch(i_unit_type){
	case J_Unit_Types::DEFAULT:
		break;
	case J_Unit_Types::BOOLEAN:
		M_scalar_unit = J_Unit::Scalar_Unit::BOOL;
	default:
		assert("Predefined unit type does not exists!");
	}
}

j_dbl J_Unit::time_factor()const{
	switch(M_time_unit){
	case J_Unit::Time_Unit::NONE:
		return 1;
	case J_Unit::Time_Unit::SECONDS:
		return 1;
	case J_Unit::Time_Unit::MILLISECONDS:
		return 0.001;
	case J_Unit::Time_Unit::YEARS:
		return 365 * 24 * 60 * 60;
	case J_Unit::Time_Unit::MINUTES:
		return 60;
	case J_Unit::Time_Unit::HOURS:
		return 60 * 60;
	default:
		assert(!"Undefined Time Factor");
		return 0;
	}
}

j_dbl J_Unit::scale_factor()const{
	switch(M_scalar_unit)	{
	case J_Unit::Scalar_Unit::ONE:
		return 1;
	case J_Unit::Scalar_Unit::MOL:
		return 6.234e23;
	case J_Unit::Scalar_Unit::BOOL:
		return 1;
	default:
		assert(!"Undefined Scale Factor");
		return 0;
	}
}

j_dbl J_Unit::mass_factor()const{
	switch(M_mass_unit)	{
	case J_Unit::Mass_Unit::NONE:
		return 1;
	case J_Unit::Mass_Unit::GRAMS:
		return 1e-3;
	case J_Unit::Mass_Unit::KILLIGRAMS:
		return 1;
	case J_Unit::Mass_Unit::POUNDS:
		return 0.453592;
	case J_Unit::Mass_Unit::STONES:
		return 6.35029;
	default:
		assert(!"Undefined Mass Factor");
		return 0;
	}
}

j_dbl J_Unit::distance_factor()const{
	switch(M_distance_unit)	{
	case J_Unit::Distance_Unit::NONE:
		return 1;
	case J_Unit::Distance_Unit::METERS:
		return 1;
	case J_Unit::Distance_Unit::CENTIMETERS:
		return 1e-2;
	case J_Unit::Distance_Unit::MILLIMETERS:
		return 1e-3;
	case J_Unit::Distance_Unit::MICROMETERS:
		return 1e-6;
	case J_Unit::Distance_Unit::ANGSTROMS:
		return 1e-10;
	default:
		assert(!"Undefined Distance Units");
		return 0;
	}
}

j_dbl J_Unit::total_factor()const{
	return time_factor()*scale_factor()*mass_factor()*distance_factor();
}

bool J_Unit::is_convertible(J_Unit i_unit)const{
	if(has_distance_unit() != i_unit.has_distance_unit()){
		return false;
	}

	if(has_time_unit() != i_unit.has_time_unit()){
		return false;
	}

	if(has_mass_unit() != i_unit.has_mass_unit()){
		return false;
	}
	return true;
}

bool J_Unit::has_distance_unit()const{
	return Distance_Unit::NONE != M_distance_unit;
}

bool J_Unit::has_time_unit()const{
	return Time_Unit::NONE != M_time_unit;
}

bool J_Unit::has_mass_unit()const{
	return Mass_Unit::NONE != M_mass_unit;
}

bool J_Unit::operator==(const J_Unit& irk_right)const{
	if(irk_right.M_scalar_unit != M_scalar_unit){
		return false;
	}

	if(irk_right.M_time_unit != M_time_unit){
		return false;
	}

	if(irk_right.M_distance_unit != M_distance_unit){
		return false;
	}

	if(irk_right.M_mass_unit != M_mass_unit){
		return false;
	}

	return true;
}

bool J_Unit::operator!=(const J_Unit& irk_right)const{
	return !(*this == irk_right);
}







}




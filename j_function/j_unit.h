#ifndef J_UNIT_H
#define J_UNIT_H

#include "J_Symbol_Fwd_Decl.h"


namespace jomike{

enum class J_Unit_Types{ DEFAULT, BOOLEAN };


class J_Unit{
public:
	enum class Scalar_Unit{ ONE, MOL, BOOL };
	enum class Time_Unit{ NONE, SECONDS, MILLISECONDS, YEARS, MINUTES, HOURS };
	enum class Distance_Unit{ NONE, METERS, CENTIMETERS, MILLIMETERS, MICROMETERS, ANGSTROMS };
	enum class Mass_Unit{ NONE, GRAMS, KILLIGRAMS, POUNDS, STONES };

	J_Unit(J_Unit_Types i_type = J_Unit_Types::DEFAULT);

	j_dbl time_factor()const;
	j_dbl scale_factor()const;
	j_dbl mass_factor()const;
	j_dbl distance_factor()const;
	j_dbl total_factor()const;

	bool is_convertible(J_Unit)const;
	bool has_distance_unit()const;
	bool has_time_unit()const;
	bool has_mass_unit()const;
private:
	Scalar_Unit M_scalar_unit;
	Time_Unit M_time_unit;
	Distance_Unit M_distance_unit;
	Mass_Unit M_mass_unit;
	



};



}

#endif //J_UNIT_H


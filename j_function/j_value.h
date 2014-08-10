#ifndef J_VALUE_H
#define J_VALUE_H

#include "J_Symbol_Fwd_Decl.h"
//
#include "j_unit.h"
//
#include <J_Error.h>
//
#include "Symbol_Types.h"
//
//
#include "j_numeric.h"
namespace jomike{

class j_value{
public:
	j_value();
	//Constructors
	j_value(Dbl_t, J_Unit);
	j_value(j_llint, J_Unit);
	j_value(j_int, J_Unit);
	j_value(bool, J_Unit);
	j_value(const std::string&, J_Unit);
	j_value(const j_value&, J_Unit);

	enum class Value_Types{LL_INTEGER, DOUBLE, BOOL, STRING, UNDEFINIED};

	const j_value& operator+(const j_value&)const;

	Dbl_t as_double()const;

	j_llint as_llint()const;

	bool as_bool()const;

	const std::string& as_string()const;

	bool value_status()const;

	J_Unit units()const;
	
	template<typename Num_t>
	Num_t as_type()const;

	template<>
	j_llint as_type<j_llint>()const{
		return as_llint();
	}

	template<>
	std::string as_type<std::string>()const{
		return as_string();
	}

	template<>
	const std::string& as_type<const std::string&>()const{
		return as_string();
	}
	

	std::string to_str()const;
	Symbol_Types symbol_type()const;
	//Math
	j_value& operator+=(const j_value&);
	j_value& operator-=(const j_value&);
	j_value& operator*=(const j_value&);
	j_value& operator/=(const j_value&);

	j_value operator-()const;

	bool operator==(const j_value&)const;
	bool operator!=(const j_value&)const;
	bool is_convertible(J_Unit)const;
	j_value& convert_units(J_Unit);

	Value_Types type()const;
private:
	J_Unit M_units;
	union Value_Union{
		j_llint llint_val;
		j_dbl dbl_val;
		std::string* str_val;
		bool bool_val;
	} M_val;

	template<class Operator_Class>
	void binary_value_operation(
		const j_value& i_right, Value_Union* i_value_union, const Operator_Class&);

	template<class Operator_Class>
	void binary_value_operation_no_str_or_bool(
		const j_value& i_right, Value_Union* i_value_union, const Operator_Class&);

	template<typename Ret_t, typename Left_t, typename Operator_Class>
	void binary_value_operation(
		const Left_t& i_left, const j_value& i_right, Ret_t* i_destination
		, const Operator_Class& i_func);
	
	Value_Types M_type;
	bool M_has_value_status = false;



	template<class Operator_Class>
	typename Operator_Class::return_type
		unary_value_operation(Operator_Class)const;
};

j_dbl unit_conversion(const j_value&, J_Unit);

class J_Value_Error : public J_Error{
public:
	J_Value_Error(const char * const ik_message = "NO Value in symbol");
private:
};

const j_value J_VALUE_TRUE_BOOLEAN = j_value(true, J_Unit(J_Unit_Types::BOOLEAN));
const j_value J_VALUE_FALSE_BOOLEAN = j_value(false, J_Unit(J_Unit_Types::BOOLEAN));

}

#endif //J_VALUE_H


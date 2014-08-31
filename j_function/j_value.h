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
#include "j_numeric.h"
//
#include <j_iterator.h>
namespace jomike{


j_value operator<(const j_value& irk_left, const j_value& irk_right);
j_value operator<=(const j_value& irk_left, const j_value& irk_right);
j_value operator>(const j_value& irk_left, const j_value& irk_right);
j_value operator>=(const j_value& irk_left, const j_value& irk_right);
j_value operator==(const j_value& irk_left, const j_value& irk_right);
j_value operator!=(const j_value& irk_left, const j_value& irk_right);

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

	

	enum class Value_Types{LL_INTEGER, DOUBLE, BOOL, STRING, VOID, UNDEFINIED};
	


	//Value_Union M_val;
	//J_Unit M_units;
	//Value_Types M_type;
	//bool M_has_value_status = false;

	//Deleting Constructors and Assignment Operators
	j_value(const j_value& irk_source);
	j_value(j_value&&);


	j_value& operator=(const j_value& irv_val);

	j_value& operator=(j_value&& irv_val);

	void swap(j_value& ir_val);

	const j_value& operator+(const j_value&)const;

	Dbl_t as_double()const;

	j_llint as_llint()const;

	bool as_bool()const;

	std::string as_string()const;

	bool value_status()const;

	J_Unit units()const;
	
	template<typename Val_t>
	typename std::remove_cv<typename std::remove_reference<Val_t>::type>::type
		as_type()const;

	template<>
	j_llint as_type<j_llint>()const{
		return as_llint();
	}

	template<>
	j_dbl as_type<j_dbl>()const{
		return as_double();
	}

	template<>
	bool as_type<bool>()const{
		return as_bool();
	}

	template<>
	std::string as_type<std::string>()const{
		return as_string();
	}

	template<>
	std::string as_type<const std::string&>()const{
		return as_string();
	}
	
	
	static const j_value& void_type();

	std::string to_str()const;
	Symbol_Types symbol_type()const;
	//Math
	j_value& operator+=(const j_value&);
	j_value& operator-=(const j_value&);
	j_value& operator*=(const j_value&);
	j_value& operator/=(const j_value&);

	j_value operator-()const;

	bool is_convertible(J_Unit)const;
	j_value& convert_units(J_Unit);

	Value_Types type()const;

	bool upcastable(Value_Types i_type)const;

	void upcast(Value_Types i_type);

	void convert_to_type(Value_Types i_type);

	void clear();
	~j_value();
private:
	
	j_value(Value_Types i_type);

	union Value_Union{
		j_llint llint_val;
		j_dbl dbl_val;
		std::string* str_val;
		bool bool_val;
	};

	Value_Union M_val;
	J_Unit M_units;
	Value_Types M_type;
	bool M_has_value_status = false;


	template<class Operator_Class>
	void binary_value_operation(
		const j_value& i_right, const Operator_Class&);

	template<class Operator_Class>
	void binary_value_operation_no_str_or_bool(
		const j_value& i_right, const Operator_Class&);

	template<class Operator_Class>
	void binary_value_operation_no_str(
		const j_value& i_right, const Operator_Class&);

	template<class Operator_Class>
	void binary_value_operation_no_bool(
		const j_value& i_right, const Operator_Class&);

	template<typename Ret_t, typename Left_t, typename Operator_Class>
	void binary_value_operation(
		const Left_t& i_left, const j_value& i_right, Ret_t* i_destination
		, const Operator_Class& i_func);
	
	


	template<typename Ret_t>
	Ret_t cast_to()const;

	template<>
	std::string cast_to()const;

	template<class Operator_Class>
	typename Operator_Class::return_type
		unary_value_operation(Operator_Class)const;



	friend j_value operator<(const j_value& irk_left, const j_value& irk_right);
	friend j_value operator<=(const j_value& irk_left, const j_value& irk_right);
	friend j_value operator>(const j_value& irk_left, const j_value& irk_right);
	friend j_value operator>=(const j_value& irk_left, const j_value& irk_right);
	friend j_value operator==(const j_value& irk_left, const j_value& irk_right);
	friend j_value operator!=(const j_value& irk_left, const j_value& irk_right);


};




j_dbl unit_conversion(const j_value&, J_Unit);

class J_Value_Error : public J_Error{
public:
	J_Value_Error(const char * const ik_message = "No Value in symbol");
private:
};

const j_value J_VALUE_TRUE_BOOLEAN = j_value(true, J_Unit(J_Unit_Types::BOOLEAN));
const j_value J_VALUE_FALSE_BOOLEAN = j_value(false, J_Unit(J_Unit_Types::BOOLEAN));

}

#endif //J_VALUE_H


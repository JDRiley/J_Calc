#include "j_value.h"
#include "Unit_Converter.h"
//
#include <j_type.h>
//
#include "J_Symbol_Error.h"
//
#include <cstdlib>
using std::equal;
using std::string;
using std::to_string;
namespace jomike{

j_value::j_value(j_llint i_val, J_Unit i_unit){
	M_type = Value_Types::LL_INTEGER;
	M_val.llint_val = i_val;
	M_units = i_unit;
	M_has_value_status = true;
}

j_value::j_value(j_int i_val, J_Unit i_unit){
	M_type = Value_Types::LL_INTEGER;
	M_val.llint_val = i_val;
	M_units = i_unit;
	M_has_value_status = true;
}

j_value::j_value(j_dbl i_val, J_Unit i_unit){
	M_type = Value_Types::DOUBLE;
	M_val.dbl_val = i_val;
	M_has_value_status = true;
	M_units = i_unit;
}


j_value::j_value(bool i_val, J_Unit i_unit){
	M_type = Value_Types::BOOL;
	M_val.bool_val = i_val;
	M_has_value_status = true;
	M_units = i_unit;
}

j_value::j_value(const string& i_val, J_Unit i_unit){
	M_type = Value_Types::STRING;
	M_val.str_val = new string(i_val);
	M_has_value_status = true;
	M_units = i_unit;
}



template<typename Ret_t, typename Left_t, typename Operator_Class>
void j_value::binary_value_operation(
	const Left_t& i_left, const j_value& i_right, Ret_t* i_destination
	, const Operator_Class& i_func){


	j_value::Value_Types right_type = i_right.M_type;
	assert(j_value::Value_Types::STRING != i_right.type());

		switch(right_type){
		case j_value::Value_Types::LL_INTEGER:
			i_func(i_left, i_right.as_llint(), i_destination);
			break;
		case j_value::Value_Types::DOUBLE:
			i_func(i_left, i_right.as_double(), i_destination);
			break;
		case j_value::Value_Types::BOOL:
			assert(0);
			break;
		case j_value::Value_Types::STRING:
			assert(0);
			break;
		default:
			break;
		}

}


template<typename Operator_Class>
void j_value::binary_value_operation(
	const j_value& i_right, j_value::Value_Union* i_value_union
	, const Operator_Class& i_func){
	switch(M_type){
	case Value_Types::LL_INTEGER:
		binary_value_operation(
			M_val.llint_val, i_right, &i_value_union->llint_val, i_func);
		break;
	case Value_Types::DOUBLE:
		binary_value_operation(
			M_val.dbl_val, i_right, &i_value_union->dbl_val, i_func);
		break;
	case Value_Types::BOOL:
		binary_value_operation(
			M_val.bool_val, i_right, &i_value_union->bool_val, i_func);
		break;
	case Value_Types::STRING:
		assert(i_right.type() == Value_Types::STRING);
		i_func(*M_val.str_val, i_right.as_string(), i_value_union->str_val);
		break;
	default:
		break;
	}
}


template<typename Operator_Class>
void j_value::binary_value_operation_no_str_or_bool(
	const j_value& i_right, j_value::Value_Union* i_value_union
	, const Operator_Class& i_func){
	switch(M_type){
	case Value_Types::LL_INTEGER:
		binary_value_operation(
			M_val.llint_val, i_right, &i_value_union->llint_val, i_func);
		break;
	case Value_Types::DOUBLE:
		binary_value_operation(
			M_val.dbl_val, i_right, &i_value_union->dbl_val, i_func);
		break;
	case Value_Types::BOOL:
		throw J_Value_Error("Bool in wrong binary value_operation function");
		break;
	case Value_Types::STRING:
		throw J_Value_Error("String in wrong binary value_operation function");
		break;
	default:
		break;
	}
}



j_value::j_value(const j_value& irk_source, J_Unit i_unit):M_units(i_unit)
, M_val(irk_source.M_val), M_has_value_status(irk_source.value_status()){
	convert_units(M_units);
}

j_value::j_value(){
	M_type = Value_Types::UNDEFINIED;
	M_has_value_status = false;
}

template<typename Left_t, typename Right_t>
Left_t addition(const Left_t& i_left, const Right_t& i_right){
	return i_left + i_right;
}



class Addition_Class{
public:
	template<typename Ret_t, typename Left_t, typename Right_t>
	void operator()(const Left_t& i_left, const Right_t& i_right, Ret_t* i_destination)const{
		*i_destination = static_cast<Ret_t>(i_left + i_right);
	}
};


j_value& j_value::operator+=(const j_value& irk_val){
	//Need to do unit like things here
	assert(Value_Types::STRING != M_type);
	assert(Value_Types::STRING != irk_val.M_type);
	


	binary_value_operation(irk_val, &M_val, Addition_Class());
	return *this;
}

class Subtraction_Class{
public:
	template<typename Ret_t, typename Left_t, typename Right_t>
	void operator()(const Left_t& i_left, const Right_t& i_right, Ret_t* i_destination)const{
		*i_destination = static_cast<Ret_t>(i_left - i_right);
	}
};

j_value& j_value::operator-=(const j_value& irk_val){
	//Need to do unit like things here
	assert(Value_Types::STRING != M_type);
	assert(Value_Types::STRING != irk_val.M_type);



	binary_value_operation_no_str_or_bool(irk_val, &M_val, Subtraction_Class());
	return *this;
}

class Multiplication_Class{
public:
	template<typename Ret_t, typename Left_t, typename Right_t>
	void operator()(const Left_t& i_left, const Right_t& i_right, Ret_t* i_destination)const{
		*i_destination = static_cast<Ret_t>(i_left * i_right);
	}
};

j_value& j_value::operator*=(const j_value& irk_val){
	//Need to do unit like things here
	assert(Value_Types::STRING != M_type);
	assert(Value_Types::STRING != irk_val.M_type);



	binary_value_operation_no_str_or_bool(irk_val, &M_val, Multiplication_Class());
	return *this;
}
class Division_Class{
public:
	template<typename Ret_t, typename Left_t, typename Right_t>
	void operator()(const Left_t& i_left, const Right_t& i_right, Ret_t* i_destination)const{
		if(!i_right){
			throw J_Value_Error("Division By Zero");
		}
		*i_destination = static_cast<Ret_t>(i_left / i_right);
	}
};

j_value& j_value::operator/=(const j_value& irk_val){
	//Need to do unit like things here
	assert(Value_Types::STRING != M_type);
	assert(Value_Types::STRING != irk_val.M_type);

	assert(Value_Types::BOOL != M_type);

	binary_value_operation_no_str_or_bool(irk_val, &M_val, Division_Class());
	return *this;
}

bool j_value::operator==(const j_value& irk_right)const{
	if(M_type != irk_right.M_type){
		return false;
	}

	const char* val_address = reinterpret_cast<const char*>(&M_val);
	const char* right_val_address = reinterpret_cast<const char*>(&irk_right.M_val);
	return equal(val_address, val_address + sizeof(M_val), right_val_address);
}

bool j_value::value_status()const{
	return M_has_value_status;
}

bool j_value::operator!=(const j_value& irk_right)const{
	return !(*this == irk_right);
}

j_value& j_value::convert_units(J_Unit i_unit){
	if(!M_has_value_status){
		M_units = i_unit;
		return *this;
	}

	if(!is_convertible(i_unit)){
		throw J_Argument_Error("Cannot Convert Units");
	}

	//M_val *= (M_units.total_factor() / i_unit.total_factor());
	assert(0);
	M_units = i_unit;

	return *this;
}

bool j_value::is_convertible(J_Unit i_unit)const{
	return M_units.is_convertible(i_unit);
}

j_value::Value_Types j_value::type()const{
	return M_type;
}

const std::string& j_value::as_string()const{
	return *M_val.str_val;
}

bool j_value::as_bool()const{
	return M_val.bool_val;
}

j_llint j_value::as_llint()const{

	if(!M_has_value_status){
		throw J_Symbol_Error("Value uninitialized");
	}
	switch(M_type){
	case j_value::Value_Types::LL_INTEGER:
		return M_val.llint_val;
	case j_value::Value_Types::DOUBLE:
		return static_cast<j_llint>(M_val.dbl_val);
	case j_value::Value_Types::BOOL:
		return M_val.bool_val ? 1ll : 0ll;
	case j_value::Value_Types::STRING:{
		if(M_val.str_val->empty()){
			return 0ll;
		}
		char* str_pos = nullptr;
		return std::strtoll(M_val.str_val->data(), &str_pos, 0);
	}
	case j_value::Value_Types::UNDEFINIED:
		assert(!"Type Not Identified but declared to have value");
	default:
		assert(!"Unknown Type");
		return 0ll;
	}


}

Dbl_t j_value::as_double()const{
	return M_val.dbl_val;
}


template<class Operator_Class>
typename Operator_Class::return_type j_value::unary_value_operation(Operator_Class i_class)const{
	switch(M_type){
	case Value_Types::LL_INTEGER:
		return i_class(M_val.llint_val);
	case Value_Types::DOUBLE:
		return i_class(M_val.dbl_val);
	case Value_Types::BOOL:
		return i_class(M_val.bool_val);
	case Value_Types::STRING:
		return i_class(*M_val.str_val);
	default:
		assert(!"Undefined value type");
		return i_class(0);
	}
}


class To_String_Class{
public:
	typedef string return_type;

	

	template<typename Val_t>
	return_type operator()(const Val_t& irk_val)const{
		return to_string(irk_val);
	}

	template<>
	return_type operator()(const string& irk_val)const{
		return irk_val;
	}

	template<>
	return_type operator()(const bool& irk_val)const{
		return irk_val ? "true" : "false";
	}
};

string j_value::to_str()const{


	return unary_value_operation(To_String_Class());
}


class Unary_Negate_Class{
public:
	typedef j_value return_type;



	template<typename Val_t>
	typename std::enable_if<std::is_arithmetic<Val_t>::value, return_type>::type 
		operator()(const Val_t& irk_val)const{
			return j_value(-irk_val, J_Unit());
	}

	template<typename Val_t>
	typename std::enable_if<!std::is_arithmetic<Val_t>::value, return_type>::type  operator()(const Val_t& /*irk_val*/)const{
		throw J_Value_Error("Cannot Negate Non Arithmetic type");
	}

	
	j_value operator()(const bool& /*i_val*/)const{
		throw J_Value_Error("Cannot Negate Boolean Type");
	}


};

j_value j_value::operator-()const{
	j_value value(unary_value_operation(Unary_Negate_Class()));
	value.M_units = M_units;
	return value;
}


Symbol_Types j_value::symbol_type()const{
	switch(M_type)	{
	case Value_Types::LL_INTEGER:
		return Symbol_Types::INT;
	case Value_Types::DOUBLE:
		return Symbol_Types::DOUBLE;
	case Value_Types::BOOL:
		return Symbol_Types::BOOL;
	case Value_Types::STRING:
		return Symbol_Types::STRING;
	case Value_Types::UNDEFINIED:
		return Symbol_Types::EXPRESSION_TYPE_UNINITIALIZED;
	default:
		assert(!"Unknown Symbol Type");
		return Symbol_Types(0);
	}
}

J_Unit j_value::units()const{
	return M_units;
}

J_Value_Error::J_Value_Error(const char* const ik_message):J_Error(ik_message){}




}




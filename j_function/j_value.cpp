#include "j_value.h"
#include "Unit_Converter.h"
//
#include "J_Value_Types.h"
//
#include <j_type.h>
//
#include "J_Symbol_Error.h"
//
#include <cstdlib>
//
#include <J_String.h>
//
#include "Operators.h"
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
	assert(M_type == right_type);
	(void)right_type;

	assert(j_value::Value_Types::STRING != i_right.type());

	i_func(i_left, i_right.as_type<Left_t>(), i_destination);

	//switch(right_type){
	//case j_value::Value_Types::LL_INTEGER:
	//	i_func(i_left, i_right.as_type<Left_t>(), i_destination);
	//	break;
	//case j_value::Value_Types::DOUBLE:
	//	i_func(i_left, i_right.as_double(), i_destination);
	//	break;
	//case j_value::Value_Types::BOOL:
	//	assert(0);
	//	break;
	//case j_value::Value_Types::STRING:
	//	assert(0);
	//	break;
	//default:
	//	break;
	//}

}

static void convert_to_same_type(j_value* i_first_val, j_value* i_second_val){
	if(i_first_val->upcastable(i_second_val->type())){
		
		i_first_val->upcast(i_second_val->type());

	} else if(i_second_val->upcastable(i_first_val->type())){
		
		i_second_val->upcast(i_first_val->type());

	} else if(i_first_val->type() != i_second_val->type()){
		throw J_Value_Error("Cannot convert Value to Same type in convert_to_same_type function");
	}

	return;
}


template<typename Operator_Class>
void j_value::binary_value_operation(
	const j_value& i_right, const Operator_Class& i_func){
	
	j_value right_val(i_right);
	
	if(type() != right_val.type()){
		convert_to_same_type(this, &right_val);
	}

	switch(M_type){
	case Value_Types::LL_INTEGER:
		binary_value_operation(
			M_val.llint_val, i_right, &M_val.llint_val, i_func);
		break;
	case Value_Types::DOUBLE:
		binary_value_operation(
			M_val.dbl_val, i_right, &M_val.dbl_val, i_func);
		break;
	case Value_Types::BOOL:
		binary_value_operation(
			M_val.bool_val, i_right, &M_val.bool_val, i_func);
		break;
	case Value_Types::STRING:
		assert(i_right.type() == Value_Types::STRING);
		i_func(*M_val.str_val, i_right.as_string(), M_val.str_val);
		break;
	default:
		break;
	}
}

//This function is to prevent compilation errors for non arithmetic type Operator_Classes. Such as
//division and multiplication
template<typename Operator_Class>
void j_value::binary_value_operation_no_str_or_bool(
	const j_value& i_right, const Operator_Class& i_func){

	j_value right_val(i_right);

	if(type() != right_val.type()){
		convert_to_same_type(this, &right_val);
	}




	switch(M_type){
	case Value_Types::LL_INTEGER:
		binary_value_operation(
			M_val.llint_val, i_right, &M_val.llint_val, i_func);
		break;
	case Value_Types::DOUBLE:
		binary_value_operation(
			M_val.dbl_val, i_right, &M_val.dbl_val, i_func);
		break;
	case Value_Types::BOOL:
		throw J_Value_Error("Bool in wrong binary value_operation function");
	case Value_Types::STRING:
		throw J_Value_Error("String in wrong binary value_operation function");
	default:
		assert(!"Unhandled Value Type");
	}

}

//division and multiplication
template<typename Operator_Class>
void j_value::binary_value_operation_no_str(
	const j_value& i_right, const Operator_Class& i_func){

	j_value right_val(i_right);

	if(type() != right_val.type()){
		convert_to_same_type(this, &right_val);
	}




	switch(M_type){
	case Value_Types::LL_INTEGER:
		binary_value_operation(
			M_val.llint_val, i_right, &M_val.llint_val, i_func);
		break;
	case Value_Types::DOUBLE:
		binary_value_operation(
			M_val.dbl_val, i_right, &M_val.dbl_val, i_func);
		break;
	case Value_Types::BOOL:
		binary_value_operation(
			M_val.bool_val, i_right, &M_val.bool_val, i_func);
	case Value_Types::STRING:
		throw J_Value_Error("String in wrong binary value_operation function");
	default:
		assert(!"Unhandled Value Type");
	}

}
template<typename Operator_Class>
void j_value::binary_value_operation_no_bool(
	const j_value& irk_right, const Operator_Class& i_func){

	j_value right_val(irk_right);

	if(type() != right_val.type()){
		convert_to_same_type(this, &right_val);
	}




	switch(M_type){
	case Value_Types::LL_INTEGER:
		binary_value_operation(
			M_val.llint_val, irk_right, &M_val.llint_val, i_func);
		break;
	case Value_Types::DOUBLE:
		binary_value_operation(
			M_val.dbl_val, irk_right, &M_val.dbl_val, i_func);
		break;
	case Value_Types::BOOL:
		throw J_Value_Error("String in wrong binary value_operation function");
		
	case Value_Types::STRING:
		assert(irk_right.type() == Value_Types::STRING);
		i_func(*M_val.str_val, irk_right.as_string(), M_val.str_val);
		break;
	default:
		assert(!"Unhandled Value Type");
	}

}

j_value::j_value(const j_value& irk_source, J_Unit i_unit):j_value(irk_source){
	convert_units(i_unit);
}

j_value::j_value(){
	M_type = Value_Types::UNDEFINIED;
	M_has_value_status = false;
}

j_value::j_value(const j_value& irk_source)
: M_val(irk_source.M_val), M_units(irk_source.M_units), M_type(irk_source.M_type)
, M_has_value_status(irk_source.M_has_value_status){
	switch(M_type){
	case Value_Types::STRING:
		M_val.str_val = new string(irk_source.as_string());
	default:
		;
	}
}


j_value::j_value(j_value&& irv_src)
: M_val(irv_src.M_val), M_units(irv_src.M_units), M_type(irv_src.M_type)
, M_has_value_status(irv_src.M_has_value_status){
	M_type = Value_Types::LL_INTEGER;
	M_has_value_status = false;
	swap(irv_src);
}

j_value::j_value(Value_Types i_type):M_type(i_type){

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
	
	
	if(Value_Types::STRING != M_type){
		throw J_Value_Error("Cannot use \'+\' on strings yet");
	}
	assert(Value_Types::STRING != irk_val.M_type);
	

	
	binary_value_operation(irk_val, Addition_Class());
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



	binary_value_operation_no_str_or_bool(irk_val, Subtraction_Class());
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



	binary_value_operation_no_str_or_bool(irk_val,  Multiplication_Class());
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

	binary_value_operation_no_str_or_bool(irk_val, Division_Class());
	return *this;
}



bool j_value::value_status()const{
	return M_has_value_status;
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

std::string j_value::as_string()const{
	return cast_to<std::string>();
}

bool j_value::as_bool()const{
	return cast_to<bool>();
}

template<typename Ret_t>
Ret_t j_value::cast_to()const{

	if(!M_has_value_status){
		throw J_Symbol_Error("Value uninitialized");
	}
	switch(M_type){
	case j_value::Value_Types::LL_INTEGER:
		return static_cast<Ret_t>(M_val.llint_val);
	case j_value::Value_Types::DOUBLE:
		return static_cast<Ret_t>(M_val.dbl_val);
	case j_value::Value_Types::BOOL:
		return static_cast<Ret_t>(M_val.bool_val ? 1ll : 0ll);
	case j_value::Value_Types::STRING:{


		return from_string<Ret_t>(*M_val.str_val);
	}
	case j_value::Value_Types::UNDEFINIED:
		assert(!"Type Not Identified but declared to have value");
	default:
		assert(!"Unknown Type");
		return 0ll;
	}

}

template<>
std::string j_value::cast_to()const{
	switch(M_type){
	case j_value::Value_Types::LL_INTEGER:
		return to_string(M_val.llint_val);
	case j_value::Value_Types::DOUBLE:
		return to_string(M_val.dbl_val);
	case j_value::Value_Types::BOOL:
		return M_val.bool_val ? "true" : "false";
	case j_value::Value_Types::STRING:
		return *M_val.str_val;
	case j_value::Value_Types::UNDEFINIED:
		throw J_Value_Error("Undefined Value type");
	default:
		throw J_Value_Error("Could not convert type to string");
	}
}

j_llint j_value::as_llint()const{

	
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
	return cast_to<j_dbl>();
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
	if(Value_Types::VOID == M_type){
		return "void";
	}

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
	case Value_Types::VOID:
		return Symbol_Types::VOID_TYPE;
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

bool j_value::upcastable(Value_Types i_type)const{
	switch(M_type){
	case j_value::Value_Types::LL_INTEGER:
		return int_upcastable(i_type);
	case j_value::Value_Types::DOUBLE:
		return dbl_upcastable(i_type);
	case j_value::Value_Types::BOOL:
		return bool_upcastable(i_type);
	case j_value::Value_Types::STRING:
		return str_upcastable(i_type);
	case j_value::Value_Types::UNDEFINIED:
		return false;
	default:
		assert(!"Unknown Value_Type");
		return false;
	}
}

void j_value::upcast(Value_Types i_type){
	assert(upcastable(i_type));
	convert_to_type(i_type);
}

void j_value::convert_to_type(Value_Types i_type){
	if(!value_status()){
		throw J_Value_Error("Cannot convert j_value type when j_value has no value");
	}

	j_value temp_type(*this);
	J_Unit old_units = temp_type.units();
	clear();

	switch(i_type){
	case j_value::Value_Types::LL_INTEGER:
		(*this) = j_value(temp_type.as_llint(), old_units);
		break;
	case j_value::Value_Types::DOUBLE:
		(*this) = j_value(temp_type.as_double(), old_units);
		break;
	case j_value::Value_Types::BOOL:
		(*this) = j_value(temp_type.as_bool(), old_units);
		break;
	case j_value::Value_Types::STRING:
		(*this) = j_value(temp_type.as_string(), old_units);
		break;
	case j_value::Value_Types::UNDEFINIED:
	default:
		throw J_Value_Error("Could Not Convert j_value type");
	}

	assert(M_type == i_type);
	assert(units() == old_units);
}

void j_value::clear(){
	if(Value_Types::STRING == M_type){
		delete M_val.str_val;
	}
}

j_value::~j_value(){
	clear();
}

void j_value::swap(j_value& ir_val){
	std::swap(M_val, ir_val.M_val);
	std::swap(M_units, ir_val.M_units);
	std::swap(M_type, ir_val.M_type);
	std::swap(M_has_value_status, ir_val.M_has_value_status);
}

j_value& j_value::operator=(const j_value& irv_val){
	j_value temp(irv_val);
	swap(temp);
	return *this;
}

j_value& j_value::operator=(j_value&& irv_val){
	swap(irv_val);
	return *this;
}

const j_value& j_value::void_type(){
	static j_value void_value(Value_Types::VOID);
	void_value.M_has_value_status = true;
	return void_value;
}

J_Value_Error::J_Value_Error(const char* const ik_message):J_Error(ik_message){}


class Relational_Class{
public:

	Relational_Class(Operators i_operator):M_operator(i_operator){}
	template<typename Ret_t, typename Left_t, typename Right_t>
	void operator()(const Left_t& i_left, const Right_t& i_right, Ret_t* i_destination)const{
	

		switch(M_operator){
		case Operators::LESS:
			*i_destination = i_left < i_right;
			break;
		case Operators::LESS_EQUAL:
			*i_destination = i_left <= i_right;
			break;
		case Operators::GREATER:
			*i_destination = i_left > i_right;
			break;
		case Operators::GREATER_EQUAL:
			*i_destination = i_left >= i_right;
			break;
		default:
			assert(!"Bad Operator type for relational class");
		}
		
	}
private:
	Operators M_operator;
};

j_value operator<(const j_value& irk_left, const j_value& irk_right){
	j_value left_val(irk_left);

	left_val.binary_value_operation_no_bool(irk_right, Relational_Class(Operators::LESS));
	
	return j_value(left_val.as_bool(), J_Unit());
}


j_value operator<=(const j_value& irk_left, const j_value& irk_right){
	j_value left_val(irk_left);

	left_val.binary_value_operation_no_bool(irk_right, Relational_Class(Operators::LESS_EQUAL));

	return j_value(left_val.as_bool(), J_Unit());
}
j_value operator>(const j_value& irk_left, const j_value& irk_right){
	j_value left_val(irk_left);

	left_val.binary_value_operation_no_bool(irk_right, Relational_Class(Operators::GREATER));

	return j_value(left_val.as_bool(), J_Unit());
}
j_value operator>=(const j_value& irk_left, const j_value& irk_right){
	j_value left_val(irk_left);

	left_val.binary_value_operation_no_bool(irk_right, Relational_Class(Operators::GREATER_EQUAL));

	return j_value(left_val.as_bool(), J_Unit());
}


class Equality_Class{
public:

	Equality_Class(Operators i_operator):M_operator(i_operator){}

	template<typename Ret_t, typename Left_t, typename Right_t>
	void operator()(const Left_t& i_left, const Right_t& i_right, Ret_t* i_destination)const{


		switch(M_operator){
		case Operators::EQUAL:
			*i_destination = i_left == i_right;
			break;
		case Operators::NOT_EQUAL:
			*i_destination = i_left != i_right;
			break;
		default:
			assert(!"Bad Operator type for equality class");
		}

	}
private:
	Operators M_operator;
};
j_value operator==(const j_value& irk_left, const j_value& irk_right){
	j_value left_val(irk_left);

	left_val.binary_value_operation(irk_right, Equality_Class(Operators::EQUAL));

	return j_value(left_val.as_bool(), J_Unit());
}
j_value operator!=(const j_value& irk_left, const j_value& irk_right){
	j_value left_val(irk_left);

	left_val.binary_value_operation(irk_right, Equality_Class(Operators::NOT_EQUAL));

	return j_value(left_val.as_bool(), J_Unit());
}



}




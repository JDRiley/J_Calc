#ifndef MATH_BASE_H
#define MATH_BASE_H

#include <j_physical_constants.h>
//
#include <limits>
//
#include <utility>
//
#include <ex_array.h>
//
#include <iosfwd>
#include <exception>

namespace jtl = jomike;


#pragma comment(lib, "j_function" J_LIB_APPEND)

namespace jomike{

typedef double Dbl_t;
typedef double_arr_t Dbl_data_t;
typedef std::pair<Dbl_t, Dbl_t> dbl_pair;
typedef ex_array<dbl_pair> Dbl_pair_arr;

class j_function;

const dbl_pair g_default_pair(std::numeric_limits<Dbl_t>::min(), std::numeric_limits<Dbl_t>::max());
const Dbl_t DBL_T_PRINT_WIDTH = 10;

class Math_Base_Error: public std::exception{
public:
	Math_Base_Error(const char* const ik_message):M_msg(ik_message){}
	const char* what()const throw(){return M_msg;}
	virtual void print()const throw();
private:
	const char* const M_msg;
};

class Quit_Class : public Math_Base_Error{
public:
	Quit_Class(const char* const ik_message = "Terminating Quit Class Error Given")
		:Math_Base_Error(ik_message){}
private:
};

class Function_Error: public Math_Base_Error{
public:
	Function_Error(const char* const ik_message = "Function_Error")
		:Math_Base_Error(ik_message){
	}
private:
};

class Param_Error : public Function_Error{
public:
	Param_Error(Dbl_t i_param, int param_num
		, const char* const ik_message = "Invalid_Parameter")
		:Function_Error(ik_message), invalid_param(i_param)
		, param_index(param_num){}
	void print()const throw() override;
	int get_index()const throw(){return param_index;}
	Dbl_t get_param_val()const throw(){return invalid_param;}
private:
	Dbl_t invalid_param;
	int param_index;
};

class Argument_Error : public Function_Error{
public:
	Argument_Error(const char* const ik_message = "Bad Arguments given to function call!")
		:Function_Error(ik_message){}
};

class Unsupported_Function_Call : public Function_Error{
public:
	Unsupported_Function_Call(const char* const ik_message = "Unsupported_Function_Call Error")
		: Function_Error(ik_message){}
};

std::ostream& operator<<(std::ostream&, const jtl::Dbl_data_t&);

Dbl_t convolve_gauss(j_function* i_func, Dbl_t i_x);
}



#endif

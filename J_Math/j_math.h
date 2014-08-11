#ifndef J_MATH_H
#define J_MATH_H

#include <math_base.h>
//
#include "j_numeric.h"
//
#include <ex_array.h>
//Math
#include <cmath>

//Facilities
#include <future>

//Conatiners
#include <vector>



namespace jomike{

const Dbl_t GOLDEN_RATIO = (1+std::sqrt(5))/2.0;


class j_function;
int_arr_t range(int i_low, int i_high, int i_step = 1);

Dbl_data_t range(Dbl_t i_low, Dbl_t i_high, Dbl_t i_step = 1);
Dbl_data_t range_inclusive(Dbl_t i_low, Dbl_t i_high, Dbl_t i_step = 1);
void get_intervals(int* i_dest, int start,  int end,int i_num_threads);
Dbl_t j_integrate(const j_function* i_func, Dbl_t i_start, Dbl_t i_end
				   , int num_steps = 1000);



template<class Func_t, class Num_t>
Dbl_t j_golden_minimum(const Func_t& i_func, Num_t i_lower_bound, Num_t i_upper_bound){
	Dbl_t left_bound(i_lower_bound)
		, right_bound(i_upper_bound);
	if(i_upper_bound <= i_lower_bound){
		throw Argument_Error("Invalid Bounds for Min!");
	}
	auto first_midpoint_calc 
		= [&left_bound, &right_bound](){return right_bound - (right_bound - left_bound)/GOLDEN_RATIO;};
	auto second_midpoint_calc 
		= [&left_bound, &right_bound](){return left_bound + (right_bound - left_bound)/GOLDEN_RATIO;};
	
	Dbl_t midpoint_one = first_midpoint_calc();
	Dbl_t midpoint_two = second_midpoint_calc();

	Dbl_data_t func_vals;
	func_vals.push_back(i_func(left_bound));
	func_vals.push_back(i_func(midpoint_one));
	func_vals.push_back(i_func(midpoint_two));
	func_vals.push_back(i_func(right_bound));

	const Dbl_t k_target_accuracy = 1e-8;

	while((right_bound - left_bound) > k_target_accuracy){
		if(func_vals[1] < func_vals[2]){
			right_bound = midpoint_two;
			func_vals[3] = func_vals[2];

			midpoint_two = midpoint_one;
			func_vals[2] = func_vals[1];
			midpoint_one = first_midpoint_calc();
			func_vals[1] = i_func(midpoint_one);
		}else{
			left_bound = midpoint_one;
			func_vals[0] = func_vals[1];

			midpoint_one = midpoint_two;
			func_vals[1] = func_vals[2];

			midpoint_two = second_midpoint_calc();
			func_vals[2] = i_func(midpoint_two);
		}
		//cerr << "\nCur Min Geuss: " << 0.5*(midpoint_one+midpoint_two);
	}


	return 0.5*(midpoint_one+midpoint_two);
}

template<class Func_t, class Num_t>
Dbl_t j_golden_maximum(const Func_t& i_func, Num_t i_lower_bound, Num_t i_upper_bound){
	return j_golden_minimum([&i_func](Num_t i_x){return -i_func(i_x);}
	, i_lower_bound, i_upper_bound);
}
Dbl_t calculate_gaussian_quadrature_weight(const Dbl_data_t& irk_data,int i_index);


typedef j_llint(*ll_int_binary_function_t)(j_llint, j_llint);

ll_arr_t greatest_common_divisor(j_llint i_val_one, j_llint i_val_two);

j_llint greatest_common_divisor_simp(j_llint i_val_one, j_llint i_val_two);

j_llint least_common_multiple(j_llint i_val_one, j_llint i_val_two);



bool does_divide(int i_divisor, int i_number);

bool relative_prime(j_llint, j_llint);

bool is_prime(j_llint);

j_llint euler_func(j_llint);



}


#endif
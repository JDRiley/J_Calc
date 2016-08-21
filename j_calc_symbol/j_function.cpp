#include "j_function.h"
#include <j_math.h>
#include <Data_File.h>
#include <algorithm>
#include <numeric>
#include <functional>
#include <iterator>
#include <cstring>
#include <utility>
#include <cassert>
#include <utility>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <cmath>
#include <memory>
#include <sstream>
#include <vector>
#include <array>
#include <future>

using std::async;using std::future; using std::all_of;using std::abs;using std::array;using std::swap;using std::min;
using std::for_each; using std::accumulate;using namespace std::placeholders;using std::shared_ptr;
using std::copy; using std::bind; using std::back_inserter;using std::move;using std::transform;
using std::cref;using std::cerr;using std::endl;using std::string; using std::istream_iterator;
using std::cin;using std::cout; using std::istream; using std::move;using std::vector;
using std::ifstream;using std::cref;using std::mem_fn;using std::plus;using std::memset;using std::streamsize;
using std::numeric_limits;using std::ostringstream;using std::endl; using std::upper_bound;
using std::is_sorted; using std::greater; using std::less;using std::sort;using std::ceil; 
using std::floor;using std::minus;using std::divides;using namespace std::placeholders;using std::plus;
using std::ofstream; using std::copy; using std::map;
using std::to_string;

namespace jomike{

const int NUMBER_OF_THREADS = 8;

Dbl_t j_abs(Dbl_t i_x){
	return abs(i_x);
}

template<typename Iter>
Dbl_t get_minimum_val(Iter first, Iter last){
	Dbl_t cur_min = numeric_limits<Dbl_t>::max();
	Iter return_iter;
	while(first != last){
		if(cur_min > *first){
			cur_min = *first;
			return_iter = first;
		}
		++first;
	}
	return *return_iter;
}

void j_function::set_param(){
	throw Unsupported_Function_Call("Cannot Call: j_function::set_param()");
}

void j_function::partial_param_calc(Dbl_t* i_params, const Data_t& irk_data, int i_start, int i_end)const{
	for(int i=i_start; i < i_end; i++){
		function_param_calc(i_params, irk_data[0][i], irk_data[1][i]);
	}
}

Dbl_data_t j_function::function_param_calc_threaded(const Data_t& irk_data)const{
	
	Dbl_data_t params(number_of_parameters(),0.0);

	array<int,51> intervals;
	get_intervals(intervals.data(),0, irk_data.size(), NUMBER_OF_THREADS);	
	
	array<future<void>, NUMBER_OF_THREADS> results;
	const int MAX_PARAMS = 15;
	Dbl_t params_parts[NUMBER_OF_THREADS][MAX_PARAMS];

	memset(params_parts, 0, NUMBER_OF_THREADS*MAX_PARAMS*sizeof(Dbl_t));

	for(int i=0; i <NUMBER_OF_THREADS; i++){
		results[i] = async(&j_function::partial_param_calc,this, params_parts[i],
			irk_data, intervals[i], intervals[i+1]);
	}

	for(int i=0; i < NUMBER_OF_THREADS; i++){
		results[i].get();
		for(int b=0; b < number_of_parameters(); b++){
			params[b] += params_parts[i][b];
		}
	}

	return move(params);
}

Dbl_data_t j_function::function_param_calc(const Data_t& irk_data)const{
	Dbl_data_t params(number_of_parameters(),0.0);

	for(j_size_t i=0; i < irk_data[0].size(); i++){
		function_param_calc(params.data(), irk_data[0][i], irk_data[1][i]);
	}

	return move(params);
}

void j_function::function_param_calc(Dbl_t* ir_params, Dbl_t i_x, Dbl_t i_y)const{
	auto param_it = param_deriv_begin();
	for(int i=0; i < number_of_parameters(); i++){
		ir_params[i] -= (param_it[i]->eval(i_x)*2*(i_y-eval(i_x)));
	}
}

void j_composite_function::set(const Dbl_t* i_params){
	for(int i=0; i < size();  i++){
		M_functions[i]->set(i_params);
		i_params += (M_functions[i]->number_of_parameters());
	}
}

void j_composite_function::checked_set(const Dbl_t* i_params) 
	throw(Param_Error, Unsupported_Function_Call){
	for(int i=0; i < size(); i++){
		M_functions[i]->checked_set(i_params);
		i_params += (M_functions[i]->number_of_parameters());
	}
}

int j_composite_function::number_of_parameters()const{
	return accumulate(M_functions.begin(), M_functions.end(), int(0)
		,[](int first, const j_function* i_func){return first+=i_func->number_of_parameters();});
}

j_composite_function::~j_composite_function(){
	for_each(M_functions.begin(), M_functions.end(),[](j_function* i_func){delete i_func;});
}

Dbl_data_t j_composite_function::get_params()const{
	Dbl_data_t params;
	auto inserter = back_inserter(params);
	for(int i=0; i < size(); i++){
		Dbl_data_t temp_params = M_functions[i]->get_params();
		copy(temp_params.begin(), temp_params.end(), inserter);
	}
	return params;
}

j_composite_function::j_composite_function(const j_composite_function& irk_source){
	for_each(irk_source.M_functions.begin(), irk_source.M_functions.end()
		,bind(&j_composite_function::attach, this, _1));
	std::cerr << "\nComposite Function Copied...";
}

j_composite_function* j_composite_function::get_copy()const{
	return new j_composite_function(*this);
}

void j_composite_function::attach(const j_function* i_new_func){
	auto param_deriv_it = back_inserter(M_param_derivs);
	M_functions.push_back(i_new_func->get_copy());
	copy(M_functions.back()->param_deriv_begin(), M_functions.back()->param_deriv_end()
		, param_deriv_it);
}

Dbl_t j_composite_function::eval(Dbl_t i_x)const{
	Dbl_t result =  accumulate(M_functions.begin(), M_functions.end(), Dbl_t(0)
		,[i_x](Dbl_t total, j_function* i_func){ return (total+ i_func->eval(i_x));});
	//cerr << "Result: " << result << "\n";
	return result;
}

j_conv::j_conv(const j_conv& i_src):Wrapper_Function(*i_src.get_member())
	,M_impulse(i_src.M_impulse), M_step_size(i_src.M_step_size){
	std::cerr << "\nJ_Conv Copied...";
}

j_conv* j_conv::get_copy()const{
	return new j_conv(*this);
}

j_conv::j_conv(const j_function& irk_func, const Data_t& irk_impulse)
	:Wrapper_Function(irk_func), M_impulse(irk_impulse)
	, M_step_size(M_impulse[0][1]-M_impulse[0][0]){
		attach_impulse_data(irk_impulse);
	std::cerr << "\nJ_Conv Constructed...";
}

/*J_Conv self Interpolator*/
j_conv::j_conv(const j_function& irk_func, const Data_t& irk_impulse, Dbl_t i_step_size)
	:Wrapper_Function(irk_func), M_step_size(i_step_size){
	
	Data_t test_data(irk_impulse);
	Dbl_t min_val = get_minimum_val(irk_impulse[1].begin(), irk_impulse[1].end());
	
	transform(test_data[1].begin(), test_data[1].end(), test_data[1].begin()
		, bind(minus<Dbl_t>(), _1, min_val));

	

	Interpolation_Function interpolation_func(test_data, Evaluation_Methods::CUBIC_SPLINE);
	Dbl_t range_distance = interpolation_func.range().second - interpolation_func.range().first;

	Dbl_t max_x_pos = j_golden_maximum(interpolation_func
		, interpolation_func.range().first+0.1*range_distance
		, interpolation_func.range().second-range_distance*0.1);

	interpolation_func.offset_x_vals(-max_x_pos);
	streamsize old_precision = cout.precision(11);
	cout << "\nData was centered around point: " << max_x_pos;
	cout.precision(old_precision);
	Dbl_t range_of_interpolation = min(floor(abs(interpolation_func.range().first))
		, floor(abs(interpolation_func.range().second)));

	Dbl_data_t x_vals = range(-range_of_interpolation, range_of_interpolation, M_step_size);
	Dbl_data_t y_vals(x_vals.size());

	Dbl_t area_under_impulse = j_integrate(&interpolation_func, interpolation_func.range().first
		, interpolation_func.range().second);

	transform(x_vals.begin(), x_vals.end(), y_vals.begin()
		, bind(&Interpolation_Function::eval, interpolation_func, _1));

	transform(y_vals.begin(), y_vals.end(), y_vals.begin()
		, bind(divides<Dbl_t>(), _1, area_under_impulse));

	M_impulse = Data_t(x_vals, y_vals);
	ofstream data_file("laser_input.txt");
	M_impulse.print(data_file);
	std::cerr << "\nJ_Conv Constructed...";
}

Evaluation_Methods get_eval_method_enum(const string& irk_eval_method){
	if("linear" == irk_eval_method){
		return Evaluation_Methods::LINEAR;
	}else if("cubic_spline" == irk_eval_method){
		return Evaluation_Methods::CUBIC_SPLINE;
	}else{
		throw Math_Base_Error("Unknown Interplation Method Given!");
	}

}

void j_conv::attach_impulse_data(const Data_t& irk_data){
	M_impulse = irk_data;

}


/*single_slice(int)const*/
Dbl_t j_conv::single_slice(j_function* i_func, Dbl_t i_x, int i_cur_slice)const{
	return (1.0/3)*M_step_size*(i_func->eval((i_x + M_impulse[0][2*i_cur_slice-2]))
		*M_impulse[1][2*i_cur_slice-2]
	+ 4*(i_func->eval(i_x + M_impulse[0][2*i_cur_slice-1]))*M_impulse[1][2*i_cur_slice-1] 
		+ (i_func->eval(i_x + M_impulse[0][2*i_cur_slice]))*M_impulse[1][2*i_cur_slice]);
}

Dbl_t j_conv::single_param_slice(Dbl_t i_x, Dbl_t i_y, int i_cur_slice, int i_param)const{
	auto param_it = get_member()->param_deriv_begin();
	Dbl_t pos_one = i_x + M_impulse[0][2*i_cur_slice-2];
	Dbl_t pos_two = i_x + M_impulse[0][2*i_cur_slice-1];
	Dbl_t pos_three = i_x + M_impulse[0][2*i_cur_slice];
	return -2.0*(1.0/3)*M_step_size*(((param_it[i_param]->eval(pos_one))
		*(i_y - eval(pos_one)))*M_impulse[1][2*i_cur_slice-2]
	+ 4*(param_it[i_param]->eval(pos_two))*(i_y - eval(pos_two))*M_impulse[1][2*i_cur_slice-1] 
		+ (param_it[i_param]->eval(pos_three))*(i_y - eval(pos_three))
			*M_impulse[1][2*i_cur_slice]);
}

void j_conv::function_param_calc(Dbl_t* i_params, Dbl_t i_x, Dbl_t i_y)const{
	auto param_deriv_it = param_deriv_begin();
	for(int i=0; i < number_of_parameters(); i++){
		i_params[i] -= 2*convolve(param_deriv_it[i], i_x)*(i_y - eval(i_x));
		
	}
}

/*j_conv eval*/
Dbl_t j_conv::eval(Dbl_t i_x)const{
	return convolve(get_member(), i_x);
}

Dbl_t j_conv::partial_convolve(j_function* i_func, Dbl_t i_x, int i_start, int i_end)const{
	Dbl_t result = 0;
	for(int i=i_start; i < i_end; i++){
		result += single_slice(i_func, i_x, i);
	}
	return result;
}

Dbl_t j_conv::convolve_threaded(j_function* i_func,Dbl_t i_x)const{
	array<int,51> intervals;
	get_intervals(intervals.data(),1,M_impulse.size()/2+1, NUMBER_OF_THREADS);	
	
	array<future<Dbl_t>, NUMBER_OF_THREADS> results;
	for(size_t i=0; i < results.size(); i++){
		results[i] = async(&j_conv::partial_convolve,this, i_func, i_x
			, intervals[i], intervals[i+1]);

	}

	return accumulate(results.begin(), results.end()
		,0.0, bind(plus<Dbl_t>(),_1, bind(&future<Dbl_t>::get,_2)));
}

Dbl_t j_conv::convolve(j_function* i_func,Dbl_t i_x)const{
	Dbl_t result = 0.0;
	for(int i=1; i < M_impulse.size()/2+1; i++){
		result += single_slice(i_func, i_x, i);
	}

	return result;
}

class j_reliant_constant : public Reliant_Function{
public:
	//Construction
	j_reliant_constant(Dbl_t i_constant):M_constant(i_constant){}
	Dbl_t eval(Dbl_t)const override final{return M_constant;}
	int number_of_parameters()const override final{return 1;}
	Dbl_data_t get_params()const override final{return Dbl_data_t(1, M_constant);}
private:
	Dbl_t M_constant;
};

class j_reliant_line : public Reliant_Function{
public:
	j_reliant_line(Dbl_t i_slope, Dbl_t i_offset){
		M_params[0] = i_slope;
		M_params[1] = i_offset;
	}


	Dbl_t eval(Dbl_t i_x)const override final{
		return M_params[0]*i_x + M_params[1];
	}
	int number_of_parameters()const{return 2;}
	Dbl_data_t get_params()const{
		return Dbl_data_t(M_params.begin(), M_params.end());
	}
private:
	std::array<Dbl_t,2> M_params;
};

void Line::init_derivs(){
	M_param_funcs[0] = new j_reliant_line(1,0);
	M_param_funcs[1] = new j_reliant_constant(1);
}

void Line::set(const Dbl_t* i_params){
	copy(i_params, i_params+number_of_parameters(), M_params.begin());
}

Line::Line(Dbl_t i_slope, Dbl_t i_offset){
	M_params[0] = i_slope;
	M_params[1] = i_offset;
	init_derivs();
}

Line::Line(const Line& irk_src):M_params(irk_src.M_params){
	init_derivs();
	std::cerr << "\nLine Function Copied...";
}

Line::~Line(){
	for_each(M_param_funcs.begin(), M_param_funcs.end(), [](j_function* i_func){delete i_func;});
}

j_constant::j_constant(const j_constant& irk_src):M_constant(irk_src.M_constant){
	M_param_func = new j_reliant_constant(1);
	cerr << "\nJ_Constant Copied...";
}

j_constant::j_constant():M_param_func(new  j_reliant_constant(1)){
	cerr << "\nJ_Constant Constructed...";
}

/*Interpolation Evaluation Method*/
class Interpolation_Evaluation_Method{
public:
	virtual bool valid()const{return false;}
	virtual Interpolation_Evaluation_Method* get_copy()const =0;
	virtual Dbl_t eval(Dbl_t)const =0;
	virtual ~Interpolation_Evaluation_Method(){};
	virtual dbl_pair range()const=0;
	virtual void offset_x_vals(Dbl_t)=0;
private:
};

//Linear Interpolation Method
class Linear_Interpolation_Method: public Interpolation_Evaluation_Method{
public:
	Linear_Interpolation_Method(const Data_t& ){}
	Dbl_t eval(Dbl_t)const override final;
	dbl_pair range()const override final{assert(!"Not Implemented"); return dbl_pair(0,0);}
	void offset_x_vals(Dbl_t )override final{}
	Linear_Interpolation_Method* get_copy()const override final{return new Linear_Interpolation_Method(*this);}
private:
};

//Cubic Spline Method
class Cubic_Spline_Method : public Interpolation_Evaluation_Method{
public:
	Cubic_Spline_Method(){}
	Dbl_t eval(Dbl_t i_x)const override final{
		int cubic_index = get_cubic_index(i_x);
		
		if((cubic_index > 0)
		&& (cubic_index < M_x_values.size())
		&& (i_x >= M_x_values[cubic_index-1])
		&& (i_x <= M_x_values[cubic_index])){
		
		Dbl_t t_val = (i_x - M_x_values[cubic_index-1])
			/(M_x_values[cubic_index] - M_x_values[cubic_index-1]);
		return ((1- t_val)*M_y_values[cubic_index-1] + t_val*M_y_values[cubic_index]
			+ t_val*(1-t_val)
				*(M_a_parameters[cubic_index]*(1-t_val)+M_b_parameters[cubic_index]*t_val));

		}else{
			return 0;
		}
	}
	void offset_x_vals(Dbl_t i_offset)override final{
		transform(M_x_values.begin(), M_x_values.end(), M_x_values.begin()
			, bind(plus<Dbl_t>(),_1, i_offset));
	}
	Cubic_Spline_Method(const Data_t&);
	Cubic_Spline_Method* get_copy()const override final{return new Cubic_Spline_Method(*this);}
	bool valid()const override final;
	dbl_pair range()const override final;
private:
	//So named because they correspond to a and b in the wikipidea page on cubic splining
	Dbl_data_t M_a_parameters;
	Dbl_data_t M_b_parameters;
	Dbl_data_t M_x_values;
	Dbl_data_t M_y_values;
	bool consistency_check()const;
	int get_cubic_index(Dbl_t i_x)const;
};

Dbl_t Linear_Interpolation_Method::eval(Dbl_t)const{
	vector<Dbl_t>::const_iterator found_pos;
	assert(!"Not Implemented!");
	return 0;
}

void Param_Function::checked_set(const Dbl_t* i_params)throw(Param_Error, Unsupported_Function_Call){
	for(j_size_t i=0; i < M_bounds.size(); i++){	
		if((i_params[i] < M_bounds[i].first) || (i_params[i] > M_bounds[i].second)){
//			cerr << i << ": " << i_params[i] << '\n' ;
			throw Param_Error(i_params[i], static_cast<int>(i));
		}
	}
	Wrapper_Function::checked_set(i_params);
}

j_constant::j_constant(Dbl_t i_constant):M_constant(i_constant)
	, M_param_func(new j_reliant_constant(1)){
	cerr << "\nJ_Constant Constructed...";
}

void j_polynomial_function::set(const Dbl_t* i_params){
	copy(i_params, i_params+number_of_parameters(), M_params.data());
}

Interpolation_Function::Interpolation_Function(const Interpolation_Function& irk_source){
	M_evaluator = irk_source.M_evaluator->get_copy();
}

void Interpolation_Function::set(const Dbl_t*){
	//throw Unsupported_Function_Call("Cannot call Inerpolation_Function::set(const Dbl_t*);");
}

Dbl_t j_polynomial_function::eval(Dbl_t i_x)const{
	Dbl_t result = 0;
	j_size_t degree = M_params.size() -1;
	for(j_size_t i=0; i < M_params.size(); i++){
		result += M_params[i]*pow(i_x, degree-i);
	}
	return result;
}

//Interpolation Function Eval
Dbl_t Interpolation_Function::eval(Dbl_t i_x)const{
	return M_evaluator->eval(i_x);
}

Dbl_data_t Interpolation_Function::function_param_calc(const Data_t& irk_data)const{
	
	Dbl_data_t params(number_of_parameters(),0.0);
	for(j_size_t i=0; i < irk_data[0].size(); i++){
		if((irk_data[0][i] > M_evaluator->range().first) 
		&& (irk_data[0][i] < M_evaluator->range().second)){
			function_param_calc(params.data(), irk_data[0][i], irk_data[1][i]);
		}
	}

	return move(params);
}

void Interpolation_Function::swap(Interpolation_Function& ir_src){
	::swap(M_evaluator, ir_src.M_evaluator);
}

Interpolation_Function::Interpolation_Function(const Data_t& irk_data_file, Evaluation_Methods i_eval_method
											   , Dbl_t i_low, Dbl_t i_high){
	Data_t data;
	for(int i=0; i < irk_data_file.size(); i++){
		if((irk_data_file[0][i] >= i_low)&& (irk_data_file[0][i] <= i_high)){
			data[0].push_back(irk_data_file[0][i]);
			data[1].push_back(irk_data_file[1][i]);
		}
	}
	Dbl_t min_val = get_minimum_val(data[1].begin(), data[1].end());
	cerr << "\nMin Val subtracted was: " << min_val;
	transform(data[1].begin(), data[1].end(), data[1].begin()
		, bind(minus<Dbl_t>(), _1, min_val));

	switch(i_eval_method){
	case Evaluation_Methods::LINEAR:
		M_evaluator =  new Linear_Interpolation_Method(data);
		break;
	case Evaluation_Methods::CUBIC_SPLINE:
		M_evaluator = new Cubic_Spline_Method(data);
		break;
	default:
		assert(!"Faulty Interpolation Type");
	}
}

dbl_pair Interpolation_Function::range(){return M_evaluator->range();}

void Interpolation_Function::offset_x_vals(Dbl_t i_offset){
	M_evaluator->offset_x_vals(i_offset);
}

/*Cubic Spline Interpolation*/
Cubic_Spline_Method::Cubic_Spline_Method(const Data_t& irk_data_file):M_a_parameters(irk_data_file.size())
	, M_b_parameters(irk_data_file.size()), M_x_values(irk_data_file[0]), M_y_values(irk_data_file[1]){

	if(is_sorted(irk_data_file[0].begin(), irk_data_file[0].end(), greater<Dbl_t>())){
		reverse(M_x_values.begin(), M_x_values.end());
		reverse(M_y_values.begin(), M_y_values.end());
	}
	//Dbl_t min_val = get_minimum_val(M_y_values.begin(), M_y_values.end());
	
	


	if(irk_data_file.size() < 3){
		throw Math_Base_Error("Not Enough Data Points For Cubic Interpolation!");
	}

	Dbl_data_t spline_param_vals(irk_data_file.size(), static_cast<Dbl_t>(1));

	Dbl_t target_accuracy = 1e-8;
	Dbl_t current_error = 1;
	Dbl_t over_relaxation_factor = 1;
	int iterations = 0;
	int max_error_index = -1;
	Dbl_t prev_error = 0;
	while(abs(current_error) > target_accuracy){
		prev_error = current_error;
		if(!(iterations%1)){
			cerr << "\nAt Interpolation Iteration: " << iterations << " At Index: "
				<< max_error_index << " Error of: " << current_error;
		}
		if(!(iterations%20)){
			cerr << "\t...Been Going a While...";
		}
		++iterations;
		Dbl_t first_x_difference = M_x_values[1] - M_x_values[0];
		Dbl_t first_equation_value =  current_error = 3.0*(M_y_values[1] - M_y_values[0])
				/pow(first_x_difference,2) - (2.0*spline_param_vals[0] + spline_param_vals[1])
				/first_x_difference;
		spline_param_vals[0] -= over_relaxation_factor*first_equation_value/(-3.0/first_x_difference);
		//cerr << "\nFirst Spline Val: " << spline_param_vals[0];
		max_error_index  = 0;
		for(int i = 1; i < irk_data_file.size()-1; i++){
			Dbl_t lower_x_delta = M_x_values[i] - M_x_values[i-1];
			Dbl_t upper_x_delta = M_x_values[i+1] - M_x_values[i];

			Dbl_t lower_y_delta = M_y_values[i] - M_y_values[i-1];
			Dbl_t upper_y_delta = M_y_values[i+1] - M_y_values[i];

			Dbl_t equation_value = 3.0*(lower_y_delta/pow(lower_x_delta,2) + upper_y_delta/pow(upper_x_delta,2))
				- spline_param_vals[i+1]/upper_x_delta - spline_param_vals[i-1]/lower_x_delta
				-2.0*spline_param_vals[i]*(1.0/lower_x_delta+ 1.0/upper_x_delta);

			current_error = abs(equation_value) > current_error ? abs(equation_value) : current_error;
			max_error_index = (abs(equation_value) == current_error) ? i : max_error_index;
			Dbl_t f_prime = -3.0*(1.0/lower_x_delta +1.0/upper_x_delta);
			spline_param_vals[i] -= over_relaxation_factor*equation_value/f_prime;

			if(spline_param_vals[i] != spline_param_vals[i]){
				assert(!"Broken Data File");
			}
		}

		Dbl_t last_x_difference = M_x_values.back() - *(&M_x_values.back()-1);
		Dbl_t spline_param_add = *(&spline_param_vals.back()-1) + 2*spline_param_vals.back();
		Dbl_t last_equation_value = 3.0*(M_y_values.back() - *(&M_y_values.back()-1))
			/pow(last_x_difference,2) 
			- spline_param_add/last_x_difference;	

		spline_param_vals.back() -= over_relaxation_factor*last_equation_value/(-3.0/last_x_difference);
		current_error = abs(last_equation_value) > current_error ? abs(last_equation_value) : current_error;
		max_error_index = (abs(last_equation_value) == current_error) ? irk_data_file.size()-1 : max_error_index;
	}
	//Calculating a and b parameters

	for(j_size_t i=1; i < M_a_parameters.size(); i++){
		Dbl_t x_delta = M_x_values[i] - M_x_values[i-1];
		Dbl_t y_delta = M_y_values[i] - M_y_values[i-1];
		M_a_parameters[i] = spline_param_vals[i-1]*x_delta - y_delta;
		M_b_parameters[i] = -spline_param_vals[i]*x_delta + y_delta;
	}
	
	assert(consistency_check());

	for(int i=0 ; i < irk_data_file.size(); i++){
		Dbl_t eval_val = eval(M_x_values[i]);
		//Dbl_t actual_val = M_y_values[i];
		if(abs(eval_val- M_y_values[i]) > target_accuracy*2){
			assert(!"Bad Interpolation created!");
		}
	}
	cerr << "\nCubic Spline Method Constructed...";
}

int Cubic_Spline_Method::get_cubic_index(Dbl_t i_x)const{
	if( (i_x < M_x_values.front()) || (i_x > M_x_values.back())){
		return -1;
		//cerr << "\nValue: " << i_x;
		//throw Math_Base_Error("Extrapolation Encountered!");
	}
	ptrdiff_t calculated_index = (upper_bound(M_x_values.begin(), M_x_values.end(), i_x) - M_x_values.begin());
	calculated_index =  (M_x_values[calculated_index-1] == i_x) ? calculated_index - 1 : calculated_index;
	return static_cast<int>(calculated_index ? calculated_index : calculated_index+1);
}

bool Cubic_Spline_Method::valid()const{
	return !M_a_parameters.empty();
}

bool Cubic_Spline_Method::consistency_check()const{
	return (M_a_parameters.size() == M_b_parameters.size());

}

dbl_pair Cubic_Spline_Method::range()const{
	return dbl_pair(M_x_values.front(), M_x_values.back());
}


//Symbols*************************************************


//J_Symbol Functions--------------------------







/*int size()const*/







//j_value***************************









}



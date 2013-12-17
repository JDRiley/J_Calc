#include "j_math.h"
#include <j_function.h>
#include <J_Utile.h>
//IO and Stream
#include<iostream>

//Facilities
#include <chrono>
#include <exception>
#include <functional>
#include <future>
#include <thread>

//Algorithms
#include <algorithm>
#include <cmath>
#include <numeric>

//Containers
#include <array>
#include <vector>



//IO
using std::cerr; using std::cout; using std::endl;

//Function Objects
using std::bind; using std::plus;

//Facilities
#ifdef WIN32
using std::launch;

#else
#include <time.h>
using std::launch;
#endif

using std::async;namespace chrono = std::chrono; using std::future; using std::future_error;
using namespace std::placeholders; using std::shared_future;using std::system_error; 
namespace this_thread = std::this_thread; using std::thread;

//Algorithms
using std::abs;using std::accumulate;
//Containers
using std::array; using std::vector;

//IO Facilities
#include <iostream>
#include <iomanip>
using std::cout; using std::setw;


//KERNALS for multi_threading
const int KERNELS = 1;
const int THREAD_WAIT_MS = 100;
const int MAX_THREADS = 16;
namespace jomike{

int_arr_t range(int i_low, int i_high, int i_step){
	int_arr_t return_val;
	while(i_low < i_high){
		return_val.push_back(i_low);
		i_low += i_step;
	}
	return return_val;
}



Dbl_data_t range(Dbl_t i_low, Dbl_t i_high, Dbl_t i_step){
	Dbl_data_t return_val;
	while((abs(i_high - i_low) < 0.0000001) || (i_low < i_high)){
		return_val.push_back(i_low);
		i_low += i_step;
	}
	//cerr << i_high - i_low << '\n' << j_abs(i_high - i_low) << endl;
	//cerr << i_high << ' ' << i_low << "\n\n\n"<< endl;

	return return_val;
}

void get_intervals(int* i_dest, int start,  int end,int i_num_threads);



Dbl_t j_integrate(const j_function* i_func, Dbl_t i_start, Dbl_t i_end
				   , int num_steps){
	using namespace std::placeholders;
	Dbl_t step_size = (i_end - i_start)/num_steps;
	
#ifdef VS_STUDIO
	clock_t start_clock = clock();
#else
	//timespec start_time, end_time;
	//start_time.tv_nsec = 0; end_time.tv_nsec = 0;
//	clock_gettime(CLOCK_REALTIME, &start_time);
#endif

	const int k_thread_num = KERNELS;

	//Interval must hold one extra value to indicate end of range
	array<int,MAX_THREADS> intervals;
	get_intervals(intervals.data(),1,num_steps/2, k_thread_num);
	assert(k_thread_num <= MAX_THREADS);
	array<future<Dbl_t>, 100> results;

	//i<k_thread_num -2 to save space for final two threads
	for(int i=0; i < k_thread_num; i++){
		//cerr << "i*****************************: " << i <<  endl;
	try{
		results[i] = async(launch::async, [i_func,step_size,i,i_start](Dbl_t y_start, Dbl_t y_end){
			Dbl_t y_result =0;
			//cerr <<"\nThread: " << i << " given bounds: " << y_start << ' ' << y_end;
			for(int b = static_cast<int>(y_start) ; b < static_cast<int>(y_end); b++){
				y_result += (4*i_func->eval(i_start + (2*b-1)*step_size) 
					+ 2*i_func->eval(i_start+2*b*step_size));
			}
		//	cerr << "\nThread: " << i << " Finished with val of: " << y_result << " Two Eval: " << i_func->eval(2);
			return y_result;
		
		}, intervals[i], intervals[i+1]); //Interval set here

		//cerr << "\nInterval: " << intervals[i] << ' ' << intervals[i+1] << endl;
		if(!results[i].valid())
			throw Unsupported_Function_Call("Thread Not Ready");
	}catch(Unsupported_Function_Call& error){
		cerr << "\nThread Could not be launched. Waiting for: " << THREAD_WAIT_MS << "ms *****************************\n";
		error.print();
		--i;
		this_thread::sleep_for(chrono::milliseconds(THREAD_WAIT_MS));
	}catch(...){
		cerr << "what is going on here?"<<endl;
		--i;
		this_thread::sleep_for(chrono::milliseconds(THREAD_WAIT_MS));
	}
	}
	Dbl_t result = i_func->eval(i_start) + i_func->eval(i_end) 
			+ 4*i_func->eval(i_start+(num_steps-1)*step_size);
	
	result += accumulate(results.begin(), results.begin()+k_thread_num
		,0.0, bind(plus<Dbl_t>(),_1, bind(&future<Dbl_t>::get,_2)));


#ifdef VS_STUDIO
	Dbl_t time = (clock() - start_clock)/Dbl_t(CLOCKS_PER_SEC);
#else
	//clock_gettime(CLOCK_REALTIME, &end_time);
	//Dbl_t time = Dbl_t(end_time.tv_nsec - start_time.tv_nsec)/1e9;
#endif
	bool check_kernels = false;
	if (check_kernels){
		cerr << "\nKernels: "<< k_thread_num << " Total Time: " << time;
	}
	return (1.0/3.0)*step_size*result;
}


//Helpers******************************************************************************

void get_intervals(int* i_dest, int i_start, int i_end,int i_num_threads){
	int step_size = (i_end-i_start)/i_num_threads;
	for(int i=0,  val = i_start	; i < i_num_threads; i++){
		i_dest[i] = val;
		val += step_size;
	}
	i_dest[i_num_threads] = i_end;
}

ll_arr_t greatest_common_divisor(j_llint i_val_one, j_llint i_val_two){
	Stream_Settings_Preserver stream_settings_preserver(cout);
	j_llint high_val = static_cast<j_llint>(std::max(std::abs(i_val_one), std::abs(i_val_two)));
	j_llint low_val = static_cast<j_llint>(std::min(std::abs(i_val_one), std::abs(i_val_two)));

	j_llint remainder = -1;
	j_llint prev_remainder = low_val;
	j_llint prev_prev_remainder = high_val;
	auto width = setw(7);
	auto small_width = setw(4);
	ll_arr_t quotients;
	if(!prev_remainder){
		return ll_arr_t(1, low_val);
	}

	while(remainder){
		j_llint quotient = prev_prev_remainder/prev_remainder;
		quotients.push_back(quotient);
		remainder = prev_prev_remainder - prev_remainder*(quotient);
		cout << '\n' << width << prev_prev_remainder << " = " << small_width << quotient << '.' 
			<< width << prev_remainder << " + " << remainder;
		prev_prev_remainder = prev_remainder;
		prev_remainder = remainder;
	}
	j_llint prev_prev_r_val = high_val;
	j_llint prev_r_val = low_val;
	j_llint prev_prev_x_val = 1;
	j_llint prev_x_val = 0;
	j_llint prev_prev_y_val = 0;
	j_llint prev_y_val = 1;
	j_llint y_val, r_val, x_val;

	for( j_llint cur_quotient : quotients){
		cout << '\n' << width << prev_r_val << width <<  prev_x_val << width << prev_y_val;
		r_val = prev_prev_r_val - cur_quotient*prev_r_val;
		prev_prev_r_val = prev_r_val;
		prev_r_val = r_val;

		x_val = prev_prev_x_val - cur_quotient*prev_x_val;
		prev_prev_x_val = prev_x_val;
		prev_x_val = x_val;

		y_val = prev_prev_y_val - cur_quotient*prev_y_val;
		prev_prev_y_val = prev_y_val;
		prev_y_val = y_val;
	}


	ll_arr_t answer;
	answer.push_back(prev_prev_remainder);
	answer.push_back(prev_prev_x_val);
	answer.push_back(prev_prev_y_val);
	return answer;
}

bool does_divide(int i_divisor, int i_number){
	i_divisor = abs(i_divisor);
	i_number = abs(i_number);

	return (i_number/i_divisor)*i_divisor == i_number;
}

bool relative_prime(j_llint i_val_one, j_llint i_val_two){
	return 1 == greatest_common_divisor(i_val_one, i_val_two).front();
}

j_llint euler_func(j_llint i_num){
	i_num = abs(i_num);
	int value = 0;
	for(int i = 1; i < i_num; i++){
		value += relative_prime(i, i_num);
	}
	return value;
}

bool is_prime(j_llint i_num){
	i_num = abs(i_num);
	return (i_num - 1) == euler_func(i_num);
}

j_llint greatest_common_divisor_simp(j_llint i_val_one, j_llint i_val_two){
	return greatest_common_divisor(i_val_one, i_val_two).front();
}

j_llint least_common_multiple(j_llint i_val_one, j_llint i_val_two){
	return i_val_one*i_val_two / greatest_common_divisor_simp(i_val_one, i_val_two);
}



}
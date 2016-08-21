#include "math_base.h"
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cassert>
#include<iomanip>
using std::copy;using std::cerr;
using std::ostream; using std::ostream_iterator;
using std::setw;using std::for_each;



namespace jomike{




void Math_Base_Error::print()const throw(){ cerr << '\n' << what();}

void Param_Error::print()const throw(){
	cerr << "\nInvalid Param at Index: " << param_index << " with value: "
		<< invalid_param ;
}



ostream& operator<<(ostream& o_str, const jtl::Dbl_data_t& ir_cont){
	auto width = setw(o_str.precision()+6);
	o_str << "[" << width;
	for_each(ir_cont.begin(), ir_cont.end()
		,[&o_str, &width](jtl::Dbl_t val){o_str << width << val << ' ';});

	o_str << "]";
	return o_str;
}


Dbl_t convolve_gauss(j_function* , Dbl_t ){
/*	array<int,51> intervals;
	get_intervals(intervals.data(),0,M_impulse.size(), NUMBER_OF_THREADS);	
	
	array<future<Dbl_t>, NUMBER_OF_THREADS> results;
	for(size_t i=0; i < results.size(); i++){
		results[i] = async(&j_conv::partial_convolve,this, i_func, i_x
			, intervals[i], intervals[i+1]);
	}
	return accumulate(results.begin(), results.end()
		,0.0, bind(plus<Dbl_t>(),_1, bind(&future<Dbl_t>::get,_2)));
		*/
	assert(0);
	return 1;
}



}


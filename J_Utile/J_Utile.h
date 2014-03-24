#ifndef J_UTILE_H
#define J_UTILE_H

#include <J_Fwd_Decl.h>

//IO Facilities
#include <iosfwd>

#pragma comment(lib, "J_Utile"J_LIB_APPEND)

namespace jomike{


class True_Class{
public:
	operator bool(){ return true; }
};

extern True_Class j_true;

template<typename T>
bool between_inclusive(const T& irk_cand, const T& irk_low, const T& irk_high){
	return ((irk_low <= irk_cand) && (irk_high >= irk_cand));
}

void j_block_execution(int i_milliseconds);

class Stream_Settings_Preserver{
public:
	Stream_Settings_Preserver(std::ios&);
	~Stream_Settings_Preserver();
private:
	class Stream_Settings_Impl;
	std::unique_ptr<Stream_Settings_Impl> M_impl;
};

bool true_test(bool i_val);

template<class St>
class Instance_Pointer{
public:
	St* operator->();
	St& operator*();
};

template<class St>
St* Instance_Pointer<St>::operator->(){
	return &St::get_instance();
}

template<class St>
St& Instance_Pointer<St>::operator*(){return St::get_instance();}


template<typename St>
St zero_set(){
	St new_st;
	memset(&new_st, 0, sizeof(St));
	return new_st;
}

template<typename St>
void set_to_zero(St* i_target){
	memset(i_target, 0, sizeof(St));
}


template<typename Ret_t, typename Int_t>
typename std::enable_if<std::is_integral<Int_t>::value
	&& std::is_integral<Ret_t>::value, Ret_t>::type normalized_val(Int_t i_val){
	j_dbl ratio = static_cast<j_dbl>(i_val)
		/std::numeric_limits<Int_t>::max();

	return static_cast<Ret_t>(std::numeric_limits<Ret_t>::max()*ratio);
}

template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t = j_dbl>
class J_Duration_Tester{
public:
	J_Duration_Tester();
	J_Duration_Tester(const Time_Retrieval_t1&, const Time_Retrieval_t2&);
	
	bool time_exceeded()const;
	bool time_not_exceeded()const;
	Num_t elapsed_time()const;
	Num_t time_left()const;
	void reset_timer();
private:
	mutable Time_Retrieval_t1 M_time_retriever;
	mutable Time_Retrieval_t2 M_duration_time_retriever;
	Num_t M_last_reset_time;
};

template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t /*= j_dbl*/>
Num_t J_Duration_Tester<Time_Retrieval_t1, Time_Retrieval_t2, Num_t>::time_left()const{
	return M_duration_time_retriever() - elapsed_time();
}

template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t /*= j_dbl*/>
Num_t J_Duration_Tester<Time_Retrieval_t1, Time_Retrieval_t2, Num_t>::elapsed_time()const{
	return M_time_retriever() - M_last_reset_time;
}

template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t /*= j_dbl*/>
bool J_Duration_Tester<Time_Retrieval_t1, Time_Retrieval_t2, Num_t>::time_exceeded()const{
	return (M_time_retriever() - M_last_reset_time) > M_duration_time_retriever();
}

#ifdef VS_2013
template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t = decltype(Time_Retrieval_t1()())>
J_Duration_Tester<Time_Retrieval_t1, Time_Retrieval_t2, Num_t>
	make_duration_tester(const Time_Retrieval_t1& irk_cur_time_retriever
	, const Time_Retrieval_t2 irk_duration_retriever, Num_t i_duration){
		typedef J_Duration_Tester<Time_Retrieval_t1
			, Time_Retrieval_t2
			, decltype(Time_Retrieval_t1()())> duration_tester_t;

		return duration_tester_t(irk_cur_time_retriever, irk_duration_retriever, i_duration)

}
#endif

template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t /*= j_dbl*/>
void J_Duration_Tester<Time_Retrieval_t1, Time_Retrieval_t2, Num_t>::reset_timer(){
	M_last_reset_time = M_time_retriever();
}

template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t /*= j_dbl*/>
J_Duration_Tester<Time_Retrieval_t1, Time_Retrieval_t2, Num_t>
	::J_Duration_Tester(const Time_Retrieval_t1& i_time_retriever
		, const Time_Retrieval_t2& irk_duration_time_retriever)
		:M_time_retriever(i_time_retriever)
		, M_duration_time_retriever(irk_duration_time_retriever){

		M_last_reset_time = M_time_retriever();
}

template<typename Time_Retrieval_t1, typename Time_Retrieval_t2, typename Num_t /*= j_dbl*/>
J_Duration_Tester<Time_Retrieval_t1, Time_Retrieval_t2, Num_t>::J_Duration_Tester(){
	M_last_reset_time = M_time_retriever();
}


template<int M_mod_value>
class J_Modder{
public:
	J_Modder();
	bool tick();
	int operator()(int);
private:
	long long M_counter;
};

template<int M_mod_value>
bool J_Modder<M_mod_value>::tick(){
	return !(M_counter++%M_mod_value);
}

template<int M_mod_value>
J_Modder<M_mod_value>::J_Modder(){
	M_counter = 0;
}
template<int M_mod_value>
int J_Modder<M_mod_value>::operator()(int i_val){
	return i_val%M_mod_value;
}


}

#endif
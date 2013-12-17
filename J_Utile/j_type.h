#ifndef J_TYPE_H
#define J_TYPE_H

#include <J_Fwd_Decl.h>

#define PC_OUSIDE_EXPECTED_FLOW_OF_CONTROL_ASSERT (!"PC has breached expected area of control");



//numeric
#include <limits>

//Utilities 
#include <cassert>

#include <type_traits>

namespace jomike{

extern int g_j_type_cpp_symbol_to_define;

template<typename St, typename Num_t>
typename std::enable_if<std::is_signed<St>::value && std::is_signed<Num_t>::value, St>::type
	safe_cast(Num_t i_val){
	assert(std::numeric_limits<St>::max() >= i_val);
	assert(std::numeric_limits<St>::min() <= i_val);
	return static_cast<St>(i_val);
}

template<typename St, typename Num_t>
typename std::enable_if<std::is_signed<St>::value && std::is_unsigned<Num_t>::value, St>::type
	safe_cast(Num_t i_val){
	typedef typename std::make_unsigned<St>::type St_Unsigned_t;

	assert(i_val >= 0);
	assert(static_cast<St_Unsigned_t>(std::numeric_limits<St>::max()) 
				>= i_val);

	return static_cast<St>(i_val);
}

template<typename St, typename Num_t>
typename std::enable_if<std::is_unsigned<St>::value, St>::type
	safe_cast(Num_t i_val){
	typedef typename std::make_unsigned<Num_t>::type Num_Unsigned_t;

	assert(i_val >= 0);
	assert(std::numeric_limits<St>::max() >= static_cast<Num_Unsigned_t>(i_val));

	return static_cast<St>(i_val);
}

template<typename Num_t>
char safe_char_cast(Num_t i_val){return safe_cast<char, Num_t>(i_val);}

template<typename Num_t>
int safe_int_cast(Num_t i_val){return safe_cast<int>(i_val);}

template<typename Num_t>
ptrdiff_t safe_ptrdiff_cast(Num_t i_val){return safe_cast<ptrdiff_t>(i_val);}


template<typename Num_t>
long safe_long_cast(Num_t i_val){return safe_cast<long>(i_val);}

template<typename Num_t>
unsigned safe_uns_cast(Num_t i_val){return safe_cast<unsigned>(i_val);}

template<typename Num_t>
long long safe_long_long_cast(Num_t i_val){return safe_cast<long long>(i_val);}

}


#endif
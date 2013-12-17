#ifndef J_NUMERIC_H
#define J_NUMERIC_H

#include <J_Fwd_Decl.h>

#pragma comment(lib, "J_Math"J_LIB_APPEND)

namespace jomike{



typedef long long j_llint;
typedef j_llint integral_t;

class j_exponent{
public:
	integral_t value()const;
private:
	j_llint M_base;
	j_llint M_exponent;
};


}

#endif
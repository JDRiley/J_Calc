#include "J_Error.h"

//IO Facilities
#include <iostream>

using std::cerr; using std::endl;

namespace jomike{


void J_Error::print()const{
	cerr << '\n' << M_msg << endl;
}


}
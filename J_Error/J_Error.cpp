#include "J_Error.h"

//IO Facilities
#include <iostream>

using std::cerr; using std::endl;

namespace jomike{


void J_Error::print()const{
	cerr << '\n' << M_msg << endl;
}
J_Syntax_Error::J_Syntax_Error(const std::string& ik_message)
:J_Error(ik_message){}


}
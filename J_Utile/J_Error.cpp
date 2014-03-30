#include "J_Error.h"

//IO Facilities
#include <iostream>

using std::cerr; using std::endl;
using std::string;

namespace jomike{
J_Syntax_Error::J_Syntax_Error(const string& ik_message /*= "Bad Symbol Operation"*/)
	:J_Error(ik_message){

}

void J_Error::print()const{
	cerr << '\n' << M_msg << endl;
}


}
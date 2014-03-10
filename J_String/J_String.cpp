#include "J_String.h"
#include <ex_array.h>
#include <J_UI/J_UI_String.h>
#include <j_type.h>
//
#include <J_Utile.h>
//Algorithm
#include <algorithm>

using std::find_first_of; using std::for_each;

//Containers
using std::string;

//IO Facilities

#include <fstream>


using std::ifstream;

//Utilities
#include <iterator>

using std::istreambuf_iterator;

namespace jomike{
string file_to_str(const string& irk_filename){

	
	ifstream file(irk_filename);
	assert(file);
	return string(istreambuf_iterator<char>(file), istreambuf_iterator<char>());
}

J_UI_String::const_iterator get_closing_bracket(J_UI_String::const_iterator ik_pos){
	if('[' != ik_pos->charcode()){
		return ik_pos;
	}
	int bracket_count = 0;
	while(j_true){
		switch(ik_pos->charcode()){
		case '[': ++bracket_count; break;
		case ']': --bracket_count; break;
		default:
			;
		}
		if(!bracket_count){
			return ik_pos;
		} else{
			++ik_pos;
		}
	}
	assert(0);
	return J_UI_String::const_iterator();
}









}
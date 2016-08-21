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


template<>
int from_string<int>(const std::string& irk_str){
	return std::stoi(irk_str);
}

template<>
j_dbl from_string<j_dbl>(const std::string& irk_str){
	return std::stod(irk_str);
}

template<>
j_llint from_string<j_llint>(const std::string& irk_str){
	return std::stoll(irk_str);
}

template<>
bool from_string<bool>(const std::string& irk_str){
	return !irk_str.empty();
}



template<>
int from_wstring<int>(const std::wstring& irk_str){
	return std::stoi(irk_str);
}

template<>
j_dbl from_wstring<j_dbl>(const std::wstring& irk_str){
	return std::stod(irk_str);
}

template<>
j_llint from_wstring<j_llint>(const std::wstring& irk_str){
	return std::stoll(irk_str);
}

template<>
bool from_wstring<bool>(const std::wstring& irk_str){
	return !irk_str.empty();
}




}
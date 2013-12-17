#include "Data_File.h"
#include <math_base.h>
#include "j_math.h"
#include <j_function.h>
#include <iostream>
#include <functional>
#include <algorithm>
#include <fstream>
#include <map>
#include <string>
#include <utility>
#include <iterator>
using std::minus;using std::copy;using std::ostream_iterator;using std::move;using std::ios;
using std::vector;using std::cerr;using std::cin;using std::ostream;using std::ofstream;
using std::transform; using std::istream; using std::ifstream;using std::cout;using std::endl;
using std::map; using std::string; using std::bind; using namespace std::placeholders;
using std::streamsize;using std::back_inserter;using std::ref;
namespace jomike{

//Data Print
void Data_File::print(ostream& ir_os)const{
	streamsize old_precision = ir_os.precision(15);
	for(int i=0; i < size(); i++){
		ir_os << '\n' << M_data[0][i] << '\t' << M_data[1][i];
	}

	ir_os << endl;
	ir_os.precision(old_precision);
}

//Dbl_Data_t and j_function constructor
Data_File::Data_File(const Dbl_data_t& irk_x_vals, const j_function& i_func){
	M_data.resize(2);
	M_data[0] = irk_x_vals;

	auto insert_it = back_inserter(M_data[1]);
	transform(M_data[0].begin(), M_data[0].end()
		, insert_it, bind(&j_function::eval, ref(i_func), _1));

}

//Stream Constructor
Data_File::Data_File(istream& ir_is):M_data(2){
	get_data_from_file(ir_is);
}


void Data_File::get_data_from_file(istream& ir_is){
	Dbl_t x,y;
	M_data[0].clear();
	M_data[1].clear();
	while((ir_is >> x) && (ir_is >> y)){
		M_data[0].push_back(x);
		M_data[1].push_back(y);
	}
}




}









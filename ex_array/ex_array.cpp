#include <vld/vld.h>
#include "ex_array.h"
//
#include <J_Error.h>

//Algorithm
#include <algorithm>
#include <functional>
#include <random>
using std::copy;using std::equal; using std::uniform_int_distribution; using std::default_random_engine;



//Container
#include <array>
#include <vector>
using std::array;
using std::vector;


#include <iostream>
using std::cerr; using std::cin; using std::cout; using std::endl;


#include <utility>
using std::move;
namespace jomike{

#if EX_ARRAY_TEST

class Test_Class{
public:
	Test_Class(int i_size):M_data(i_size, 0){}
	Test_Class(Test_Class&& irr_source):M_data(move(irr_source.M_data)){}
	~Test_Class(){}
private:
	vector<int> M_data;
	int M_size;
};
const int TEST_SIZE_VALUE = 101;


void ex_array_int_test();
void ex_array_class_destruction_test(int i_range);
void compilation_test();
#endif //EX_ARRAY_TEST
}


int main(int , char** ){
#if EX_ARRAY_TEST
	try{

		jtl::ex_array_int_test();
		jtl::ex_array_class_destruction_test(jtl::TEST_SIZE_VALUE);
		jtl::compilation_test();


	}catch(jtl::J_Error& e){
		cerr << "\n" << e.message() << std::endl;
#ifdef WIN32	
		MessageBoxA(0, e.message(), "Error", MB_OK);
#endif
		return 1;
	}

#else //!EX_ARRAY_TEST


#endif //EX_ARRAY_TEST
	cout << "\n\nEnter Key To Finish: ";
	cin.get();
	return 0;
}

namespace jomike{
#if EX_ARRAY_TEST


void ex_array_int_test(){

	array<int, 10> source_int;
	for(int i=0 ; i < source_int.size(); i++){
		source_int[i] = i;
	}
	vector<int> vect_int(source_int.begin(), source_int.end());
	int_arr_t ex_int(source_int.begin(), source_int.end());

	assert(static_cast<size_t>(ex_int.size()) == vect_int.size());
	assert(equal(vect_int.begin(), vect_int.end(), ex_int.begin()));

	vect_int.erase(vect_int.begin() + 4, vect_int.end());
	ex_int.erase(ex_int.begin() + 4 , ex_int.end());

	assert(equal(vect_int.begin(), vect_int.end(), ex_int.begin()));

	vect_int.insert(vect_int.end(), source_int.begin()+4, source_int.end());
	ex_int.insert(ex_int.end(), source_int.begin()+4, source_int.end());
	assert(equal(vect_int.begin(), vect_int.end(), ex_int.begin()));
}




void ex_array_class_destruction_test(int i_range){
	cerr << "\n\nBeginning Class Destruction Test*************************************************\n";
	ex_array<Test_Class> test_ex_array, test_ex_array2, test_ex_array3;
	for(int i=0; i <  i_range; i++){
		if(!(i%1000)){
			cerr << "\nInserting Test_Class(" << i << ")!";
		}
		test_ex_array.push_back(Test_Class(i));
	}
	
	test_ex_array.clear();
	if(VLDGetLeaksCount()){
		throw J_Error("Memory Leaks Encountered");
	}

	//Testing different forms of inser
	cerr <<"\n\nTesting Different forms of insertion" << endl;

{
	test_ex_array2.set_front_buffer(0.5);
	test_ex_array3.set_front_buffer(0.5);
	vector<Test_Class> test_vec1;
	default_random_engine gen(1);
	for(int i=0; i <  i_range; i++){
		if(!(i%100)){
			cerr << "\nInserting Test_Class(" << i << ")!";
		}
		assert(i == test_ex_array.size());
		uniform_int_distribution<int> rand_gen(0, i);
		int i_pos = rand_gen(gen);

		
		test_ex_array.insert(test_ex_array.begin(), Test_Class(i));
		test_ex_array2.insert(test_ex_array2.end(), Test_Class(i));
		test_ex_array3.insert(test_ex_array3.begin() + i_pos, Test_Class(i));
		assert(test_ex_array.size() == test_ex_array2.size());
		assert(test_ex_array.size() == test_ex_array3.size());
	}

	cerr << "\n\nBeginning Deletion !" << endl;

	for(int i=0; i < i_range; i++){
		if(!(i%100)){
			cerr << "\nDeleting Test_Class(" << i << ")!";
		}
		assert(test_ex_array.size() == (i_range - i));
		uniform_int_distribution<int> rand_gen(0, i_range - i - 1);
		test_ex_array.erase(test_ex_array.begin() + rand_gen(gen));
		test_ex_array2.erase(test_ex_array2.begin() + (i_range - i - 1));
		test_ex_array3.erase(test_ex_array3.begin());
		assert(test_ex_array.size() == test_ex_array2.size());
		assert(test_ex_array.size() == test_ex_array3.size());
	}

}

	test_ex_array.clear();
	test_ex_array2.clear();
	test_ex_array3.clear();


	if(VLDGetLeaksCount()){
		throw J_Error("Memory leaks encountered in insertion test");
	}
}



void compilation_test(){
	array<Test_Class, 10> test_source = {
		Test_Class(5), Test_Class(5), Test_Class(5), Test_Class(5),
		 Test_Class(5), Test_Class(5), Test_Class(5), Test_Class(5),
		  Test_Class(5), Test_Class(5)
	};
	const int test_size = safe_int_cast(test_source.size());
	typedef ex_array<Test_Class> Test_Arr_t;
	Test_Arr_t test_ex_array;
	const Test_Arr_t& ex_const = test_ex_array;
	Test_Class dummy(7);

	test_ex_array.assign(&dummy, &dummy + 1);

	test_ex_array.assign(5, dummy);


	Test_Class dummy2(test_ex_array.back());
	Test_Class dummy3(ex_const.back());

	assert(test_ex_array.back_buffer() == ex_const.back_buffer());

	auto begin_it = test_ex_array.begin();
	auto const_begin_it = ex_const.begin();


	assert(const_begin_it == test_ex_array.cbegin());
	assert(const_begin_it == begin_it);

	auto end_it = test_ex_array.end();
	auto const_end_it = ex_const.end();

	assert(const_end_it == test_ex_array.cend());
	assert(const_end_it == end_it);
	
	assert(ex_const.buffer() == (ex_const.front_buffer() + ex_const.back_buffer()));

	assert(ex_const.capacity() >= ex_const.size());

	test_ex_array.clear();
	assert(test_ex_array.data() == nullptr);

	test_ex_array.emplace_back(dummy3);
	assert(!test_ex_array.empty());


	test_ex_array.clear();
	assert(test_ex_array.empty());

	test_ex_array = Test_Arr_t(test_source.begin(), test_source.end());

	test_ex_array.erase(test_ex_array.begin(), test_ex_array.begin()+2);
	test_ex_array.erase(test_ex_array.begin());
	test_ex_array.erase(test_ex_array.begin(), 2);
	assert(test_ex_array.size() == (test_size - 5));
	test_ex_array.erase(0, 2);
	assert(test_ex_array.size() == (test_size - 7));
	test_ex_array.erase(0);
	assert(test_ex_array.empty());
	test_ex_array.emplace_back(5);

}


#endif //EX_ARRAY_TEST
}


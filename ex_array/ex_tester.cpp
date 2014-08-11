#include "ex_array.h"
#include <iostream>
#include <cassert>
#include <deque>
#include <vector>
#include "../Utile/utile.h"
using jomike::ex_array;
using std::vector;
using std::deque;

template<class cont_one, class cont_two>
bool assert_equal(cont_one& , cont_two& );

template<class cont_one, class cont_two>
bool assert_equal_copy(cont_one , cont_two );


class TestClass{
public:
	void print(){
		for(unsigned i=0; i < x; i++){
			std::cerr << _M_ptr[i] << ' ';
		}
		std::cerr << std::endl;
	}
	TestClass():x(5), _M_ptr(new int[x]), count(0){
		for(unsigned i=0; i < x; i++){
			if(i < 2){
				_M_ptr[i] = 1;
			}else{
				_M_ptr[i] = 0;
				for(unsigned b = 0 ; b <i ; b++){
					_M_ptr[i] += _M_ptr[b];
					if(_M_ptr[i] > 10000)
						_M_ptr[i] = 1;
				}
			}

		}

	}

	TestClass(TestClass&& _irk_src):x(_irk_src.x), _M_ptr(_irk_src._M_ptr){
		_irk_src._M_ptr = 0;
		count = _irk_src.count;
	}

	TestClass& operator=(TestClass&& mover){
		delete [] _M_ptr;
		_M_ptr = mover._M_ptr;
		x = mover.x;
		mover._M_ptr = 0;
		count = mover.count;
		return *this;
	}

	TestClass& operator=(TestClass& src){
		delete [] _M_ptr;
		x = src.x;
		_M_ptr = new int[x];
		for(unsigned i = 0; i < x; i++){
			_M_ptr[i] = src._M_ptr[i];
		}
		count = src.count;
		return *this;
	}

	TestClass(const TestClass& _irk_src): x(_irk_src.x),_M_ptr(new int[x]){
		for(unsigned i=0; i <x; i++){
			_M_ptr[i] = _irk_src._M_ptr[i];
		}
		count = _irk_src.count;
	}
	void inc_count(const unsigned i){
		count += i;
	}
	~TestClass(){
		delete [] _M_ptr;
	}
	bool operator==(const TestClass& right){
		if(x != right.x)
			return false;
		for(unsigned i=0; i <x; i++){
			if(_M_ptr[i] != right._M_ptr[i])
				return false;
			if(count != right.count)
				return false;
		}
		return true;
	}
	void print_count(){
		std::cout << count ;
	}
private:
		unsigned x;
	int* _M_ptr;
	int count;
};

class TestClass2{
public:
	TestClass2(int x):_M_ptr(new int[5]), count(0){
		for(unsigned i=0; i < 5; i++){
			_M_ptr[i] = x;
		}
	}
	TestClass2(TestClass2&& src):_M_ptr(src._M_ptr), count(src.count){
		_M_ptr = 0;
	}
	TestClass2(const TestClass2& _irk_src):_M_ptr(new int[5]), count(_irk_src.count){
		std::copy(_irk_src._M_ptr, _irk_src._M_ptr+5, _M_ptr);
	}
	~TestClass2(){
		delete [] _M_ptr;
	}
	void inc_count(const unsigned _ik_count){
		count += _ik_count;
	}
	bool operator==(const TestClass2& right){
		if(count != right.count)
			return false;
		for(unsigned i=0; i < 5; i++){
			if(_M_ptr[i] != right._M_ptr[i])
				return false;
		}
		return true;
	}
	TestClass2& operator=(const TestClass2& right){
		if(this == &right)
			return *this;
		count = right.count;
		for(unsigned i=0; i < 5; i++){
			_M_ptr[i] = right._M_ptr[i];
		}
		return *this;
	}
private:
	int* _M_ptr;
	int count;
};
int insert_test();

template<class iter>
void print(iter,iter);

int big_test();
int main(){

	return insert_test();
}

int big_test(){
	deque<TestClass2> int_vec;
	deque<TestClass2> int_vec2;

	ex_array<TestClass2> int_ex;
	ex_array<TestClass2> int_ex2;
	unsigned num = 2500000;
	int_ex2.capacity();
	//int_vec.reserve(num);
	int_ex.reserve(num*2,2);
	TestClass2 tester2(5);


	for(unsigned i=5; i < num+5; i++){
		int_vec.push_back(tester2);
		int_vec.insert(int_vec.begin(),int_vec.back());
			int_vec.front().inc_count(i);
			int_vec.back().inc_count(i);
		tester2.inc_count(i);
	}

	//assert_equal(int_ex, int_vec);
	//assert_equal_copy(int_ex, int_vec);

	TestClass2 tester(5);


	for(unsigned i=5; i < num+5; i++){
		int_ex.push_back(tester);
		int_ex.push_front(int_ex.back());
			int_ex.front().inc_count(i);
			int_ex.back().inc_count(i);
		tester.inc_count(i);
	}




	std::cerr << "Start Assign" << std::endl;


	int_ex2.assign(int_ex.begin(), int_ex.end());

	int_vec2.assign(int_vec.begin(), int_vec.end());

	std::cerr <<"End Assign" << std::endl;

	//assert_equal(int_ex2, int_vec2);
	//assert_equal_copy(int_ex2, int_vec2);


	for(unsigned i=0; i < num; i++){
		assert(int_vec2[i] == int_vec[i]);
		assert(int_ex2[i] == int_ex[i]);
		assert(int_ex[i] == int_vec[i]);
		//int_vec[i].print_count();
		//std::cout.put(' ');
		//int_ex[i].print_count();
	}
		//	std::cout << std::endl;
	assert(tester == tester2);

	//int_ex2.print_impl(std::cerr);

	return 0;
}

template<class cont_one, class cont_two>
bool assert_equal(cont_one& first, cont_two& second){
	assert(first.size() == second.size());
	typename cont_one::iterator fpos = first.begin();
	typename cont_two::iterator spos = second.begin();
	typename cont_one::const_iterator k_fpos = first.cbegin();
	typename cont_two::const_iterator k_spos= second.cbegin();
	for(unsigned i=0; i < first.size(); i++){
		assert(first[i] == second[i]);
	}
	unsigned size1 = std::distance(spos, second.end());
	unsigned size2 = std::distance(fpos, first.end());
	assert(size1 == size2);

	size1 = std::distance(k_spos, second.cend());
	size2 = std::distance(k_fpos, first.cend());

	std::cerr << "Size 1: " << size1 << " Size 2: " << size2
		<< " Size1 cont " << second.size() << " Size two cont " <<first.size() 
		<< std::endl;
	assert(size1 == first.size());
	while(first.end() != fpos){
		assert(*fpos++ == *spos++);
		assert(*k_fpos++ == *k_spos++);
	}
	return true;
}

template<class cont_one, class cont_two>
bool assert_equal_copy(cont_one first, cont_two second){
	return assert_equal(first,second);
}

int insert_test(){
	typedef int test_type;
	typedef ex_array<test_type> cont_one;
	typedef vector<test_type> cont_two;
	
	cont_one int_ex;
	cont_two int_vec;
	int_vec.emplace_back(4);
	const unsigned SIZE = 10; 
	for(unsigned i=0; i < SIZE; i++){
		int_ex.push_back(i);
		int_vec.push_back(i);
	}
	int_vec.reserve(SIZE*2);

	std::cerr << "Back: " << int_vec.back() << " int_vec.end()-1: "
		<< *(int_vec.end() - 1) << std::endl;
	
	for(unsigned i=0; i < SIZE; i++){
		int_ex.insert(int_ex.begin(), int_ex.begin()
			, int_ex.end());
		std::cerr << "ex " << std::endl;
		print(int_ex.begin(), int_ex.end());
		int_vec.insert(int_vec.begin(), (int_vec.end()-1)
			, int_vec.end());
		std::cerr << "vec " << std::endl;
		print(int_vec.begin(), int_vec.end());
	}
	
	std::cerr << "Eq Test" << std::endl;
	print(int_ex.begin(), int_ex.end());
	std::cerr << "Diff" << std::endl;
	print(int_vec.begin(), int_vec.end());

	bool result_of_test = 
		std::equal(int_vec.begin(), int_vec.end(), int_ex.begin());
	assert(result_of_test);
	
	return result_of_test;


}

template<class iter>
void print(iter _i_first, iter _i_last){
	std::ostringstream ooo;
	while(_i_first != _i_last){
		ooo << *_i_first++ << ' ';
	}
	std::cerr << ooo.str() << std::endl;
}

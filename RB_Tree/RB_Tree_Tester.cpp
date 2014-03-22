#include "RB_Tree_Tester.h"
//
#include "j_multi_map.h"
//
#include "j_tree.h"
//
#include <ex_array.h>
//
#include <algorithm>

using std::ostream;
using std::pair; using std::random_shuffle;
namespace jomike{

std::ostream& operator<<(ostream& ir_os, pair<int, int> i_pair){
	ir_os << "{" << i_pair.first << ',' << i_pair.second << '}';
	return ir_os;
}

void RB_Tree_Tester::multi_map_test()const{
	j_multi_map<int, int> multi_map;
	multi_map.M_tree.set_messages(false);
	const int num_to_test = 60;
	for(int i = 0; i < num_to_test; i++){
		multi_map.insert(0, i);
	}
	
	auto pos = multi_map.lower_bound(0);
	for(int i = 0; i < num_to_test; i++){
		assert(*pos++ == i);
	}

	multi_map.clear();
	const int mod_val = 3;
	for(int i = 0; i < num_to_test; i++){
		multi_map.insert(i % mod_val, i);
	}
	auto pos_2 = multi_map.M_tree.begin();
	int val = 0;
	int last_mod = 0;
	int num_units = num_to_test / mod_val;
	for(int i = 0; i < num_to_test; i++, ++pos_2){
		if(last_mod != (i/num_units)){
			last_mod = (i/num_units);
			val = last_mod;
		}
		assert(pos_2->first == i/num_units);
		assert(pos_2->second == val);
		val += mod_val;
		
	}
	for(int i = 0; i < mod_val; i++){
		const j_size_t num_erased = multi_map.erase(i);
		assert(num_units == num_erased);
	}

	return;
}

ex_array<int> range(int i_first_val, int i_num_vals){
	ex_array<int> int_arr(i_num_vals);
	int* data = int_arr.data();
	for(int i = 0; i < i_num_vals; ++i, ++i_first_val){
		data[i] = i_first_val;
	}
	return std::move(int_arr);
}

void RB_Tree_Tester::tree_test()const{
	j_tree<int> tree;

	tree.insert(0);
	bool inserted = tree.insert(0).second;
	assert(!inserted);
	assert(1 == tree.size());

	j_size_t erase_size = tree.erase(0);
	assert(1 == erase_size);

	assert(tree.empty());
	const int num_to_test = 1000;

	ex_array<int> array_range(range(0, num_to_test));


	random_shuffle(array_range.begin(), array_range.end());

	tree = j_tree<int>(array_range.begin(), array_range.end());
	auto tree_pos = tree.begin();
	for(int i = 0; i < num_to_test; ++i, ++tree_pos){
		assert(i == *tree_pos);
	}
	return;
}

}




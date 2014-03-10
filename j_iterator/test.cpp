//
#include <j_map.h>

//
#include <string>
using namespace jomike;
int test(){
	using std::string;
	typedef std::pair<int, string> int_string_pair_t;
	J_RB_Tree <
		int_string_pair_t
		, int
		, Pair_Key_Comparer < int_string_pair_t
		, std::less < int >>
		, std::allocator < int_string_pair_t >> tree;
	(void)tree;


	bool val = tree.end() != tree.find(3);
	(void)val;
	return 0;
}
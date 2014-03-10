#ifndef RB_TREE_TESTER_H
#define RB_TREE_TESTER_H

#ifndef RB_TREE_DEBUG

#error //Test class only available during testing

#endif // !j_tree_TESTING


#include "RB_Tree.h"


namespace jomike{
class RB_Tree_Tester{
public:
	void multi_map_test()const;
	void tree_test()const;
private:
};

}

#endif //RB_TREE_TESTER_H



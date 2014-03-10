#ifndef J_RB_TREE_ERROR
#define J_RB_TREE_ERROR

#include "J_Error.h"


namespace jomike{
const char* const UNDEFINED_OPERATION = "Undefined Operation: ";
class Bad_Tree_Op : public J_Error{
	std::string M_message;

public:
	Bad_Tree_Op(const char* const ik_message = ""):J_Error(ik_message)
		, M_message("###BAD_TREE_OPERATION###! ->"){
		M_message.append(ik_message);
	};

	const std::string& what(){
		return M_message;
	}
};
}

#endif //J_RB_TREE_ERROR



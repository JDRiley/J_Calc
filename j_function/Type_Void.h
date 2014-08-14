#ifndef TYPE_VOID_H
#define TYPE_VOID_H

#include "Type_Syntax.h"


namespace jomike{
class Type_Void : public Type_Syntax{
public:
	Type_Void();

	Type_Void* get_copy()const override;

	Type_Void* move_copy()override;

	bool is_void()const override;

private:
};

}

#endif //TYPE_VOID_H



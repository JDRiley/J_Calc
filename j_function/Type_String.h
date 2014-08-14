#ifndef TYPE_STRING_H
#define TYPE_STRING_H

#include "Type_Syntax.h"


namespace jomike{
class Type_String : public Type_Syntax{
public:
	Type_String();

	Type_String* get_copy()const override;

	Type_String* move_copy()override;
private:
};

}

#endif //TYPE_STRING_H



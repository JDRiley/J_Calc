#ifndef TYPE_INT_H
#define TYPE_INT_H

#include "Type_Syntax.h"


namespace jomike{
class Type_Int : public Type_Syntax{
public:
	Type_Int();

	Type_Int* get_copy()const override;

	Type_Int* move_copy()override;

	j_value convert_value(const j_value& irk_val)const override;
private:

};

}

#endif //TYPE_INT_H



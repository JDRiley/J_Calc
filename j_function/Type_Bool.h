#ifndef TYPE_BOOL_H
#define TYPE_BOOL_H

#include "Type_Syntax.h"


namespace jomike{
class Type_Bool : public Type_Syntax{
public:
	Type_Bool();

	Type_Bool* get_copy()const override;

	Type_Bool* move_copy()override;

	j_value convert_value(const j_value& irk_val)const override;
private:
};

}

#endif //TYPE_BOOL_H



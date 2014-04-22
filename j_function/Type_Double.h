#ifndef TYPE_DOUBLE_H
#define TYPE_DOUBLE_H

#include "Type_Syntax.h"


namespace jomike{
class Type_Double : public Type_Syntax{
public:
	Type_Double();

	Type_Double* get_copy()const override; 

	Type_Double* move_copy()override;

	j_value convert_value(const j_value& irk_val)const override;
private:
};

}

#endif //TYPE_DOUBLE_H



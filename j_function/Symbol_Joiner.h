#ifndef SYMBOL_JOINER_H
#define SYMBOL_JOINER_H

#include "J_Calc_Symbol_Fwd_Decl.h"


namespace jomike{

class Symbol_Joiner{
public:
	virtual Symbol_Joiner* get_copy()const = 0;
	virtual ~Symbol_Joiner(){}
private:
};

}

#endif //SYMBOL_JOINER_H


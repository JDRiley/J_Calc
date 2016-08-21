#ifndef ADD_SYMBOL_JOINER_H
#define ADD_SYMBOL_JOINER_H

#include "Symbol_Joiner.h"


namespace jomike{

class Add_Symbol_Joiner : public Symbol_Joiner{
public:
	Add_Symbol_Joiner* get_copy()const override;
private:
};

Add_Symbol_Joiner* Add_Symbol_Joiner::get_copy()const{ return new Add_Symbol_Joiner(*this); }

}

#endif //ADD_SYMBOL_JOINER_H


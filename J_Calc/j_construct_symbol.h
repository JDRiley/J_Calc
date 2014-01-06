#ifndef J_CONSTRUCT_SYMBOL_H
#define J_CONSTRUCT_SYMBOL_H

#include "J_Calc_Fwd_Decl.h"

#include <j_symbol.h>

namespace jomike{

class j_construct_symbol : public j_symbol{
public:
private:
	virtual void derived_eval(const Arguments&);
};




}


#endif //!J_CONSTRUCT_SYMBOL_H
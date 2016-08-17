#ifndef J_SYMBOL_ERROR_H
#define J_SYMBOL_ERROR_H

#include "J_Calc_Symbol_Fwd_Decl.h"
//
#include <J_Error.h>
//
#include <string>
namespace jomike{
class J_Sym_Argument_Error : public J_Error{
public:
	J_Sym_Argument_Error(const std::string&  ik_message = "invalid args");
private:
};

class J_Symbol_Error : public J_Error{
public:
	J_Symbol_Error(const std::string& ik_message = "Bad Symbol Operation");
private:
};


}

#endif //J_SYMBOL_ERROR_H



#ifndef TYPE_FACTORY_H
#define TYPE_FACTORY_H

#include "J_Calc_Symbol_Fwd_Decl.h"
//
#include "Symbol_Types.h"
namespace jomike{

Type_Syntax* make_int_type_syntax(const yy::location& irk_loc);
Type_Syntax* make_double_type_syntax(const yy::location& irk_loc);
Type_Syntax* make_bool_type_syntax(const yy::location& irk_loc);
Type_Syntax* make_void_type_syntax(const yy::location& irk_loc);
Type_Syntax* make_string_type_syntax(const yy::location& irk_loc);
Type_Syntax* make_type_syntax(const yy::location& irk_loc, Symbol_Types i_symbol_type);
}

#endif //TYPE_FACTORY_H



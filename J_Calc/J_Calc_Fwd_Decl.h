#ifndef J_INTEGRATE_FWD_DECL_H
#define J_INTEGRATE_FWD_DECL_H

//Utilities
#include <J_Fwd_Decl.h>

namespace jomike{

J_FWD_DECL(j_function)
J_FWD_DECL(J_Calc_View)
J_FWD_DECL(J_Calc_Text_Subroutine)
J_FWD_DECL(J_Calc_Subroutine)
J_FWD_DECL(J_Calc_Data)
J_FWD_DECL(Math_Input_Box)
J_FWD_DECL(j_symbol)
J_FWD_DECL(j_value)
J_FWD_DECL(Line_Input)
J_FWD_DECL_LOWER(j_conditional_construct_symbol)

typedef j_symbol_Shared_t J_Symbol_Shared_t;
typedef j_value_Shared_t J_Value_Shared_t;
}



#endif
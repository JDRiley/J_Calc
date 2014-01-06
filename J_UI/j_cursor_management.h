#ifndef j_cursor_management
#define j_cursor_management


#include "J_UI_Fwd_Decl.h"

namespace jomike{


void j_set_cursor_type(int i_cursor_id);

const int J_NO_CURSOR_ID = 0;
const int J_NORMAL_CURSOR_ID = 1;
const int J_I_BEAM_CURSOR_ID = 2;
}

#endif //j_cursor_management


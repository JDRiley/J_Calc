#include "J_Calc_Subroutine.h"
#include <J_UI\J_Text_Box.h>

//Utilities
#include <cassert>

namespace jomike{

//J_Calc Subroutine Functions*************************************************
/*J_Calc_Subroutine()*/
J_Calc_Subroutine::J_Calc_Subroutine():M_finished_flag(false){}

//J_Calc_Text_Subroutine********************************************************
J_Calc_Text_Subroutine::J_Calc_Text_Subroutine(J_Text_Box_Shared_t i_text_box, int i_pos):
	M_text_box(i_text_box), M_cursor_begin_pos(i_pos), M_cursor_end_pos(i_pos){}

/*void key_input_cmd(j_window_t, int, int, int, int)*/
void J_Calc_Text_Subroutine::key_input_cmd(j_window_t, int , int , int , int ){
	assert(0);
}



/*void reset()*/
void J_Calc_Text_Subroutine::reset(){
	M_text_box.reset();
	M_cursor_end_pos = M_cursor_begin_pos;
}

}
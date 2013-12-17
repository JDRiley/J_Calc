#ifndef J_FUNCTION_CREATION_SUBROUTINE_H
#define J_FUNCTION_CREATION_SUBROUTINE_H

#include "J_Calc_Subroutine.h"

namespace jomike{


class Function_Creation_Subroutine : public J_Calc_Text_Subroutine{
public:
	Function_Creation_Subroutine(J_Text_Box_Shared_t i_text_box, int i_pos)
		:J_Calc_Text_Subroutine(i_text_box, i_pos){}

	void key_input_cmd(j_window_t, int key, int scancode, int action, int modifier)override;
	j_function* get_function();
private:
};


}


#endif
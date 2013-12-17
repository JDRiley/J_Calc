#ifndef INTEGRATION_SUBROUTINE_H
#define INTEGRATION_SUBROUTINE_H

#include "J_Calc_Subroutine.h"

namespace jomike{

class Integration_Subroutine : public J_Calc_Text_Subroutine{
public:
	Integration_Subroutine(J_Text_Box_Shared_t i_text_box, int i_pos)
		:J_Calc_Text_Subroutine(i_text_box, i_pos), M_func(nullptr){}
	void key_input_cmd(j_window_t, int key, int scancode, int action, int modifier)override;
	void reset()override;
private:
	j_function* M_func;
};



}


#endif
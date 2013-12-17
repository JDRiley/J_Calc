#ifndef J_CALC_SUBROUTINE_H
#define J_CALC_SUBROUTINE_H

#include "../J_Calc_Fwd_Decl.h"
//
#include <J_UI\J_UI_Fwd_Decl.h>

namespace jomike{

class J_Calc_Subroutine{
public:
	J_Calc_Subroutine();
	virtual void key_input_cmd(j_window_t, int key, int scancode, int action, int modifier)=0;
	bool is_finished()const{return M_finished_flag;}
	virtual void reset()=0;
	virtual ~J_Calc_Subroutine(){}
protected:
	bool set_finished(){M_finished_flag = true;}
private:
	bool M_finished_flag;
};

class J_Calc_Text_Subroutine: public J_Calc_Subroutine{
public:
	J_Calc_Text_Subroutine(J_Text_Box_Shared_t, int pos);
	void key_input_cmd(j_window_t, int key, int scancode, int action, int modifier)override;
	int get_begin_pos()const{return M_cursor_begin_pos;}
	int get_end_pos()const{return M_cursor_end_pos;}
	void reset()override;
protected:

private:
	J_Text_Box_Weak_t M_text_box;
	int M_cursor_begin_pos;
	int M_cursor_end_pos;
};






}

#endif
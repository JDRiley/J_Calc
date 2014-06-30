#ifndef MATH_INPUT_BOX_H
#define MATH_INPUT_BOX_H


//
#include "J_Calc_Fwd_Decl.h"
//
#include <J_UI\J_Text_Box.h>

//Containers
#include <ex_array.h>

//
#include "Line_Input.h"
namespace jomike{
extern const char LINE_END_SYMBOL;



class Math_Input_Box : public J_Text_Box{
public:
	//Constructors 
	Math_Input_Box(const J_Rectangle& irk_rec
		, const J_UI_Multi_String& irk_input_string
		, J_Font_Face i_output_font_face);

	bool insert_char(J_UI_Char);



private:
	static void math_key_input_cmd(J_Text_Box_Object_Shared_t i_math_box
		, int i_charcode
		, int i_scancode, int i_action, int i_modifiers);

	static void math_box_char_input_cmd(J_Text_Box_Object_Shared_t i_math_box
		, int i_charcode);

	void insert_integration_subroutine(j_size_t i_pos);
	typedef ex_array<Line_Input> Line_Input_Cont_t;
	typedef Line_Input_Cont_t::iterator Line_Input_it;
	Line_Input_Cont_t M_line_inputs;
	Line_Input_Cont_t::iterator lower_bound_line_input(j_size_t pos);
	Line_Input_Cont_t::iterator get_line_pos_at_pos(j_size_t pos);
	Line_Input& get_line_at_pos(j_size_t pos);
	Line_Input_it join_to_next(Line_Input_it);
	void key_backspace();
	void key_delete();
	void insert_line_input(const Line_Input&);
	void eval_and_break_line_input();
	Line_Input_it clear_output(Line_Input_Cont_t::iterator);
};

}

#endif
#ifndef MULTI_STATE_TEXT_BOX_H
#define MULTI_STATE_TEXT_BOX_H

#include "J_Text_Box.h"


namespace jomike{

	
class Multi_State_Text_Box : public J_Text_Box_Object{
public:
	Multi_State_Text_Box();



	//String Maintenance
	void clear_string()override;
	bool insert_char(J_UI_Char i_char)override;

	void insert_string(const J_UI_String&)override;
	void insert_string(j_size_t pos, const J_UI_String&)override;

	void backspace()override;

	void delete_char()override;
	void erase_chars(j_size_t pos, j_size_t size)override;

	void set_cursor_pos(j_size_t i_cursor_pos)override;
	void set_cursor_color(const J_Color_RGBA<j_float>)override;
	void set_string(const J_UI_String& irk_string)override;

	void move_cursor_to_line_begin()override;
	void move_cursor_to_line_end()override;

	void set_left_click_on()override;
	void set_left_click_off()override;

	void update()override;
	//Cursor Maintenance
	void move_cursor(j_size_t)override;
	void alert_cursor_pos(Pen_Pos_FL_t i_pos)override;
	//UI Functionality
	void key_input_cmd(j_window_t, int charcode, int scancode, int action, int modifier)override;
	void char_input_cmd(j_window_t, int charcode)override;

	//Box Maintenance
	void set_left_bound()override;
	void set_right_bound()override;
	void set_top_bound()override;
	void set_bottom_bound()override;

	//Class Maintenance
	void add_flags(unsigned i_flags)override;
	void set_flags(unsigned i_flags)override;
	unsigned get_flags()const override;

	void set_cursor_visibility_status(bool i_status)override;
	bool cursor_visibility_status()const override;


	j_size_t get_cursor_pos()const override;

	void set_read_only_status(bool)override;
	bool read_only_status()const override;

	void set_colors(J_Color_RGBA<j_float> cursor_color
					, J_Color_RGBA<j_float> box_color, J_Color_RGBA<j_float> outline_color)override;


	void change_color_at_pos(int i_pos, J_UI_Color i_color)override;
	//String Accessory
	const J_UI_Multi_String& get_string()const override;
	void broadcast_current_state()const override;
	void mouse_button_release(int i_button, int i_modifiers, Pen_Pos_FL_t i_pos)override;
	void mouse_button_press(int i_button, int i_modifiers, Pen_Pos_FL_t i_pos)override;
	void set_rectangle(const J_Rectangle& irk_rec)override;
	void set_outline_visibility_status(bool i_status)override;
	void set_fill_visibility_status(bool i_status)override;

	//Multi State Interface
	virtual void add_state();
	virtual void add_states(j_size_t);
	virtual void set_state(j_size_t i_index);
	virtual j_size_t state()const;

	void set_fill_color(const J_Color_RGBA<j_float>& i_color)override;
	void set_outline_color(const J_Color_RGBA<j_float>& i_color)override;

	void set_key_input_command(Key_Input_Cmd_Func_t)override;
	void set_char_input_command(Char_Input_Cmd_Func_t i_command)override;

	Multi_State_Text_Box_Shared_t shared_from_this();
	void move_cursor_line_pos_up(j_size_t)override;
	void silent_set_cursor_pos(j_size_t i_cursor_pos)override;
	void move_cursor_line_pos_down(j_size_t i_move_val)override;
	j_uint get_cursor_line_id()const;
	bool auto_scrolling_status()const override;
	void set_auto_scrolling_status(bool i_status)override;
protected:
	bool is_cursor_pos_in_view(j_size_t i_pos)const;
private:
	ex_array<J_Text_Box_Shared_t> M_states;
	J_Text_Box_Shared_t M_current_text_box;
#ifdef VS_2013
	j_size_t M_current_state = 0;

#else
	j_size_t M_current_state;
	void default_initialization();





#endif // VS_2013

};


}

#endif //MULTI_STATE_TEXT_BOX_H



#ifndef J_TEXT_H
#define J_TEXT_H


#include "../J_UI_Fwd_Decl.h"
//
#include "../J_UI_Box.h"
//
#include <J_Image.h>

//Container
#include <ex_array.h>
//
#include <ordered_pair.h>
//
#include <J_UI/J_UI_String.h>
//
#include <J_Gadget_Classes.h>
//Utilities
#include <memory>
#include <utility>



namespace jomike{

J_FWD_DECL(Text_Section)

void standard_text_box_input_parser(J_Text_Box_Object_Shared_t, int i_key, int i_scancode
, int i_action, int i_modifiers);

void standard_char_input_cmd(J_Text_Box_Object_Shared_t, int i_charcode);

void numeric_entry_char_input_cmd(J_Text_Box_Object_Shared_t, int i_charcode);

class J_Text_Box_Object : public J_UI_Box{
public:
	typedef void(*Key_Input_Cmd_Func_t)(J_Text_Box_Object_Shared_t, int i_key, int i_scancode
		, int i_action, int i_modifiers);

	typedef void(*Char_Input_Cmd_Func_t)(J_Text_Box_Object_Shared_t, int i_charcode);

	J_Text_Box_Object(const J_Rectangle& irk_rectangle = J_Rectangle());
	virtual void clear_string() = 0;
	virtual bool insert_char(J_UI_Char i_char) = 0;

	virtual void insert_string(const J_UI_String&) = 0;
	virtual void insert_string(j_size_t pos, const J_UI_String&) =0;

	virtual void backspace() = 0;

	virtual void delete_char() = 0;
	virtual void erase_chars(j_size_t pos, j_size_t size) = 0;

	virtual void set_cursor_pos(j_size_t i_cursor_pos) = 0;
	virtual void set_cursor_color(const J_Color_RGBA<j_float>) = 0;
	virtual void set_string(const J_UI_String& irk_string) = 0;

	
	void enable_default_key_char_processing();
	virtual void set_left_click_on() = 0;
	virtual void set_left_click_off() = 0;

	virtual j_uint get_cursor_line_id()const = 0;

	//Cursor Maintenance
	virtual void move_cursor(j_size_t) = 0;
	virtual void move_cursor_line_pos_up(j_size_t) = 0;
	virtual void move_cursor_line_pos_down(j_size_t) = 0;
	virtual void move_cursor_to_line_begin() = 0;
	virtual void move_cursor_to_line_end() = 0;

	//Box Maintenance
	virtual void set_left_bound() = 0;
	virtual void set_right_bound() = 0;
	virtual void set_top_bound() = 0;
	virtual void set_bottom_bound() = 0;

	//Class Maintenance
	virtual void add_flags(unsigned i_flags) = 0; 
	virtual void set_flags(unsigned i_flags) = 0;
	virtual unsigned get_flags()const = 0;

	enum Text_Flags{
		BOTTOM_FIXED = 1, RIGHT_FIXED = 2
		, SCROLL = 4, DISPLAY_BOX = 8, WRITE = 16
	};


	virtual j_size_t get_cursor_pos()const = 0;

	virtual void set_read_only_status(bool) = 0;
	virtual bool read_only_status()const = 0;

	virtual void set_colors(J_Color_RGBA<j_float> cursor_color
		, J_Color_RGBA<j_float> box_color, J_Color_RGBA<j_float> outline_color) = 0;


	virtual void change_color_at_pos(int i_pos, J_UI_Color i_color) = 0;
	//String Accessory
	virtual const J_UI_Multi_String& get_string()const = 0;

	virtual void set_key_input_command(Key_Input_Cmd_Func_t) = 0;
	virtual void set_char_input_command(Char_Input_Cmd_Func_t) = 0;

	J_Text_Box_Object_Shared_t shared_from_this();

	virtual void set_cursor_visibility_status(bool i_status) = 0;
	virtual bool cursor_visibility_status()const = 0;
	virtual void silent_set_cursor_pos(j_size_t i_cursor_pos) =0;
	virtual bool auto_scrolling_status()const = 0;
	virtual void set_auto_scrolling_status(bool i_status) = 0;

	~J_Text_Box_Object();
protected:
	J_Text_Box_Object(const J_Rectangle&, j_uint);
private:
};


J_FWD_DECL(J_Text_Cursor_Blinker_Updater)

class J_Text_Box : public J_Text_Box_Object{
public:
	J_Text_Box(const J_Rectangle& irk_rectangle = J_Rectangle()
		, const J_UI_Multi_String& i_string = J_UI_Multi_String());

	J_Text_Box(const J_Rectangle&
		, const J_UI_Multi_String& i_string, j_uint i_ID);


	//String Maintenance
	void clear_string()override;
	bool insert_char(J_UI_Char i_char)override;

	void insert_string(const J_UI_String&)override;
	void insert_string(j_size_t pos, const J_UI_String&)override;

	void recalculate_letter_poses();

	void backspace()override;
		
	void delete_char()override;
	void erase_chars(j_size_t pos, j_size_t size)override;

	void set_cursor_pos(j_size_t i_cursor_pos)override;

	void set_cursor_line_position(j_size_t i_cursor_pos);

	void set_cursor_color(const J_Color_RGBA<j_float>)override;
	void set_string(const J_UI_String& irk_string)override;



	void set_left_click_on()override;
	void set_left_click_off()override;


	//Cursor Maintenance
	void move_cursor(j_size_t)override;

	//UI Functionality
	void key_input_cmd(int charcode, int scancode, int action, int modifier)override;
	void char_input_cmd(int i_charcode)override;

	//Box Maintenance
	void set_left_bound()override;
	void set_right_bound()override;
	void set_top_bound()override;
	void set_bottom_bound()override;

	//Class Maintenance
	void add_flags(unsigned i_flags)override;
	void set_flags(unsigned i_flags)override;
	unsigned get_flags()const override;



	void move_cursor_line_pos_up(j_size_t)override;
	void move_cursor_line_pos_down(j_size_t)override;
	void move_cursor_to_line_begin()override;
	void move_cursor_to_line_end()override;


	j_size_t get_cursor_pos()const override;

	void set_read_only_status(bool)override;
	bool read_only_status()const override;

	void set_cursor_visibility_status(bool i_status)override;
	bool cursor_visibility_status()const override;

	void set_colors(J_Color_RGBA<j_float> cursor_color
		, J_Color_RGBA<j_float> box_color, J_Color_RGBA<j_float> outline_color)override;

	void set_key_input_command(Key_Input_Cmd_Func_t i_command)override;
	void set_char_input_command(Char_Input_Cmd_Func_t i_command)override;

	void change_color_at_pos(int i_pos, J_UI_Color i_color)override;
	//String Accessory
	const J_UI_Multi_String& get_string()const override;
	void broadcast_current_state()const override;

	void notify_letter_box_poses(j_size_t i_pos = J_SIZE_T_ZERO)const;
	void mouse_button_release(int i_button, int, Pen_Pos_FL_t i_pos)override;
	void mouse_button_press(int i_key, int i_modifiers, Pen_Pos_FL_t i_pen_pos)override;
	void mouse_button_press_n(int i_key, int i_modifiers, Pen_Pos_FL_t i_pen_pos, int i_count)override;
	void silent_set_cursor_pos(j_size_t i_cursor_pos);
	void enable_blinking_cursor();
	J_Text_Box_Shared_t shared_from_this();
	j_uint get_cursor_line_id()const override;
	void auto_scroll_window(j_size_t i_pos);

	bool auto_scrolling_status()const override;
	void set_auto_scrolling_status(bool i_status)override;
	void alert_changed();

	void alert_cursor_pos(Pen_Pos_FL_t i_pos)override;
protected:
	bool is_cursor_pos_in_view(j_size_t i_pos)const;
	void set_cursor_pos_no_scroll(j_size_t i_pos);
	void insert_string_silent(j_size_t i_index, const J_UI_String& irk_string);
	j_size_t selection_start()const;
	j_size_t selection_end()const;
private:
	
		


	
	void set_cursor_on();
	


	void notify_string_data()const;

	void scroll_selection_boxes(j_float i_x_scroll, j_float i_y_scroll);

	void initialize();
	J_Text_Cursor_Blinker_Updater_Shared_t M_blinker_updater;
	J_Text_Box& operator=(const J_Text_Box&);
	void calculate_remaining_letter_poses();
	
	
	Pen_Pos_FL_t default_pen_pos()const;
	Pen_Pos_FL_t new_line_pen_pos(Pen_Pos_FL_t i_cur_pen)const;
	j_float new_line_screen_size()const;
	Pen_Pos_FL_t calculate_pen_advance(Pen_Pos_FL_t i_cur_pen
									, int i_advance)const;
	
	void set_starting_pen_pos(Pen_Pos_FL_t i_pen_pos);
	void add_string(const J_UI_String& irk_string);
	j_size_t get_cursor_index(Pen_Pos_FL_t i_pen_pos)const;

	
	void scroll(int i_scroll_val);

	
	int lines_scrolled_per_tick()const;
	void set_selection_box_settings(J_UI_Box_Shared_t i_box)const;
	J_Line get_cursor_line(const Pen_Pos_FL_t& M_pen_poses)const;
	void clear_selection_boxes();
	void set_selection_box_visibility_statuses();
	

	int M_text_state;
	mutable bool M_changed_flag = true;
	bool M_auto_scrolling_status = true;

	int M_new_line_size = 30;

	J_UI_Multi_String M_multi_string;
	Array_Modifier_Manger<ex_array<J_UI_Letter_Box_Shared_t>> M_letter_box_string;
	ex_array<J_UI_Box_Shared_t> M_selection_boxes;
	ex_array<Pen_Pos_FL_t> M_pen_poses;

	Pen_Pos_FL_t M_last_set_cursor_pos;
	j_size_t M_cursor_pos;
	J_UI_Line_Shared_t M_cursor_line;

	ordered_pair<j_size_t> M_selection;
	

	J_GL_Framebuffer M_framebuffer;
	J_GL_Texture_Render_Buffer M_texture_render_buffer;
	Key_Input_Cmd_Func_t M_key_input_command;
	Char_Input_Cmd_Func_t M_char_input_command;

};


}
#endif
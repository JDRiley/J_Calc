#ifndef J_TEXT_H
#define J_TEXT_H

#include "../J_Image/J_Image.h"
#include "../J_UI_Fwd_Decl.h"
#include "../J_UI_Object.h"
//Container
#include <ex_array.h>
//
#include <ordered_pair.h>
//
#include <J_UI/J_UI_String.h>

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

	

	//Cursor Maintenance
	virtual void move_cursor(j_size_t) = 0;
	virtual void move_cursor_up(j_size_t) = 0;
	virtual void move_cursor_down(j_size_t) = 0;
	virtual void move_cursor_begin_of_line() = 0;
	virtual void move_cursor_end_of_line() = 0;

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
	~J_Text_Box_Object();
protected:
	J_Text_Box_Object(const J_Rectangle&, j_uint);
private:
};

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

	void move_cursor_begin_of_line()override;
	void move_cursor_end_of_line()override;

	void set_left_click_on()override;
	void set_left_click_off()override;

	void update()override;
	//Cursor Maintenance
	void move_cursor(j_size_t)override;

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
	virtual void move_cursor_up(j_size_t);
	void silent_set_cursor_pos(j_size_t i_cursor_pos)override;
	virtual void move_cursor_down(j_size_t);
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

		void backspace()override;
		
		void delete_char()override;
		void erase_chars(j_size_t pos, j_size_t size)override;

		void set_cursor_pos(j_size_t i_cursor_pos)override;
		void set_cursor_color(const J_Color_RGBA<j_float>)override;
		void set_string(const J_UI_String& irk_string)override;



		void set_left_click_on()override;
		void set_left_click_off()override;


		//Cursor Maintenance
		void move_cursor(j_size_t)override;

		//UI Functionality
		void key_input_cmd(j_window_t, int charcode, int scancode, int action, int modifier)override;
		void char_input_cmd(j_window_t, int i_charcode)override;

		//Box Maintenance
		void set_left_bound()override;
		void set_right_bound()override;
		void set_top_bound()override;
		void set_bottom_bound()override;

		//Class Maintenance
		void add_flags(unsigned i_flags)override;
		void set_flags(unsigned i_flags)override;
		unsigned get_flags()const override;



		void move_cursor_up(j_size_t)override;
		void move_cursor_down(j_size_t)override;
		void move_cursor_begin_of_line()override;
		void move_cursor_end_of_line()override;


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
		void silent_set_cursor_pos(j_size_t i_cursor_pos);
		void enable_blinking_cursor();
		J_Text_Box_Shared_t shared_from_this();
protected:
	

private:
	
		int M_text_state;

		J_UI_Multi_String M_multi_string;

		j_size_t M_cursor_pos;

		J_Color_RGBA<j_float> M_cursor_color;

		bool M_cursor_visibility_status;

		void set_cursor_on();


		Key_Input_Cmd_Func_t M_key_input_command;
		Char_Input_Cmd_Func_t M_char_input_command;

		bool M_left_click_is_on;

		bool M_saved_outline_visibility_status;
		void initialize();
		J_Text_Cursor_Blinker_Updater_Shared_t M_blinker_updater;
		J_Text_Box& operator=(const J_Text_Box&);
		
		
		ordered_pair<j_size_t> M_selection;
};


}
#endif
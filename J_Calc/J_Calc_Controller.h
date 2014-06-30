#ifndef J_INTEGRATOR_H
#define J_INTEGRATOR_H

#include "J_Calc_Fwd_Decl.h"
//
#include <J_UI/J_UI_Controller.h>
//
#include <J_Utile.h>


//Containers
#include <map>


namespace jomike{

void cursor_pos_callback(j_window_t i_window, j_dbl, j_dbl);
void char_callback(j_window_t i_window, j_uint i_charcode);
void key_callback(j_window_t i_window, int i_charcode, int, int, int);
void window_close_callback(j_window_t i_window);
void resize_callback(j_window_t i_window, int i_width, int i_height);
void main_mouse_button_callback(j_window_t i_window, int i_mouse_button, int i_action, int i_modifiers);


class J_Calc_Controller : public J_UI_Controller{
public:
	void execute();

private:
	friend void cursor_pos_callback(j_window_t i_window, j_dbl, j_dbl);
	friend void char_callback(j_window_t i_window, j_uint i_charcode);
	friend void key_callback(j_window_t i_window, int i_charcode, int, int, int);
	friend void window_close_callback(j_window_t i_window);
	friend void resize_callback(j_window_t i_window, int i_width, int i_height);
	friend void main_mouse_button_callback(j_window_t i_window, int i_mouse_button, int i_action, int i_modifiers);

	
	void poll_events()const;
	void wait_events()const;
protected:
	//Accessors
	J_Calc_View_Shared_t get_view(j_window_t);

	
	void window_close_cmd(j_window_t);

	void integral_input_cmd(j_window_t, int key, int scancode, int action, int modifier);
	void function_input_cmd(j_window_t, int charcode, int, int, int);
	void text_key_input_cmd(j_window_t, int charcode, int, int, int);

	
	void clear_all()override;
	~J_Calc_Controller();
private:
	static J_Calc_Controller& get_instance();
	J_Calc_Controller();
	friend class Instance_Pointer<J_Calc_Controller>;

	void derived_init(int argc, char** argv)override;

	void initialize_font_faces();
	J_Font_Face M_input_font_face = nullptr;
	J_Font_Face M_log_font_face = nullptr;
	J_Font_Face M_message_font_face = nullptr;

	
	bool M_continue_flag;
	void end_execute();


	J_Calc_View_Shared_t M_main_view;
	void remove_view(j_window_t);

	typedef 
		void (J_Calc_Controller::*Text_Key_Cmd_fp_t)(j_window_t, int charcode, int, int, int);
	
	
};

}

#endif
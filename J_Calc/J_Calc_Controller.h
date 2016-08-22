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



class J_Calc_Controller : public J_UI::J_UI_Controller{
public:
	void construct_ui();
	void execute();

private:

	
	void poll_events()const;
	void wait_events()const;
protected:
	//Accessors
	

	
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

	

	void initialize_font_faces();
	J_UI::J_Font_Face M_input_font_face = nullptr;
	J_UI::J_Font_Face M_log_font_face = nullptr;
	J_UI::J_Font_Face M_message_font_face = nullptr;

	


	
	

	typedef 
		void (J_Calc_Controller::*Text_Key_Cmd_fp_t)(j_window_t, int charcode, int, int, int);
	
	
};

}

#endif
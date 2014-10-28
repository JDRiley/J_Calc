#include "J_Calc_Controller.h"
//
#include "J_Calc_View.h"
//
#include "J_Calc_Subroutine\J_Calc_Subroutine.h"
//
#include "J_Calc_Subroutine\Integration_Subroutine.h"
//
#include "J_Calc_Subroutine\Function_Creation_Subroutine.h"
//
#include "J_Calc_Data.h"
//
#include <J_Error.h>
//
#include <J_OpenGl_Functions.h>
//
#include <J_UI/J_Text_Box.h>
//
#include <J_String.h>
//
#include <GLFW\glfw3.h>
//
#include <J_UI/J_Font_Manager.h>
//
#include "Math_Input_Box.h"
//
#include <J_UI/J_Font_Face.h>
//
#include <J_UI/J_UI.h>
//Algorithms
#include <algorithm>
//
#include <functional>
using std::any_of; using std::bind; using std::equal_to; using std::for_each;
using std::lower_bound; using std::mem_fn;using std::ref;

using namespace std::placeholders;

//Containers
using std::map;


//Facilities
#include <cassert>

//IO Facilities
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
using std::cerr; using std::ios; using std::ifstream; using std::stringstream; using std::setw;


//Utilities
#include <utility>




using std::pair;

const int INPUT_FONT_SIZE = 36;


namespace jomike{
static Instance_Pointer<Contexts_Handler> s_contexts;
static Instance_Pointer<J_Calc_Data> s_calc_data;
static Instance_Pointer<J_Calc_Controller> s_controller;
static Instance_Pointer<J_Font_Manager> s_font_manager;
static Instance_Pointer<J_UI> s_j_ui;
void cursor_pos_callback(j_window_t i_window, j_dbl, j_dbl);
void char_callback(j_window_t i_window, j_uint i_charcode);
void key_callback(j_window_t i_window, int i_charcode, int, int, int);
void window_close_callback(j_window_t i_window);
void resize_callback(j_window_t i_window, int i_width, int i_height);
void main_mouse_button_callback(j_window_t i_window, int i_mouse_button, int i_action, int i_modifiers);
void scroll_callback(j_window_t, j_dbl, j_dbl);
const j_dbl DRAW_REFRESH_TIME = 1.0 / 60.0;
const j_dbl PROGRAM_REFRESH_TIME = 1.0 / 2000.0;
static j_dbl draw_refresh_time(){ return DRAW_REFRESH_TIME; }
static j_dbl model_refresh_time(){ return PROGRAM_REFRESH_TIME; }
J_UI_Controller& J_UI_Controller::get_instance(){
	return *s_controller;
}

/*J_Calc_Controller Constructor*/
J_Calc_Controller::J_Calc_Controller():M_continue_flag(true){
	J_Calc_View_Shared_t new_view(new J_Calc_View(1400, 800, "J_Calc"));
	
	//J_Calc_View_Shared_t new_view(new J_Calc_View(1407, 813, "J_Calc"));
	M_main_view = new_view;
	add_view(M_main_view);
	M_main_view->make_active_context();

	initialize_font_faces();

	M_main_view->make_active_context();
	Math_Input_Box_Shared_t main_text_box_ptr(
		new Math_Input_Box(J_Rectangle(-0.95f, -0.70f, 1.90f, 1.50f)
		, J_UI_String(M_input_font_face, J_CYAN), M_input_font_face)
	);

	M_main_view->add_text_box(main_text_box_ptr);
	main_text_box_ptr->enable_blinking_cursor();
	main_text_box_ptr->set_clickable_status(true);
	main_text_box_ptr->set_read_only_status(false);
	main_text_box_ptr->set_outline_and_fill_visibility_status(true);

	main_text_box_ptr->set_colors(J_WHITE, J_CLEAR, J_CYAN);

	M_main_view->subscribe_cursor_updates(main_text_box_ptr);

	main_text_box_ptr->set_cursor_pos(0);
	M_main_view->set_focused_object(main_text_box_ptr);
	



	J_Text_Box_Shared_t cursor_pos_box(
			new J_Text_Box(J_Rectangle(0.50f, -1.0f, 0.50f, 0.1f)
			, J_UI_String("Cursor Box", M_log_font_face, J_WHITE))
		);

	M_main_view->add_text_box(cursor_pos_box);

	cursor_pos_box->set_outline_and_fill_visibility_status(true);
	cursor_pos_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);
	add_cursor_pos_text_updater(cursor_pos_box, M_main_view);
	J_Text_Box_Shared_t mode_text_box(
			new J_Text_Box(J_Rectangle(-0.25f, -1.0f, 0.75f, 0.1f)
			, J_UI_String("Normal Mode", M_log_font_face, J_WHITE))
		);

	M_main_view->add_text_box(mode_text_box);
	mode_text_box->set_outline_and_fill_visibility_status(true);
	mode_text_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);

	J_Text_Box_Shared_t draw_fps_text_box(
		new J_Text_Box(J_Rectangle(-0.65f, -1.0f, 0.4f, 0.1f)
		, J_UI_String("Draw FPS", M_log_font_face, J_WHITE))
		);

	M_main_view->add_text_box(draw_fps_text_box);
	draw_fps_text_box->set_outline_and_fill_visibility_status(true);
	draw_fps_text_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);

	add_draw_fps_updater(draw_fps_text_box);


	J_Text_Box_Shared_t update_fps_text_box(
		new J_Text_Box(J_Rectangle(-1.0f, -1.0f, 0.35f, 0.1f)
		, J_UI_String("Update FPS", M_log_font_face, J_WHITE))
		);

	M_main_view->add_text_box(update_fps_text_box);
	update_fps_text_box->set_outline_and_fill_visibility_status(true);
	update_fps_text_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);

	add_update_fps_updater(update_fps_text_box);

	assert(!open_gl_error());
}

void J_Calc_Controller::initialize_font_faces(){
	M_message_font_face
		= s_font_manager->get_font_face("times", 14);
	M_log_font_face
		= s_font_manager->get_font_face("times_italic", 20);
	M_input_font_face
		= s_font_manager->get_font_face("consolas", 14);
}

void J_Calc_Controller::execute(){
	M_main_view->open_window();
	
#ifdef _DEBUG
	run_script("button_press.jcs");
	//run_script("button_press.jcs");
#else
	run_script("button_press.jcs");
#endif


	draw_views();
	j_block_execution(10);


	J_Duration_Tester<j_dbl(*)(void), j_dbl(*)(void)>
		draw_timer(get_j_ui_time, draw_refresh_time);


	J_Duration_Tester<j_dbl(*)(void), j_dbl(*)(void)>
		update_timer(j_get_time, model_refresh_time);


	while(M_continue_flag){
		poll_events();

		if(update_timer.time_exceeded()){
			s_calc_data->update();
			s_j_ui->update();
			M_main_view->update();
			update_timer.reset_timer();

		} else{
			j_block_execution(5);
		}

		if(!has_views()){
			end_execute();
			continue;
		}
		if(draw_timer.time_exceeded()){
			draw_views();
			draw_timer.reset_timer();
		}


		
	}

	return;
}

J_Calc_Controller& J_Calc_Controller::get_instance(){
	static J_Calc_Controller j_calc_controller;
	return j_calc_controller;
}

J_Calc_View_Shared_t J_Calc_Controller::get_view(j_window_t i_window){
	assert(M_main_view->get_window() == i_window);
	(void)i_window;

	return M_main_view;
}

void J_Calc_Controller::remove_view(j_window_t i_window){

	J_UI_Controller::remove_view(i_window);

	if(M_main_view->get_window() == i_window){
		glfwSetCursorPosCallback(i_window, nullptr);
		end_execute();
	}

}

void J_Calc_Controller::end_execute(){
	M_continue_flag = false;
}

void J_Calc_Controller::poll_events()const{
	glfwPollEvents();
}

void J_Calc_Controller::wait_events()const{
	glfwWaitEvents();
}


//Call Backs***********************************************************************************

J_Calc_Controller::~J_Calc_Controller(){
	clear_all();
	M_main_view.reset();
	j_free_glfw();
	delete M_input_font_face;
	delete M_log_font_face;
	delete M_message_font_face;
}

void J_Calc_Controller::clear_all(){
	J_UI_Controller::clear_all();
	

}



void J_Calc_Controller::derived_init(int , char** ){
	auto window = M_main_view->get_window();
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowCloseCallback(window, window_close_callback);
	glfwSetWindowSizeCallback(window, resize_callback);
	glfwSetCursorPosCallback(window, cursor_pos_callback);
	glfwSetMouseButtonCallback(window, main_mouse_button_callback);
	glfwSetCharCallback(window, char_callback);
	glfwSetScrollCallback(window, scroll_callback);
}

/*void main_mouse_button_callback(j_window_t , int , int , int )*/
void main_mouse_button_callback(j_window_t i_window, int i_mouse_button, int i_action, int i_modifiers){
	s_controller->mouse_button_cmd(i_window, i_mouse_button, i_action, i_modifiers);
}

/*void key_callback(j_window_t i_window, int i_key, int i_scancode, int i_action, int i_modifiers)*/
void key_callback(j_window_t i_window, int i_key, int i_scancode
				  , int i_action, int i_modifiers){
	s_controller->key_input_cmd(i_window, i_key, i_scancode, i_action, i_modifiers);
}




/*window_close_callback(GLFWwindow*)*/
void window_close_callback(j_window_t i_window){
	s_controller->remove_view(i_window);
}

/*char_callback*/
void char_callback(j_window_t i_window, j_uint i_charcode){
	s_controller->char_input_cmd(i_window, i_charcode);
}



/*void resize_callback(j_window_t, int, int)*/
void resize_callback(j_window_t i_window, int i_width, int i_height){
	s_controller->resize_cmd(i_window, i_width, i_height);
}

/*cursor_pos_callback(j_window_t, j_dbl, j_dbl)*/
void cursor_pos_callback(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos){
	s_controller->cursor_pos_input_cmd(i_window, i_x_pos, i_y_pos);
}

void scroll_callback(j_window_t i_window, j_dbl i_x_scroll, j_dbl i_y_scroll){
	/*Hot fix for the mouse wheel up commands. noted as JTL Additions*/
	/*in the J_UI_Fwd_Decl.h header file*/
	if(!i_y_scroll){
		//Only Processing vertical scrolling events
		(void)i_x_scroll; //Compiler warning ridding
		return;
	}

	j_key_id_t wheel_button_key = i_y_scroll > 0 ? J_MOUSE_WHEEL_UP : J_MOUSE_WHEEL_DOWN;
	
	int num_scrolls = abs(static_cast<int>(i_y_scroll));
	int modifiers = s_controller->current_key_modifiers();
	if(num_scrolls > 1){
		s_controller->mouse_button_cmd_n(i_window, wheel_button_key, J_PRESS, modifiers, num_scrolls);
	} else{
		s_controller->mouse_button_cmd(i_window, wheel_button_key, J_PRESS, modifiers);
	}
}



}


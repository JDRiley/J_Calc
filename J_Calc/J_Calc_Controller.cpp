#include "J_Calc_Controller.h"
//
#include "J_Calc_View.h"
//
#include "J_Calc_Data.h"
//
#include <J_Error.h>
//
#include <JGL/J_OpenGl_Functions.h>
//
#include <J_UI_Object/J_Button.h>
//
#include <J_UI_Object/J_Text_Box.h>
//
#include <J_String.h>
//
#include <J_UI/J_Font_Manager.h>
//
#include "Math_Input_Box.h"
//
#include <J_UI/J_Font_Face.h>
//
#include <J_UI/J_UI_Curser_And_Fps.h>
//
#include <J_UI_Object/J_Vertical_Layout.h>
//
#include <J_UI_Object/J_Horizontal_Layout.h>
//Algorithms
#include <algorithm>
//
#include <functional>
//
#include <j_type.h>
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
using namespace J_UI;

using std::wostringstream;


namespace jomike{
static Instance_Pointer<J_Calc_Controller> s_controller;

}
namespace J_UI{

J_UI_Controller& J_UI_Controller::get_instance(){
	return *jtl::s_controller;
}

}

namespace jomike{
static Instance_Pointer<Contexts_Handler> s_contexts;
static Instance_Pointer<J_Calc_Data> s_calc_data;
static Instance_Pointer<J_Font_Manager> s_font_manager;
static Instance_Pointer<J_UI::J_UI_Curser_And_Fps> s_j_ui;

const j_dbl DRAW_REFRESH_TIME = 1.0 / 60.0;
const j_dbl PROGRAM_REFRESH_TIME = 1.0 / 2000.0;
//static j_dbl draw_refresh_time(){ return DRAW_REFRESH_TIME; }
//static j_dbl model_refresh_time(){ return PROGRAM_REFRESH_TIME; }



/*J_Calc_Controller Constructor*/
void J_Calc_Controller::construct_ui(){
	J_Calc_View_Shared_t new_view(new J_Calc_View(1400, 800, "J_Calc"));
	

	add_view(new_view);
	new_view->make_active_context();

	

	new_view->make_active_context();
	Math_Input_Box* main_text_box_ptr =
		new Math_Input_Box(J_UI_String(M_input_font_face, J_CYAN));


	J_UI::J_Vertical_Layout* layout = new J_UI::J_Vertical_Layout;
	new_view->set_layout(layout);

	layout->insert(main_text_box_ptr, layout->count(), Alignment::LEFT, Alignment::TOP,90.0f);



	
	main_text_box_ptr->enable_blinking_cursor();
	main_text_box_ptr->set_clickable_status(true);
	main_text_box_ptr->set_read_only_status(false);
	main_text_box_ptr->set_outline_and_fill_visibility_status(true);

	main_text_box_ptr->set_colors(J_WHITE, J_CLEAR, J_CYAN);

	main_text_box_ptr->set_cursor_pos(0);

	//M_main_view->set_focused_object(main_text_box_ptr);
	

	J_Horizontal_Layout* horizontal_layout
		= new J_Horizontal_Layout;
	layout->insert(horizontal_layout, layout->count(), Alignment::LEFT, Alignment::BOTTOM, 1.0f);

	J_Text_Box* cursor_pos_box =
		new J_Text_Box(J_UI_String("Cursor Box", M_log_font_face, J_WHITE));
			

	auto old_size_constraint = cursor_pos_box->size_constraints();
	cursor_pos_box->set_size_constraints({old_size_constraint.width_constraint(), J_UI::Dimensional_Constraint(32,32)});
	horizontal_layout->insert(cursor_pos_box, horizontal_layout->count(), Alignment::LEFT, Alignment::BOTTOM);

	
	
	cursor_pos_box->set_outline_and_fill_visibility_status(true);
	cursor_pos_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);

	
	J_Cursor_Position_Shared_t cursor_pos(new J_Cursor_Position);

	auto cursor_pos_box_updater
		= [cursor_pos_box, cursor_pos](J_Signal* y_signal){
		Cursor_Pos_Signal* signal
			= safe_dynamic_cast<J_Signal, Cursor_Pos_Signal>(y_signal);

		if(*cursor_pos == signal->cursor_position()){
			return;
		}

		*cursor_pos = signal->cursor_position();

		
		wostringstream o_str;
		auto width_manip = setw(8);
		o_str.precision(3);
		o_str << std::fixed;
		o_str << '\t' << cursor_pos->screen_pos().first
			<< " : " << cursor_pos->screen_pos().second;

		auto font_face = cursor_pos_box->get_string().front().font_face();
	
		cursor_pos_box->set_string(J_UI_String(o_str.str().c_str(), font_face));
		//std::cout << "\n" << o_str.str() << std::endl;
	};

	link_signal(new_view->cursor_pos_signal(), cursor_pos_box_updater);

	J_Text_Box* mode_text_box =
		new J_Text_Box(
			J_UI_String("Normal Mode", M_log_font_face, J_WHITE)
			, J_Rectangle<j_float>(-0.25f, -1.0f, 0.75f, 0.1f));


	horizontal_layout->insert(mode_text_box);

	mode_text_box->set_outline_and_fill_visibility_status(true);
	mode_text_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);

	J_Text_Box* draw_fps_text_box =
		new J_Text_Box(
			J_UI_String("Draw FPS", M_log_font_face, J_WHITE)
			, J_Rectangle<j_float>(-0.65f, -1.0f, 0.4f, 0.1f)
		);

	horizontal_layout->insert(draw_fps_text_box);
	draw_fps_text_box->set_outline_and_fill_visibility_status(true);
	draw_fps_text_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);

	

	auto draw_fps_updater
		= [draw_fps_text_box](J_UI_Object* y_obj){
		assert(draw_fps_text_box == y_obj);
		(void)y_obj;
		
		
		wostringstream o_str;
		auto width_manip = setw(8);
		o_str.precision(4);
		o_str << std::fixed;
		o_str << "Draw FPS: " << s_controller->fps();

		auto font_face = draw_fps_text_box->get_string().front().font_face();

		draw_fps_text_box->set_string(J_UI_String(o_str.str().c_str(), font_face));
	
	};

	draw_fps_text_box->add_update_callback(draw_fps_updater);

	J_Text_Box* update_fps_text_box =
		new J_Text_Box(
			J_UI_String("Update FPS", M_log_font_face, J_WHITE)
			, J_Rectangle<j_float>(-1.0f, -1.0f, 0.35f, 0.1f));


	horizontal_layout->insert(update_fps_text_box);
	
	update_fps_text_box->set_outline_and_fill_visibility_status(true);
	update_fps_text_box->set_colors(J_CLEAR, J_BLACK, J_CYAN);


	auto update_fps_updater
		= [update_fps_text_box](J_UI_Object* y_obj){
		assert(update_fps_text_box == y_obj);
		(void)y_obj;

		wostringstream o_str;
		auto width_manip = setw(8);
		o_str.precision(4);
		o_str << std::fixed;
		o_str << "FPS: " << s_j_ui->fps();

		auto font_face = update_fps_text_box->get_string().front().font_face();

		update_fps_text_box->set_string(J_UI_String(o_str.str(), font_face));
	};



	update_fps_text_box->add_update_callback(update_fps_updater);

	horizontal_layout->calculate_size_restraints();


	J_UI_Size_Constraints size_contraints = horizontal_layout->size_constraints();
	assert(!open_gl_error());

	layout->apply_style_sheet();
}

void J_Calc_Controller::initialize_font_faces(){
	M_message_font_face
		= s_font_manager->get_font_face("times", 14);
	M_log_font_face
		= s_font_manager->get_font_face("times_italic", 20);
	M_input_font_face
		= s_font_manager->get_font_face("consola", 14);
}

void J_Calc_Controller::execute(){
	
	
#ifdef _DEBUG
	//run_script("loop_test.jcs");
	//run_script("button_press.jcs");
#else
	run_script("button_press.jcs");
#endif

	j_block_execution(10);

	J_UI_Controller::execute();

	return;
}

J_Calc_Controller& J_Calc_Controller::get_instance(){
	static J_Calc_Controller j_calc_controller;
	return j_calc_controller;
}








J_Calc_Controller::J_Calc_Controller(){
	initialize_font_faces();
}

//Call Backs***********************************************************************************

J_Calc_Controller::~J_Calc_Controller(){
	delete M_input_font_face;
	delete M_log_font_face;
	delete M_message_font_face;
}

void J_Calc_Controller::clear_all(){
	J_UI_Controller::clear_all();
}





//
//void scroll_callback(j_window_t i_window, j_dbl i_x_scroll, j_dbl i_y_scroll){
//	/*Hot fix for the mouse wheel up commands. noted as JTL Additions*/
//	/*in the J_UI_Fwd_Decl.h header file*/
//	if(!i_y_scroll){
//		//Only Processing vertical scrolling events
//		(void)i_x_scroll; //Compiler warning ridding
//		return;
//	}
//
//	j_key_id_t wheel_button_key = i_y_scroll > 0 ? J_MOUSE_WHEEL_UP : J_MOUSE_WHEEL_DOWN;
//	
//	int num_scrolls = abs(static_cast<int>(i_y_scroll));
//	int modifiers = s_controller->current_key_modifiers();
//	if(num_scrolls > 1){
//		s_controller->mouse_button_cmd_n(i_window, wheel_button_key, J_PRESS, modifiers, num_scrolls);
//	} else{
//		s_controller->mouse_button_cmd(i_window, wheel_button_key, J_PRESS, modifiers);
//	}
//}



}


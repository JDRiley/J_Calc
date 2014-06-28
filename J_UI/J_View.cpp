#include "J_View.h"
//
#include "J_UI_Controller.h"
//
#include <J_OpenGl.h>
//
#include "J_FT_Text_Displayer\J_Text_Box.h"
//
#include "J_FT_Text_Displayer/Multi_State_Text_Box.h"
//
#include "J_UI_Line.h"
//
#include "J_Image_Pane.h"
//
#include "J_UI_Line.h"
//
#include "J_UI_Circle.h"
//Algorithms
#include <algorithm>
//
#include <functional>
using std::bind;using std::for_each;using std::greater; using std::mem_fn;  using std::sort;
using namespace std::placeholders; using std::remove_if;

//Containers
#include <string>
using std::string;

//Thread
#include <thread>

namespace this_thread = std::this_thread;


//IO Facilities
#include <fstream>
//
#include <iostream>

using std::cerr;using std::cout; using std::endl; using std::ifstream; 

//Utilities
#include <cassert>
//
#include <chrono>
//
#include <iterator>

using std::istreambuf_iterator; using std::shared_ptr;
namespace chrono = std::chrono;
using std::map;



namespace jomike{

static Instance_Pointer<Contexts_Handler> s_contexts;
static Instance_Pointer<J_UI_Controller> s_controller;
J_Context_Shared_t J_View::get_context(){return M_context;}


/*void make_active_context()*/
void J_View::make_active_context()const{
	s_contexts->make_context_active(M_context);
}

/*void clear_window()*/
void J_View::clear_window()const{
	j_clear();
}

void J_View::resize(int i_width, int i_height){

	j_view_port(0,0, i_width, i_height);
	draw_background();

	

	for_each(M_ui_object_stack.begin(), M_ui_object_stack.end()
		, bind(&J_UI_Object::alert_resize, _1, get_x_resolution(), get_y_resolution()));

}



const j_uint num_vertices = 6;
static j_int g_view_ids = 0;
J_View::J_View(int i_width, int i_height, const char* i_title
									 , j_monitor_t i_monitor, j_window_t i_share_window){

	M_context = s_contexts->create_j_context(i_width, i_height, i_title, i_monitor, i_share_window);
	M_view_id = ++g_view_ids;
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	M_ui_object_stack.set_front_buffer(0.5);
	//Do Not Initialize Call Backs here parce_que they refer back to 
	//views created in J_Integrate. Which may not be a fully formed object as this
	//constructor is called in the construction of the J_Integrator object
	
}

void J_View::add_ui_object(J_UI_Object_Shared_t i_ui_object){
	if(M_ui_objects.count(i_ui_object)){
		throw(J_Error("Display Object with this ID already found"));
	}

	M_ui_object_stack.push_back(i_ui_object);
	M_ui_objects.insert(i_ui_object);

	assert(M_ui_object_stack.size() == M_ui_objects.size());
}

/*void add_text_display(j_uint, J_FT_Text_Display_Shared_t)*/
void J_View::add_text_box(J_Text_Box_Object_Shared_t i_text_box){
	Context_RAII context_saver;
	make_active_context();
	if(M_text_box_objects.count(i_text_box)){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}

	add_ui_box(i_text_box);
	M_text_box_objects.insert(i_text_box);
}



void J_View::add_ui_box(J_UI_Box_Shared_t i_ui_box){
	if(M_disp_boxes.count(i_ui_box->get_ID())){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}
	add_ui_object(i_ui_box);

	M_disp_boxes[i_ui_box->get_ID()] = i_ui_box;
}





void J_View::add_ui_circle(J_UI_Circle_Shared_t i_display_circle){
	if (M_display_circles.count(i_display_circle->get_ID())){
		throw J_Argument_Error("View Already Has Circle Box With This ID");
	}

	add_ui_object(i_display_circle);
	M_display_circles[i_display_circle->get_ID()] = i_display_circle;
}

void J_View::add_multi_state_text_box(Multi_State_Text_Box_Shared_t i_multi_state_textbox){
	if(M_multi_state_text_boxes.count(i_multi_state_textbox->get_ID())){
		throw J_Argument_Error("View Already Has Multi Text Box With This ID");
	}


	add_text_box(i_multi_state_textbox);

	M_multi_state_text_boxes[i_multi_state_textbox->get_ID()] 
		= i_multi_state_textbox;
}
//
//void J_View::update_letter_box_rectangle(j_uint i_text_box_id, j_size_t i_index
//										 , const Pen_Pos_FL_t& i_pen_pos
//										 , const Bitmap_Metrics& i_metrics){
//	if(auto text_box = get_text_display(i_text_box_id)){
//		text_box->set_letter_box_rectangle(i_index, i_pen_pos, i_metrics);
//	}
//}
//
//
//
//void J_View::delete_char(j_uint i_text_box_id, j_size_t i_pos){
//	if(auto text_box = get_text_display(i_text_box_id)){
//		text_box->delete_char(i_pos);
//	}
//}
//
//
//
//void J_View::insert_char(j_uint i_text_box_id, j_size_t i_pos, const Pen_Pos_FL_t& i_pen_pos, const Bitmap_Metrics& i_metrics, const J_UI_Color& i_color, const j_ubyte* i_data){
//	if(auto text_box = get_text_display(i_text_box_id)){
//		text_box->add_letter_box(i_pos, i_pen_pos, i_metrics, i_color, i_data);
//	}
//}
//
//void J_View::erase_chars(j_uint i_text_box_id, j_size_t i_pos, j_size_t i_size){
//	if(auto text_box = get_text_display(i_text_box_id)){
//		text_box->erase_chars(i_pos, i_size);
//	}
//}

/*void J_View::mouse_button(int i_button, int i_action, int i_modifiers, Pen_Pos_FL_t i_pos)*/
j_uint J_View::mouse_button_press(J_View_Shared_t i_view, int i_button, int i_modifiers, Pen_Pos_FL_t i_pos){
	J_UI_Object_Shared_t disp_obj_ptr = get_display_object_at_pos(i_pos);
	if(!disp_obj_ptr){
		return 0;
	}

	disp_obj_ptr->mouse_button_press(i_button, i_modifiers, i_pos);
	switch(i_button){
	case J_MOUSE_BUTTON_LEFT:
		M_focused_object = disp_obj_ptr;
		break;
	default:
		;
	}
	//disp_obj_ptr->mouse_button_press(i_view, i_button, i_modifiers, i_pos);

	return disp_obj_ptr->get_ID();
}

j_uint J_View::mouse_button_press_n(J_View_Shared_t i_view, int i_button
									, int i_modifiers, Pen_Pos_FL_t i_pos, int i_count){
	J_UI_Object_Shared_t disp_obj_ptr = get_display_object_at_pos(i_pos);

	if(!disp_obj_ptr){
		return 0;
	}

	for(int i = 0; i < i_count; i++){
		disp_obj_ptr->mouse_button_press(i_button, i_modifiers, i_pos);
	}

	switch(i_button){
	case J_MOUSE_BUTTON_LEFT:
		M_focused_object = disp_obj_ptr;
		break;
	default:
		;
	}


	return disp_obj_ptr->get_ID();
}


void J_View::mouse_button_release(J_View_Shared_t i_view, int i_button, int i_modifiers, Pen_Pos_FL_t i_pos){

	switch(i_button){
	case J_LEFT_MOUSE_BUTTON:
		if(!M_focused_object.expired()){
			M_focused_object.lock()->mouse_button_release(i_button, i_modifiers, i_pos);
		}
		break;
	default:
		;
	}			
	
}

void J_View::display_object_pressed(J_UI_Object_Shared_t, int , int 
		, Pen_Pos_FL_t){}

J_UI_Object_Shared_t J_View::get_display_object_at_pos(Pen_Pos_FL_t i_pos){
	for(auto disp_obj_ptr : M_ui_object_stack){
		if(disp_obj_ptr->clickable_status() && disp_obj_ptr->is_inside(i_pos.first, i_pos.second)){
			return disp_obj_ptr;
		}
	}
	return J_UI_Object_Shared_t();
}

/*void open_window()*/
void J_View::open_window(){
	j_open_window(M_context);
}

void J_View::close_window(){
	j_close_window(M_context);
}

void J_View::set_context(J_Context_Shared_t i_context){M_context = i_context;}

/*void draw()const*/
void J_View::draw()const{
	Context_RAII context_saver;
	make_active_context();
	clear_window();
	draw_background();
	for(auto f_obj : M_ui_object_stack){
		f_obj->draw();
		assert(!open_gl_error());
	}

	j_swap_buffers(M_context);
}

/*void add_to_buffer()const*/
void J_View::add_to_buffer()const{
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	draw_background();
	for_each(M_ui_object_stack.begin(), M_ui_object_stack.end()
		, mem_fn(&J_UI_Object::draw));

}

void J_View::draw_background()const{
}

/*bool should_close()const*/
bool J_View::should_close()const{
	return j_should_window_close(M_context);
}

bool J_View::is_visible()const{
	return j_is_window_open(M_context);
}

void J_View::set_cursor_pos(int i_x, int i_y){
	M_cursor_pos.first = get_x_coord(i_x);
	M_cursor_pos.second = get_y_coord(i_y);
	for(auto cursor_pos_objects : M_objects_using_cursor_data){
		if(cursor_pos_objects.expired()){
			continue;
		}

		cursor_pos_objects.lock()->alert_cursor_pos(M_cursor_pos);
	}

}

//void J_View::set_fill_visibility(j_uint i_disp_obj, bool i_status){
//	if(auto disp_obj = get_display_object(i_disp_obj)){
//		disp_obj->set_fill_visibility_status(i_status);
//	}
//}
//
//void J_View::set_outline_visibility(j_uint i_disp_obj, bool i_status){
//	if (auto disp_obj = get_display_object(i_disp_obj)){
//		disp_obj->set_outline_visibility_status(i_status);
//	}
//}

/*~J_FT_Text_Displayer()*/ //Must be called on main thread and not in callback
J_View::~J_View(){
	Context_RAII context_saver;
	make_active_context();
	clear();
}

/*virtual void clear()*/
void J_View::clear(){
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	if(M_ui_object_stack.empty()){
		return;
	}

	M_ui_object_stack.clear(); 
	M_ui_objects.clear(); 
	M_text_box_objects.clear();
	M_display_circles.clear();
	M_image_panes.clear();
	M_disp_boxes.clear();
	M_focused_object.reset();

}
//
//J_UI_Object_Shared_t J_View::get_display_object(j_uint i_obj_id){
//	
//	auto found_pos = M_ui_objects.find(i_obj_id);
//	if(found_pos == M_ui_objects.end()){
//		return J_UI_Object_Shared_t();
//	}
//	make_active_context();
//	return found_pos->second;
//}

jomike::J_UI_Box_Shared_t J_View::get_display_box(j_uint i_obj_id){
	auto found_pos = M_disp_boxes.find(i_obj_id);
	if(found_pos == M_disp_boxes.end()){
		return J_UI_Box_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}

J_UI_Circle_Shared_t J_View::get_display_circle(j_uint i_circle_id){
	auto found_pos = M_display_circles.find(i_circle_id);
	if (found_pos == M_display_circles.end()){
		return J_UI_Circle_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}

//J_Text_Box_Shared_t J_View::get_text_display(j_uint i_obj_id){
//	auto found_pos = M_text_box_objects.find(i_obj_id);
//	if(found_pos == M_text_box_objects.end()){
//		return J_FT_Text_Display_Shared_t();
//	}
//	make_active_context();
//	return found_pos->second;
//}

Multi_State_Text_Box_Shared_t J_View::get_multi_state_text_display(j_uint i_obj_id){
	auto found_pos = M_multi_state_text_boxes.find(i_obj_id);
	if(M_multi_state_text_boxes.end() == found_pos){
		return Multi_State_Text_Box_Shared_t();
	}

	make_active_context();
	return found_pos->second;
}

//void J_View::update_box_coordinates(j_uint i_display_box_id, const J_Rectangle& irk_rectangle){
//	if(auto display_box = get_display_box(i_display_box_id)){
//		display_box->set_rectangle(irk_rectangle);
//	}
//}
//
//void J_View::update_fill_color(j_uint i_disp_obj_id, J_Color_RGBA<j_float> i_color){
//	if (auto disp_obj = get_display_object(i_disp_obj_id)){
//		disp_obj->set_fill_color(i_color);
//	}
//}
//
//void J_View::update_outline_color(j_uint i_disp_obj_id, J_Color_RGBA<j_float> i_color){
//	if (auto disp_obj = get_display_object(i_disp_obj_id)){
//		disp_obj->set_outline_color(i_color);
//	}
//}
//
//void J_View::update_outline_thickness(j_uint i_disp_object_id, j_float i_thickness){
//	if (auto disp_pbj = get_display_object(i_disp_object_id)){
//		disp_pbj->set_outline_thickness(i_thickness);
//	}
//}
//
//void J_View::update_circle_shape_data(j_uint i_circle_id, const J_Circle& irk_circle){
//	if (auto display_circle = get_display_circle(i_circle_id)){
//		display_circle->set_circle(irk_circle);
//	}
//	
//}
//
//void J_View::update_center(j_uint i_obj_id, j_float i_x_pos, j_float i_y_pos){
//	if (auto display_obj = get_display_object((i_obj_id))){
//		display_obj->set_center(i_x_pos, i_y_pos);
//	}
//}
//
//void J_View::update_radius(j_uint i_circle_id, j_float i_radius){
//	if (auto display_circle = get_display_circle(i_circle_id)){
//		display_circle->set_radius(i_radius);
//	}
//}

int J_View::get_x_resolution()const{
	return s_contexts->get_width(M_context);
}

int J_View::get_y_resolution()const{
	return s_contexts->get_height(M_context);
}

j_window_t J_View::get_window()const{
	return s_contexts->get_window(M_context);
}

//Image---------------------------------------------------------------------------------------
J_Image_Pane_Shared_t J_View::get_image_pane(j_uint i_obj_id){
	auto found_pos = M_image_panes.find(i_obj_id);
	if(found_pos == M_image_panes.end()){
		return J_Image_Pane_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}
//
//void J_View::update_image_clear(j_uint i_obj_id){
//	if(auto image_pane = get_image_pane(i_obj_id)){
//		image_pane->clear_image();
//	}
//}
//
//void J_View::update_image_buffer(j_uint i_obj_id, const j_ubyte* i_buffer){
//		if(auto image_pane = get_image_pane(i_obj_id)){
//		image_pane->set_buffer(i_buffer);
//	}
//}
//
//
//
//void J_View::update_image_height(j_uint i_obj_id, int i_height){
//	if(auto image_pane = get_image_pane(i_obj_id)){
//		image_pane->set_image_height(i_height);
//	}
//}
//
//void J_View::update_image_width(j_uint i_obj_id, int i_width){
//	if(auto image_pane = get_image_pane(i_obj_id)){
//		image_pane->set_image_width(i_width);
//	}
//}

//void J_View::update_image_buffer_format(j_uint i_obj_id, Image_Format i_format){
//	if(auto image_pane = get_image_pane(i_obj_id)){
//		image_pane->set_image_format(i_format);
//	}
//}
//
//void J_View::update_middle_line_color(j_uint i_obj_id, J_UI_Color i_color){
//	if(auto image_pane = get_image_pane(i_obj_id)){
//		image_pane->set_middle_line_color(i_color);
//	}
//}
//
//void J_View::update_add_multi_text_state(j_uint i_multi_text_state_id){
//	if(auto multi_text_state_box = get_multi_state_text_display(i_multi_text_state_id)){
//		multi_text_state_box->add_state();
//	}
//}
//
//void J_View::update_multi_text_state(j_uint i_multi_state_text_id, j_size_t i_state){
//	if(auto multi_state_text_box = get_multi_state_text_display(i_multi_state_text_id)){
//		multi_state_text_box->set_state(i_state);
//	}
//}
//
//
//void J_View::remove_display_line(j_uint i_line_id){
//	remove_display_object(i_line_id);
//	if(!M_display_lines.count(i_line_id)){
//		return;
//	}
//
//	M_display_lines.erase(i_line_id);
//}
//

//void J_View::remove_text_display(j_uint i_obj_id){
//	remove_display_box(i_obj_id);
//	if(!M_text_box_objects.count(i_obj_id)){
//		return;
//	}
//	M_text_box_objects.erase(i_obj_id);
//
//}
//void J_View::remove_display_box(j_uint i_obj_id){
//	remove_display_object(i_obj_id);
//	if(!M_disp_boxes.count(i_obj_id)){
//		return;
//	}
//
//	M_disp_boxes.erase(i_obj_id);
//}
//void J_View::remove_display_object(j_uint i_obj_id){
//	auto found_obj = M_ui_objects.find(i_obj_id);
//	if(M_ui_objects.end() == found_obj){
//		return;
//	}
//	M_ui_objects.erase(found_obj);
//#ifdef _DEBUG
//	j_size_t prev_size = M_ui_object_stack.size();
//#endif // _DEBUG
//	M_ui_object_stack.remove_if([i_obj_id](J_Display_Object_Shared_t i_obj){
//		return i_obj->get_ID() == i_obj_id;
//	});
//
//#ifdef _DEBUG
//	assert((prev_size - 1) == M_ui_object_stack.size());
//#endif // _DEBUG
//
//}
//
//void J_View::add_managed_display_object(j_uint i_obj_id, j_uint i_new_obj_id, UI_Object_Types i_obj_type){
//	if(is_display_object_present(i_obj_id)){
//		(this->*Ms_add_object_functions[i_obj_type])(i_new_obj_id);
//	}
//
//}
//
//void J_View::remove_managed_display_object(j_uint i_obj_id, UI_Object_Types i_obj_type){
//	if(is_display_object_present(i_obj_id)){
//		(this->*Ms_remove_object_functions[i_obj_type])(i_obj_id);
//	}
//}
//
//bool J_View::is_display_object_present(j_uint i_obj_id)const{
//	return M_ui_objects.count(i_obj_id);
//}
//
//void J_View::remove_display_circle(j_uint i_obj_id){
//	auto found_pos = M_display_circles.find(i_obj_id);
//	if(M_display_circles.end() == found_pos){
//		return;
//	}
//
//	M_display_circles.erase(found_pos);
//	remove_display_object(i_obj_id);
//}
//
//void J_View::remove_image_pane(j_uint i_obj_id){
//	auto found_pos = M_image_panes.find(i_obj_id);
//	if(M_image_panes.end() == found_pos){
//		return;
//	}
//
//	M_image_panes.erase(found_pos);
//	remove_display_box(i_obj_id);
//}
//
//void J_View::remove_multi_state_text_box(j_uint i_obj_id){
//	auto found_pos = M_multi_state_text_boxes.find(i_obj_id);
//	if(M_multi_state_text_boxes.end() == found_pos){
//		return;
//	}
//
//	M_multi_state_text_boxes.erase(found_pos);
//	remove_text_display(i_obj_id);
//}
//
//void J_View::update_line_data(j_uint i_obj_id, const J_Line& i_line){
//	if(auto line_ptr = get_display_line(i_obj_id)){
//		line_ptr->set_line(i_line);
//	}
//}
//
//J_Display_Line_Shared_t J_View::get_display_line(j_uint i_obj_id){
//	auto found_pos = M_display_lines.find(i_obj_id);
//	if(M_display_lines.end() == found_pos){
//		return J_Display_Line_Shared_t();
//	}
//	return found_pos->second;
//}
//
//void J_View::update_letter_box_data(j_uint i_text_box_id
//									, j_size_t i_index, const Bitmap_Metrics& i_metrics
//									, const J_UI_Color& i_color, const j_ubyte* i_data){
//	if(auto text_box = get_text_display(i_text_box_id)){
//		text_box->set_letter_box_data(i_index, i_metrics, i_color, i_data);
//	}
//}
//
//void J_View::update_text_string_size(j_uint i_obj_id, j_size_t i_size){
//	if(auto text_box = get_text_display(i_obj_id)){
//		text_box->set_text_string_size(i_size);
//	}
//}



void J_View::add_ui_line(J_UI_Line_Shared_t i_line_ptr){
	Context_RAII context_saver;
	make_active_context();
	if(M_display_lines.count(i_line_ptr->get_ID())){
		throw J_Argument_Error("View Already Has Line Box With This ID");
	}

	add_ui_object(i_line_ptr);
	M_display_lines[i_line_ptr->get_ID()] = i_line_ptr;

	
}
//
//void J_View::update_letter_box_poses(j_uint i_text_box_id, j_size_t i_pos
//									 , j_size_t i_size, const Pen_Pos_FL_t* i_poses){
//	if(auto text_box = get_text_display(i_text_box_id)){
//		text_box->set_letter_box_poses(i_pos, i_size, i_poses);
//	}
//}
//
//void J_View::update_text_string_add(j_uint i_text_box_id, j_size_t i_pos, j_size_t i_size, const Pen_Pos_FL_t* i_poses, Bitmap_Metrics** i_metrics
//									, const J_UI_Color& i_color, const j_ubyte* const * i_datas){
//	if(auto text_box = get_text_display(i_text_box_id)){
//		text_box->insert_text_string(i_pos, i_size, i_poses, i_metrics, i_color, i_datas);
//	}
//
//}

void J_View::subscribe_cursor_updates(J_UI_Object_Weak_t i_ui_obj){
	M_objects_using_cursor_data.insert(i_ui_obj);
}
//
//void J_View::position_after(j_uint i_front_pos_id, j_uint i_after_pos_id){
//	if(!M_ui_objects.count(i_front_pos_id)){
//		return;
//	}
//
//	assert(M_ui_objects.count(i_front_pos_id));
//	assert(M_ui_objects.count(i_after_pos_id));
//	assert(i_front_pos_id != i_after_pos_id);
//
//	J_Display_Object_Shared_t front_object = M_ui_objects[i_front_pos_id];
//	J_Display_Object_Shared_t after_object = M_ui_objects[i_after_pos_id];
//	assert(front_object);
//
//	
//	auto found_after_pos = find(M_ui_object_stack.begin(), M_ui_object_stack.end(), after_object);
//	
//	assert(M_ui_object_stack.end() != found_after_pos);
//
//	M_ui_object_stack.erase(found_after_pos);
//
//	auto found_front_pos = find(M_ui_object_stack.begin(), M_ui_object_stack.end(), front_object);
//	assert(M_ui_object_stack.end() != found_front_pos);
//	
//
//	M_ui_object_stack.insert(++found_front_pos, after_object);
//}
//
//void J_View::position_before(j_uint i_front_pos_id, j_uint i_before_pos_id){
//	if(!M_ui_objects.count(i_front_pos_id)){
//		return;
//	}
//	assert(M_ui_objects.count(i_front_pos_id));
//	assert(M_ui_objects.count(i_before_pos_id));
//	assert(i_front_pos_id != i_before_pos_id);
//
//	J_Display_Object_Shared_t front_object = M_ui_objects[i_front_pos_id];
//	J_Display_Object_Shared_t before_object = M_ui_objects[i_before_pos_id];
//	assert(front_object);
//
//
//	auto found_before_pos = find(M_ui_object_stack.begin(), M_ui_object_stack.end(), before_object);
//
//	assert(M_ui_object_stack.end() != found_before_pos);
//
//	M_ui_object_stack.erase(found_before_pos);
//
//	auto found_front_pos = find(M_ui_object_stack.begin(), M_ui_object_stack.end(), front_object);
//	assert(M_ui_object_stack.end() != found_front_pos);
//
//
//	M_ui_object_stack.insert(found_front_pos, before_object);
//}

void J_View::char_input_cmd(j_uint i_charcode){
	if(M_focused_object.expired()){
		return;
	}

	M_focused_object.lock()->char_input_cmd(i_charcode);
}

void J_View::update(){
	for(auto f_obj : M_ui_objects){
		f_obj->update();
	}
}

}


#include "J_View.h"
//
#include "J_UI_Controller.h"
//
#include <J_OpenGl.h>
//
#include "J_FT_Text_Displayer\J_FT_Text_Display.h"
//
#include "J_Display_Object\J_Display_Object.h"
//
#include "J_Display_Image_Pane.h"
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

#include <vector>


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

	

	for_each(M_disp_objs.begin(), M_disp_objs.end()
		, bind(&J_Display_Object::alert_resize, _1, get_x_resolution(), get_y_resolution()));

}

void J_View::set_clickable_status(j_uint i_obj_id, bool i_status){
	if(!M_disp_objs_by_id.count(i_obj_id)){
		return;
	}

	get_display_object(i_obj_id)->set_clickable_status(i_status);
}

const j_uint num_vertices = 6;
static j_int g_view_ids = 0;
J_View::J_View(int i_width, int i_height, const char* i_title
									 , j_monitor_t i_monitor, j_window_t i_share_window){

	M_context = s_contexts->create_j_context(i_width, i_height, i_title, i_monitor, i_share_window);
	M_view_id = ++g_view_ids;
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	M_disp_objs.set_front_buffer(0.5);
	//Do Not Initialize Call Backs here parce_que they refer back to 
	//views created in J_Integrate. Which may not be a fully formed object as this
	//constructor is called in the construction of the J_Integrator object
	
}

void J_View::add_display_object(J_Display_Object_Shared_t i_disp_obj_ptr){
	if(M_disp_objs_by_id.count(i_disp_obj_ptr->get_object_ID())){
		throw(J_Error("Display Object with this ID already found"));
	}

	M_disp_objs.push_back(i_disp_obj_ptr);
	M_disp_objs_by_id[i_disp_obj_ptr->get_object_ID()] = i_disp_obj_ptr;
	assert(M_disp_objs.size() == safe_cast<j_size_t>(M_disp_objs_by_id.size()));
}

/*void add_text_display(j_uint, J_FT_Text_Display_Shared_t)*/
void J_View::add_text_display(j_uint i_text_box_id){
	Context_RAII context_saver;
	make_active_context();
	if(M_text_displays.count(i_text_box_id)){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}
	J_FT_Text_Display_Shared_t new_text_display(new J_FT_Text_Display(i_text_box_id));
	add_display_box(new_text_display);
	M_text_displays[i_text_box_id] = new_text_display;
}

void J_View::add_image_pane(j_uint i_image_pane_id){
	if(M_image_panes.count(i_image_pane_id)){
		throw J_Argument_Error("View Already Has Image Box With This ID");
	}
	J_Display_Image_Pane_Shared_t new_image_pane(new J_Display_Image_Pane(i_image_pane_id));
	add_display_box(new_image_pane);
	M_image_panes[i_image_pane_id] = new_image_pane;
}

void J_View::add_display_box(j_uint i_display_box_id){
	if(M_disp_boxes.count(i_display_box_id)){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}

	J_Display_Box_Shared_t new_display_box(new J_Display_Box(i_display_box_id));
	add_display_object(new_display_box);
	M_disp_boxes[i_display_box_id] = new_display_box;
}

void J_View::add_display_box(J_Display_Box_Shared_t i_display_box){
	if(M_disp_boxes.count(i_display_box->get_object_ID())){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}
	add_display_object(i_display_box);
	M_disp_boxes[i_display_box->get_object_ID()] = i_display_box;
}

void J_View::add_text_display(J_FT_Text_Display_Object_Shared_t i_text_box){
	if(M_text_displays.count(i_text_box->get_object_ID())){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}
	add_display_box(i_text_box);
	M_text_displays[i_text_box->get_object_ID()] = i_text_box;
}

void J_View::add_display_circle(j_uint i_display_circle_id){
	if (M_display_circles.count(i_display_circle_id)){
		throw J_Argument_Error("View Already Has Circle With This ID");
	}

	J_Display_Circle_Shared_t new_display_circle(new J_Display_Circle(i_display_circle_id));
	add_display_object(new_display_circle);
	M_display_circles[i_display_circle_id] = new_display_circle;
}

void J_View::add_display_circle(J_Display_Circle_Shared_t i_display_circle){
	if (M_display_circles.count(i_display_circle->get_object_ID())){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}
	add_display_object(i_display_circle);
	M_display_circles[i_display_circle->get_object_ID()] = i_display_circle;
}

void J_View::add_multi_state_text_box(j_uint i_multi_state_text_id){
	if(M_multi_state_text_boxes.count(i_multi_state_text_id)){
		throw J_Argument_Error("View Already Has Text Box With This ID");
	}

	J_FT_Text_Multi_State_Display_Shared_t new_text_display(
		new J_FT_Text_Multi_State_Display(i_multi_state_text_id)
		);

	add_text_display(new_text_display);
	M_multi_state_text_boxes[i_multi_state_text_id] = new_text_display;
}

void J_View::set_text_string(j_uint i_text_box_id, const J_UI_Multi_String& irk_string){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->set_string(irk_string);
	}
}

void J_View::set_text_cursor(j_uint i_text_box_id, j_uint i_cursor_pos){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->set_cursor_pos(i_cursor_pos);
	}
}

void J_View::set_text_cursor_color(j_uint i_text_box_id, J_Color_RGBA<j_float> i_color){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->set_cursor_color(i_color);
	}
}


void J_View::set_cursor_visibility_status(j_uint i_text_box_id, bool i_status){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->set_cursor_visibility_status(i_status);
	}
}


void J_View::delete_char(j_uint i_text_box_id, j_size_t i_pos){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->delete_char(i_pos);
	}
}

/*insert_chars(j_uint text_box_it, j_size_t pos, const J_UI_String&)*/
void J_View::insert_chars(j_uint i_text_box_id, j_size_t i_pos, const J_UI_String& irk_string){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->insert_chars(i_pos, irk_string);
	}
}

void J_View::insert_char(j_uint i_text_box_id, j_size_t i_pos, J_UI_Char i_char){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->insert_char(i_pos, i_char);
	}
}

void J_View::erase_chars(j_uint i_text_box_id, j_size_t i_pos, j_size_t i_size){
	if(auto text_box = get_text_display(i_text_box_id)){
		text_box->erase_chars(i_pos, i_size);
	}
}

/*void J_View::mouse_button(int i_button, int i_action, int i_modifiers, Pen_Pos_FL_t i_pos)*/
j_uint J_View::mouse_button_press(J_View_Shared_t i_view, int i_button, int i_modifiers, Pen_Pos_FL_t i_pos){
	J_Display_Object_Shared_t disp_obj_ptr = get_display_object_at_pos(i_pos);
	if(!disp_obj_ptr){
		s_controller->notify_object_press(shared_from_this(), 0u);
		return 0;
	}
	s_controller->notify_object_press(shared_from_this(), disp_obj_ptr->get_object_ID());

	switch(i_button){
	case J_LEFT_MOUSE_BUTTON:
		disp_obj_ptr->mouse_button_press(i_view, i_button, i_modifiers, i_pos);
		M_cur_clicked_obj = disp_obj_ptr;
		break;
	default:
		;
	}
	return disp_obj_ptr->get_ID();
}

void J_View::display_object_pressed(J_Display_Object_Shared_t, int , int 
		, Pen_Pos_FL_t){}

J_Display_Object_Shared_t J_View::get_display_object_at_pos(Pen_Pos_FL_t i_pos){
	for(auto disp_obj_ptr : M_disp_objs){
		if(disp_obj_ptr->clickable_status() && disp_obj_ptr->is_inside(i_pos.first, i_pos.second)){
			return disp_obj_ptr;
		}
	}
	return J_Display_Box_Shared_t();
}

void J_View::mouse_button_release( int i_button, int i_modifiers, Pen_Pos_FL_t i_pos){

	switch(i_button){
	case J_LEFT_MOUSE_BUTTON:
		if(M_cur_clicked_obj){
			M_cur_clicked_obj->mouse_button_release(shared_from_this(), i_button, i_modifiers, i_pos);
			M_cur_clicked_obj.reset();
		}
		break;
	default:
		;
	}			
	
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
	for_each(M_disp_objs.begin(), M_disp_objs.end()
		, mem_fn(&J_Display_Object::draw));

	j_swap_buffers(M_context);
}

/*void add_to_buffer()const*/
void J_View::add_to_buffer()const{
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	draw_background();
	for_each(M_disp_objs.begin(), M_disp_objs.end()
		, mem_fn(&J_Display_Object::draw));

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
}

void J_View::set_fill_visibility(j_uint i_disp_obj, bool i_status){
	if(auto disp_obj = get_display_object(i_disp_obj)){
		disp_obj->set_fill_visibility_status(i_status);
	}
}

void J_View::set_outline_visibility(j_uint i_disp_obj, bool i_status){
	if (auto disp_obj = get_display_object(i_disp_obj)){
		disp_obj->set_outline_visibility_status(i_status);
	}
}

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
	if(M_disp_objs.empty()){
		return;
	}

	M_disp_objs.clear(); 
	M_disp_objs_by_id.clear(); 
	M_text_displays.clear();
	M_display_circles.clear();
	M_image_panes.clear();
	M_disp_boxes.clear();
	M_cur_clicked_obj.reset();

}

J_Display_Object_Shared_t J_View::get_display_object(j_uint i_obj_id){
	
	auto found_pos = M_disp_objs_by_id.find(i_obj_id);
	if(found_pos == M_disp_objs_by_id.end()){
		return J_Display_Object_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}

J_Display_Box_Shared_t J_View::get_display_box(j_uint i_obj_id){
	auto found_pos = M_disp_boxes.find(i_obj_id);
	if(found_pos == M_disp_boxes.end()){
		return J_Display_Box_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}

J_Display_Circle_Shared_t J_View::get_display_circle(j_uint i_circle_id){
	auto found_pos = M_display_circles.find(i_circle_id);
	if (found_pos == M_display_circles.end()){
		return J_Display_Circle_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}

J_FT_Text_Display_Object_Shared_t J_View::get_text_display(j_uint i_obj_id){
	auto found_pos = M_text_displays.find(i_obj_id);
	if(found_pos == M_text_displays.end()){
		return J_FT_Text_Display_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}

J_FT_Text_Multi_State_Display_Shared_t J_View::get_multi_state_text_display(j_uint i_obj_id){
	auto found_pos = M_multi_state_text_boxes.find(i_obj_id);
	if(M_multi_state_text_boxes.end() == found_pos){
		return J_FT_Text_Multi_State_Display_Shared_t();
	}

	make_active_context();
	return found_pos->second;
}

void J_View::update_box_coordinates(j_uint i_display_box_id, const J_Rectangle& irk_rectangle){
	if(auto display_box = get_display_box(i_display_box_id)){
		display_box->set_box(irk_rectangle);
	}
}

void J_View::update_fill_color(j_uint i_disp_obj_id, J_Color_RGBA<j_float> i_color){
	if (auto disp_obj = get_display_object(i_disp_obj_id)){
		disp_obj->set_fill_color(i_color);
	}
}

void J_View::update_outline_color(j_uint i_disp_obj_id, J_Color_RGBA<j_float> i_color){
	if (auto disp_obj = get_display_object(i_disp_obj_id)){
		disp_obj->set_outline_color(i_color);
	}
}

void J_View::update_outline_thickness(j_uint i_disp_object_id, j_float i_thickness){
	if (auto disp_pbj = get_display_object(i_disp_object_id)){
		disp_pbj->set_outline_thickness(i_thickness);
	}
}

void J_View::update_circle_shape_data(j_uint i_circle_id, const J_Circle& irk_circle){
	if (auto display_circle = get_display_circle(i_circle_id)){
		display_circle->set_circle(irk_circle);
	}
	
}

void J_View::update_center(j_uint i_obj_id, j_float i_x_pos, j_float i_y_pos){
	if (auto display_obj = get_display_object((i_obj_id))){
		display_obj->set_center(i_x_pos, i_y_pos);
	}
}

void J_View::update_radius(j_uint i_circle_id, j_float i_radius){
	if (auto display_circle = get_display_circle(i_circle_id)){
		display_circle->set_radius(i_radius);
	}
}

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
J_Display_Image_Pane_Shared_t J_View::get_image_pane(j_uint i_obj_id){
	auto found_pos = M_image_panes.find(i_obj_id);
	if(found_pos == M_image_panes.end()){
		return J_Display_Image_Pane_Shared_t();
	}
	make_active_context();
	return found_pos->second;
}

void J_View::update_image_clear(j_uint i_obj_id){
	if(auto image_pane = get_image_pane(i_obj_id)){
		image_pane->clear_image();
	}
}

void J_View::update_image_buffer(j_uint i_obj_id, const j_ubyte* i_buffer){
		if(auto image_pane = get_image_pane(i_obj_id)){
		image_pane->set_buffer(i_buffer);
	}
}

void J_View::update_image_buffer_mono(j_uint i_obj_id, const j_ubyte* i_buffer){
	if(auto image_pane = get_image_pane(i_obj_id)){
		image_pane->set_buffer_mono(i_buffer);
	}
}

void J_View::update_image_height(j_uint i_obj_id, int i_height){
	if(auto image_pane = get_image_pane(i_obj_id)){
		image_pane->set_image_height(i_height);
	}
}

void J_View::update_image_width(j_uint i_obj_id, int i_width){
	if(auto image_pane = get_image_pane(i_obj_id)){
		image_pane->set_image_width(i_width);
	}
}

void J_View::update_image_buffer_format(j_uint i_obj_id, Image_Format i_format){
	if(auto image_pane = get_image_pane(i_obj_id)){
		image_pane->set_image_format(i_format);
	}
}

void J_View::update_middle_line_color(j_uint i_obj_id, J_UI_Color i_color){
	if(auto image_pane = get_image_pane(i_obj_id)){
		image_pane->set_middle_line_color(i_color);
	}
}

void J_View::update_add_multi_text_state(j_uint i_multi_text_state_id){
	if(auto multi_text_state_box = get_multi_state_text_display(i_multi_text_state_id)){
		multi_text_state_box->add_state();
	}
}

void J_View::update_multi_text_state(j_uint i_multi_state_text_id, j_size_t i_state){
	if(auto multi_state_text_box = get_multi_state_text_display(i_multi_state_text_id)){
		multi_state_text_box->set_state(i_state);
	}
}

void J_View::remove_text_box(j_uint i_obj_id){
	remove_display_box(i_obj_id);
	if(!M_text_displays.count(i_obj_id)){
		return;
	}
	M_text_displays.erase(i_obj_id);

}
void J_View::remove_display_box(j_uint i_obj_id){
	remove_display_object(i_obj_id);
	if(!M_disp_boxes.count(i_obj_id)){
		return;
	}

	M_disp_boxes.erase(i_obj_id);
}
void J_View::remove_display_object(j_uint i_obj_id){
	auto found_obj = M_disp_objs_by_id.find(i_obj_id);
	if(M_disp_objs_by_id.end() == found_obj){
		return;
	}
	M_disp_objs_by_id.erase(found_obj);

	M_disp_objs.remove_if([i_obj_id](J_Display_Object_Shared_t i_obj){
		return i_obj->get_object_ID() == i_obj_id;
	});

}




}

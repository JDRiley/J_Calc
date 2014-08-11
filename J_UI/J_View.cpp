#include "J_View.h"
//
#include "J_UI_Controller.h"
//
#include <J_OpenGl_Functions.h>
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
		if(!M_focused_object.expired() && M_focused_object.lock()->is_inside(i_pos)){
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



/*~J_FT_Text_Displayer()*/ //Must be called on main thread and not in callback
J_View::~J_View(){
	
	bool detatch_context_after_clear = s_contexts->get_active_context() == M_context;

	make_active_context();
	clear();
	M_context.reset();
	if(detatch_context_after_clear){
		s_contexts->detach_active_context();
	}
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
	M_multi_state_text_boxes.clear();
	M_display_lines.clear();

}


J_UI_Box_Shared_t J_View::get_display_box(j_uint i_obj_id){
	auto found_pos = M_disp_boxes.find(i_obj_id);
	if(found_pos == M_disp_boxes.end()){
		return J_UI_Box_Shared_t();
	}
	make_active_context();
	return *found_pos;
}

J_UI_Circle_Shared_t J_View::get_display_circle(j_uint i_circle_id){
	auto found_pos = M_display_circles.find(i_circle_id);
	if (found_pos == M_display_circles.end()){
		return J_UI_Circle_Shared_t();
	}
	make_active_context();
	return *found_pos;
}

Multi_State_Text_Box_Shared_t J_View::get_multi_state_text_display(j_uint i_obj_id){
	auto found_pos = M_multi_state_text_boxes.find(i_obj_id);
	if(M_multi_state_text_boxes.end() == found_pos){
		return Multi_State_Text_Box_Shared_t();
	}

	make_active_context();
	return *found_pos;
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
J_Image_Pane_Shared_t J_View::get_image_pane(j_uint i_obj_id){
	auto found_pos = M_image_panes.find(i_obj_id);
	if(found_pos == M_image_panes.end()){
		return J_Image_Pane_Shared_t();
	}
	make_active_context();
	return *found_pos;
}



void J_View::add_ui_line(J_UI_Line_Shared_t i_line_ptr){
	Context_RAII context_saver;
	make_active_context();
	if(M_display_lines.count(i_line_ptr->get_ID())){
		throw J_Argument_Error("View Already Has Line Box With This ID");
	}

	add_ui_object(i_line_ptr);
	M_display_lines[i_line_ptr->get_ID()] = i_line_ptr;

	
}

void J_View::subscribe_cursor_updates(J_UI_Object_Weak_t i_ui_obj){
	M_objects_using_cursor_data.insert(i_ui_obj);
}

void J_View::char_input_cmd(j_uint i_charcode){
	if(M_focused_object.expired()){
		return;
	}

	M_focused_object.lock()->char_input_cmd(i_charcode);
}


void J_View::key_input_cmd(int i_key, int i_scancode, int i_action, int i_modifiers){
	if(M_focused_object.expired()){
		return;
	}

	M_focused_object.lock()->key_input_cmd(i_key, i_scancode, i_action, i_modifiers);
}


void J_View::update(){
	for(auto f_obj : M_ui_objects){
		f_obj->update();
	}
}

void J_View::set_focused_object(J_UI_Object_Shared_t i_obj){
	assert(M_ui_objects.count(i_obj));
	M_focused_object = J_UI_Object_Weak_t(i_obj);
}

}


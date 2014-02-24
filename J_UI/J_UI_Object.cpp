#include "J_UI_Object.h"
#include "J_UI_Model.h"

using std::bind;
using namespace std::placeholders;
namespace jomike{

static j_uint g_object_ids = 0;
static Instance_Pointer<J_UI_Model> s_model;

J_UI_Object::J_UI_Object() :J_UI_Outline_Fill_Management(++g_object_ids)
, M_ID(g_object_ids){
#ifndef VS_2013
	default_initialization();
#endif

}

J_UI_Object::J_UI_Object(j_uint i_id):J_UI_Outline_Fill_Management(i_id),M_ID(i_id){}


void J_UI_Object::set_clickable_status(bool i_clickable){
	M_clickable_flag = i_clickable;
	s_model->notify_clickable_status(get_ID(), i_clickable);
}

bool J_UI_Object::clickable_status()const{return M_clickable_flag;}

void J_UI_Object::broadcast_current_state()const{
	s_model->notify_clickable_status(get_ID(), M_clickable_flag);
	broadcast_outline_fill_state();
}

/*key_input_cmd(j_window_t, int , int, int, int)*/
void J_UI_Object::key_input_cmd(j_window_t i_window, int i_charcode, int i_scancode
								, int i_action, int i_modifiers){

	if(M_parent){
		M_parent->key_input_cmd(i_window, i_charcode, i_scancode, i_action, i_modifiers);
	}
}

void J_UI_Object::set_left_click_on(){if(M_clickable_flag){M_left_click_on_flag = true;}}

void J_UI_Object::set_left_click_off(){if(M_clickable_flag){M_left_click_on_flag = false;}}

void J_UI_Object::set_mouse_button_press_callback(Mouse_Press_Func_t i_func){
	M_mouse_press_callback = i_func;
}

void J_UI_Object::set_mouse_button_release_callback(Mouse_Press_Func_t i_func){
	M_mouse_release_callback = i_func;
}

void J_UI_Object::mouse_button_press(int i_key, int i_modifiers, Pen_Pos_FL_t i_pen_pos){
	if(!M_mouse_press_callback){
		return;
	}
	M_mouse_press_callback(shared_from_this(), i_key, i_modifiers, i_pen_pos);
}

void J_UI_Object::mouse_button_press_n(int i_key, int i_modifiers
									   , Pen_Pos_FL_t i_pen_pos, int i_count){
	if(!M_mouse_press_callback){
		return;
	}
	for(int i = 0; i < i_count; i++){
		M_mouse_press_callback(shared_from_this(), i_key, i_modifiers, i_pen_pos);
	}
}

void J_UI_Object::mouse_button_release(int i_key, int i_modifiers, Pen_Pos_FL_t i_pen_pos){
	if(!M_mouse_release_callback){
		return;
	}
	M_mouse_release_callback(shared_from_this(), i_key, i_modifiers, i_pen_pos);
}

void J_UI_Object::toggle(){
	M_toggled_status = !M_toggled_status;
}

bool J_UI_Object::toggle_status()const{
	return M_toggled_status;
}

void J_UI_Object::toggle_n(int i_num_toggles){
	assert(i_num_toggles > 0);
	for(int i = 0; i < i_num_toggles; i++){
		toggle();
	}
}

#ifndef VS_2013
void J_UI_Object::default_initialization(){
	M_toggled_status = M_left_click_on_flag
		= M_clickable_flag = M_focus_status = false;
	M_mouse_release_callback = M_mouse_press_callback = nullptr;
}
#endif

void J_UI_Object::add_update_callback(J_UI_Object_Update_Callback_Shared_t i_callback){
	M_update_callbacks.push_back(i_callback);
}

void J_UI_Object::update(){
	for(auto f_updater : M_update_callbacks){
		(*f_updater)(shared_from_this());
	}
}

void J_UI_Object::set_focus_status(bool i_status){
	M_focus_status = i_status;
	for(auto f_callback : M_focus_callbacks){
		f_callback(shared_from_this(), i_status);
	}
}

bool J_UI_Object::focus_status()const{
	return M_focus_status;
}

void J_UI_Object::add_focus_callback(Focus_Callback_Func_t i_callback){
	M_focus_callbacks.push_back(i_callback);
}

void J_UI_Object::alert_cursor_pos(Pen_Pos_FL_t){

}

J_UI_Outline_Fill_Management::J_UI_Outline_Fill_Management(j_uint i_id):M_ID(i_id){}

j_uint J_UI_Outline_Fill_Management::get_ID()const{ return M_ID; }


void J_UI_Outline_Fill_Management::set_fill_visibility_status(bool i_status){
	J_Outline_Fill_Management::set_fill_visibility_status(i_status);
	s_model->notify_fill_visibility_status(get_ID(), i_status);
}

void J_UI_Outline_Fill_Management::set_outline_visibility_status(bool i_status){
	J_Outline_Fill_Management::set_outline_visibility_status(i_status);
	s_model->notify_outline_visibility_status(get_ID(), i_status);
}

void J_UI_Outline_Fill_Management::set_fill_color(const J_UI_Color& i_color){
	M_fill_color = i_color;
	s_model->notify_fill_color(get_ID(), i_color);
}

void J_UI_Outline_Fill_Management::set_outline_color(const J_Color_RGBA<j_float>& i_color){
	M_outline_color = i_color;
	s_model->notify_outline_color(get_ID(), i_color);
}

void J_UI_Outline_Fill_Management::set_outline_thickness(j_float i_thickness){
	J_Outline_Fill_Management::set_outline_thickness(i_thickness);
	s_model->notify_outline_thickness(get_ID(), i_thickness);
}


J_UI_Color J_UI_Outline_Fill_Management::outline_color()const{
	return M_outline_color;
}

J_UI_Color J_UI_Outline_Fill_Management::fill_color()const{
	return M_fill_color;
}


void J_UI_Outline_Fill_Management::broadcast_outline_fill_state()const{
	s_model->notify_fill_visibility_status(get_ID(), fill_visibility_status());
	s_model->notify_outline_visibility_status(get_ID(), outline_visibility_status());
	s_model->notify_fill_color(get_ID(), fill_color());
	s_model->notify_outline_color(get_ID(), outline_color());
	s_model->notify_outline_thickness(get_ID(), outline_thickness());
}

J_UI_Box::J_UI_Box(const J_Rectangle& irk_pos):J_Rectangle(irk_pos){}

J_UI_Box::J_UI_Box(const J_Rectangle& irk_pos, j_uint i_obj_id) : J_UI_Object(i_obj_id)
	, J_Rectangle(irk_pos){}

void J_UI_Box::set_width(j_float i_width){
	J_Rectangle::set_width(i_width);
	s_model->notify_box_coordinates(get_ID(), *this);
}

void J_UI_Box::set_height(j_float i_height){
	J_Rectangle::set_height(i_height);
	s_model->notify_box_coordinates(get_ID(), *this);
}

void J_UI_Box::set_x(j_float i_x){
	J_Rectangle::set_x(i_x);
	s_model->notify_box_coordinates(get_ID(), *this);
}

void J_UI_Box::set_y(j_float i_y){
	J_Rectangle::set_y(i_y);
	s_model->notify_box_coordinates(get_ID(), *this);
}





void J_UI_Box::broadcast_current_state()const{
	J_UI_Object::broadcast_current_state();
	s_model->notify_box_coordinates(get_ID(), *this);
}

void J_UI_Box::set_rectangle(const J_Rectangle& irk_rectangle){
	J_Rectangle::set_rectangle(irk_rectangle);
	s_model->notify_box_coordinates(get_ID(), *this);
	
}

J_UI_Circle::J_UI_Circle(const J_Circle& irk_circle) :J_Circle(irk_circle){}

void J_UI_Circle::set_x(j_float i_x_pos){
	J_Circle::set_x(i_x_pos);
	s_model->notify_center(get_ID(), x1(), y1());
}

void J_UI_Circle::set_y(j_float i_y_pos){
	J_Circle::set_y(i_y_pos);
	s_model->notify_center(get_ID(), x1(), y1());
	
}

void J_UI_Circle::set_center(j_float i_x_pos, j_float i_y_pos){
	J_Circle::set_center(i_x_pos, i_y_pos);
	s_model->notify_center(get_ID(), i_x_pos, i_y_pos);
}

void J_UI_Circle::set_radius(j_float i_radius){
	s_model->notify_radius(get_ID(), radius());
	J_Circle::set_radius(i_radius);
}

void J_UI_Circle::broadcast_current_state()const{
	J_UI_Object::broadcast_current_state();
	s_model->notify_circle_shape_data(get_ID(), *this);
}

//J_UI_Circle---------------------------------------------------------------



}
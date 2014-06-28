#include "J_UI_Object.h"


using std::bind;
using namespace std::placeholders;
namespace jomike{

static j_uint g_object_ids = 0;
static Instance_Pointer<J_UI_Model> s_model;

J_UI_Object::J_UI_Object(): M_ID(++g_object_ids){
#ifndef VS_2013
	default_initialization();
#endif

}

J_UI_Object::J_UI_Object(j_uint i_id):M_ID(i_id){}


void J_UI_Object::set_clickable_status(bool i_clickable){
	M_clickable_flag = i_clickable;
	//s_model->notify_clickable_status(get_ID(), i_clickable);
}

bool J_UI_Object::clickable_status()const{return M_clickable_flag;}


/*key_input_cmd(j_window_t, int , int, int, int)*/
void J_UI_Object::key_input_cmd(int i_charcode, int i_scancode
								, int i_action, int i_modifiers){

	if(M_parent){
		M_parent->key_input_cmd(i_charcode, i_scancode, i_action, i_modifiers);
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

bool J_UI_Object::left_click_on_status()const{
	return M_left_click_on_flag;
}

void J_UI_Object::broadcast_current_state()const{

}

void J_UI_Object::alert_resize(int, int){

}




}
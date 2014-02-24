#include "Multi_State_Text_Box.h"
//
#include "../J_UI_Model.h"
using std::dynamic_pointer_cast;

namespace jomike{
static Instance_Pointer<J_UI_Model> s_model;

void Multi_State_Text_Box::set_state(j_size_t i_index){
	assert(between_inclusive(i_index, J_SIZE_T_ZERO, M_states.size()));
	M_current_state = i_index;
	M_current_text_box = M_states[i_index];
	J_Rectangle::set_rectangle(*M_current_text_box);
	s_model->notify_multi_text_state(get_ID(), i_index);
}



void Multi_State_Text_Box::set_string(const J_UI_String& irk_string){
	M_current_text_box->set_string(irk_string);
}

Multi_State_Text_Box::Multi_State_Text_Box():J_Text_Box_Object(J_Rectangle()){
	M_current_state = 0;
}

void Multi_State_Text_Box::clear_string(){
	M_current_text_box->clear_string();
}

void Multi_State_Text_Box::move_cursor(j_size_t i_move_val){
	M_current_text_box->move_cursor(i_move_val);
}

bool Multi_State_Text_Box::insert_char(J_UI_Char i_char){
	return M_current_text_box->insert_char(i_char);
}

void Multi_State_Text_Box::insert_string(const J_UI_String& irk_string){
	M_current_text_box->insert_string(irk_string);
}

void Multi_State_Text_Box::insert_string(j_size_t i_pos, const J_UI_String& irk_string){
	M_current_text_box->insert_string(i_pos, irk_string);
}

void Multi_State_Text_Box::backspace(){
	M_current_text_box->backspace();
}

void Multi_State_Text_Box::delete_char(){
	M_current_text_box->delete_char();
}

void Multi_State_Text_Box::erase_chars(j_size_t i_pos, j_size_t i_size){
	M_current_text_box->erase_chars(i_pos, i_size);
}

void Multi_State_Text_Box::set_cursor_pos(j_size_t i_cursor_pos){
	M_current_text_box->set_cursor_pos(i_cursor_pos);
}



void Multi_State_Text_Box::set_cursor_color(const J_Color_RGBA<j_float> irk_color){
	M_current_text_box->set_cursor_color(irk_color);
}



void Multi_State_Text_Box::set_left_click_on(){
	M_current_text_box->set_left_click_on();
}

void Multi_State_Text_Box::set_left_click_off(){
	M_current_text_box->set_left_click_off();
}

void Multi_State_Text_Box::update(){
	J_Text_Box_Object::update();
	M_current_text_box->update();
}

void Multi_State_Text_Box::key_input_cmd(j_window_t i_window
											, int i_charcode, int i_scancode, int i_action, int i_modifiers){
	M_current_text_box->key_input_cmd(i_window, i_charcode, i_scancode, i_action
										, i_modifiers);
}

void Multi_State_Text_Box::char_input_cmd(j_window_t i_window, int i_charcode){
	M_current_text_box->char_input_cmd(i_window, i_charcode);
}

void Multi_State_Text_Box::set_left_bound(){
	M_current_text_box->set_left_bound();
}

void Multi_State_Text_Box::set_right_bound(){
	M_current_text_box->set_right_bound();
}

void Multi_State_Text_Box::set_top_bound(){
	M_current_text_box->set_top_bound();
}

void Multi_State_Text_Box::set_bottom_bound(){
	M_current_text_box->set_bottom_bound();
}

void Multi_State_Text_Box::add_flags(unsigned i_flags){
	M_current_text_box->add_flags(i_flags);
}

void Multi_State_Text_Box::set_flags(unsigned i_flags){
	M_current_text_box->set_flags(i_flags);
}

unsigned Multi_State_Text_Box::get_flags()const {
	return M_current_text_box->get_flags();
}

j_size_t Multi_State_Text_Box::get_cursor_pos()const {
	return M_current_text_box->get_cursor_pos();
}

void Multi_State_Text_Box::set_read_only_status(bool i_status){
	M_current_text_box->set_read_only_status(i_status);
}

bool Multi_State_Text_Box::read_only_status()const {
	return M_current_text_box->read_only_status();
}

void Multi_State_Text_Box::set_colors(J_Color_RGBA<j_float> i_cursor_color
										, J_Color_RGBA<j_float> i_box_color, J_Color_RGBA<j_float> i_outline_color){
	M_current_text_box->set_colors(i_cursor_color, i_box_color, i_outline_color);
}

void Multi_State_Text_Box::change_color_at_pos(int i_pos, J_UI_Color i_color){
	M_current_text_box->change_color_at_pos(i_pos, i_color);
}

const J_UI_Multi_String& Multi_State_Text_Box::get_string()const {
	return M_current_text_box->get_string();
}

void Multi_State_Text_Box::broadcast_current_state()const{
	if(!M_current_text_box){
		return;
	}

	s_model->notify_multi_text_state(get_ID(), M_current_state);
	M_current_text_box->broadcast_current_state();
}

void Multi_State_Text_Box::add_state(){
	s_model->notify_add_multi_text_state(get_ID());
	M_states.push_back(J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle()
		, J_UI_Multi_String(), get_ID())));

}

void Multi_State_Text_Box::add_states(j_size_t i_size){
	assert(i_size >= 0);
	for(int i = 0; i < i_size; i++){
		add_state();
	}

}

j_size_t Multi_State_Text_Box::state()const{
	return M_current_state;
}

void Multi_State_Text_Box::set_rectangle(const J_Rectangle& irk_rec){
	M_current_text_box->set_rectangle(irk_rec);
	J_Rectangle::set_rectangle(*M_current_text_box);
}

void Multi_State_Text_Box::set_outline_visibility_status(bool i_status){
	M_current_text_box->set_outline_visibility_status(i_status);
}

void Multi_State_Text_Box::set_fill_visibility_status(bool i_status){
	M_current_text_box->set_fill_visibility_status(i_status);
}

void Multi_State_Text_Box::set_fill_color(const J_Color_RGBA<j_float>& i_color){
	M_current_text_box->set_fill_color(i_color);
}

void Multi_State_Text_Box::set_outline_color(const J_Color_RGBA<j_float>& i_color){
	M_current_text_box->set_outline_color(i_color);
}

void Multi_State_Text_Box::set_char_input_command(Char_Input_Cmd_Func_t i_command){
	M_current_text_box->set_char_input_command(i_command);
}

void Multi_State_Text_Box::set_key_input_command(Key_Input_Cmd_Func_t i_command){
	M_current_text_box->set_key_input_command(i_command);
}



Multi_State_Text_Box_Shared_t Multi_State_Text_Box::shared_from_this(){
	auto this_text_box_ptr
		= dynamic_pointer_cast<Multi_State_Text_Box>(J_UI_Object::shared_from_this());
	assert(this_text_box_ptr);
	return this_text_box_ptr;
}

void Multi_State_Text_Box::set_cursor_visibility_status(bool i_status){
	M_current_text_box->set_cursor_visibility_status(i_status);
}

bool Multi_State_Text_Box::cursor_visibility_status()const {
	return M_current_text_box->cursor_visibility_status();
}

void Multi_State_Text_Box::move_cursor_line_pos_up(j_size_t i_move_val){
	M_current_text_box->move_cursor_line_pos_up(i_move_val);
}

void Multi_State_Text_Box::move_cursor_line_pos_down(j_size_t i_move_val){
	M_current_text_box->move_cursor_line_pos_down(i_move_val);
}

void Multi_State_Text_Box::silent_set_cursor_pos(j_size_t i_cursor_pos){
	M_current_text_box->silent_set_cursor_pos(i_cursor_pos);
}

void Multi_State_Text_Box::move_cursor_to_line_begin(){
	M_current_text_box->move_cursor_to_line_begin();
}

void Multi_State_Text_Box::move_cursor_to_line_end(){
	M_current_text_box->move_cursor_to_line_end();
}

j_uint Multi_State_Text_Box::get_cursor_line_id()const{
	return M_current_text_box->get_cursor_line_id();
}

bool Multi_State_Text_Box::auto_scrolling_status()const {
	return M_current_text_box->auto_scrolling_status();
}

void Multi_State_Text_Box::set_auto_scrolling_status(bool i_status){
	M_current_text_box->set_auto_scrolling_status(i_status);
}

void Multi_State_Text_Box::alert_cursor_pos(Pen_Pos_FL_t i_pos){
	M_current_text_box->alert_cursor_pos(i_pos);
}

void Multi_State_Text_Box::mouse_button_release(int i_button, int i_modifiers, Pen_Pos_FL_t i_pos){
	M_current_text_box->mouse_button_release(i_button, i_modifiers, i_pos);
}

void Multi_State_Text_Box::mouse_button_press(int i_button, int i_modifiers, Pen_Pos_FL_t i_pos){
	M_current_text_box->mouse_button_press(i_button, i_modifiers, i_pos);
}





}




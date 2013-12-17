#include "J_Text_Box.h"
#include "../J_UI_Model.h"
#include "../../J_Utile/J_Utile.h"
//Algorithms
#include <algorithm>
#include <functional>
using std::any_of; using std::bind; using std::equal_to; using std::mem_fn; using std::not1;
using std::remove_if; using std::transform; using std::static_pointer_cast;
using std::dynamic_pointer_cast;

using namespace std::placeholders;

//Containers


#include <iostream>
using std::cerr;

//Utilities
#include <cassert>

namespace jomike{

static Instance_Pointer<J_UI_Model> s_model;



j_dbl DEFAULT_CURSOR_REFESH_TIME = 1.0/1.5;
J_Text_Box::J_Text_Box(const J_Rectangle& irk_rec_pos, const J_UI_Multi_String& irk_string)
	: J_Text_Box_Object(irk_rec_pos), M_multi_string(irk_string), M_cursor_pos(static_cast<int>(M_multi_string.size()))
{
	
	initialize();

}

J_Text_Box::J_Text_Box(const J_Rectangle& irk_rec_pos, const J_UI_Multi_String& irk_string
	, j_uint i_id)
: J_Text_Box_Object(irk_rec_pos, i_id), M_multi_string(irk_string), M_cursor_pos(static_cast<int>(M_multi_string.size()))
{
	initialize();
}



bool J_Text_Box::insert_char(J_UI_Char i_char){
	M_multi_string.insert(M_cursor_pos, i_char);

	s_model->notify_char_add(get_ID(), M_cursor_pos, i_char);
	
	move_cursor(1);
	set_cursor_on();

	return true;
}


/*void insert_string(const J_UI_String&)*/
void J_Text_Box::insert_string(const J_UI_String& irk_string){
	insert_string(get_cursor_pos(), irk_string);
}

/*void insert_string(int pos, const J_UI_String&)*/
void J_Text_Box::insert_string(j_size_t i_pos, const J_UI_String& irk_string){
	
	
	auto insert_pos  = M_multi_string.get_insert_pos(i_pos);

	insert_pos.second->insert(insert_pos.first, irk_string.begin(), irk_string.end());


	s_model->notify_text_string(get_ID(), M_multi_string);
	set_cursor_pos(i_pos + irk_string.size());

}
void J_Text_Box::set_string(const J_UI_String& irk_string){

	M_multi_string = J_UI_Multi_String(irk_string);

	s_model->notify_text_string(get_ID(), M_multi_string);
	set_cursor_on();
	M_cursor_pos = static_cast<int>(irk_string.size());
}

void J_Text_Box::move_cursor(j_size_t i_amt){
	j_size_t new_cursor =  M_cursor_pos + i_amt;
	if(new_cursor < 0){
		M_cursor_pos = 0;
	}else if(new_cursor > M_multi_string.size()){
		M_cursor_pos = static_cast<int>(M_multi_string.size());
	}else{
		M_cursor_pos = new_cursor;
	}
	s_model->notify_text_cursor(get_ID(), M_cursor_pos);
	set_cursor_on();
	
}

void J_Text_Box::set_cursor_pos(j_size_t i_cursor_pos){
	assert(between_inclusive(i_cursor_pos, J_SIZE_T_ZERO, M_multi_string.size()));
		
	M_cursor_pos = i_cursor_pos;
	s_model->notify_text_cursor(get_ID(), M_cursor_pos);
	set_cursor_on();
}

void J_Text_Box::set_cursor_color(const J_Color_RGBA<j_float> i_color){
	M_cursor_color = i_color;
	set_cursor_on();
	s_model->notify_text_cursor_color(get_ID(), i_color);
}

void J_Text_Box::backspace(){
	if(0 == M_cursor_pos){
		return;
	}

	M_multi_string.erase(--M_cursor_pos, 1);

	s_model->notify_char_delete(get_ID(), M_cursor_pos);
	set_cursor_on();
}

void J_Text_Box::delete_char(){
	if(M_multi_string.size() == M_cursor_pos){
		return;
	}

	M_multi_string.erase(M_cursor_pos,1);

	s_model->notify_char_delete(get_ID(), M_cursor_pos);
	set_cursor_on();
}

/*void erase_chars(int pos, int size)*/
void J_Text_Box::erase_chars(j_size_t i_pos, j_size_t i_size){
	set_cursor_pos(i_pos);

	M_multi_string.erase(i_pos, i_size);
	s_model->notify_erase_chars(get_ID(), M_cursor_pos, i_size);
}

void J_Text_Box::set_left_click_on(){
	M_left_click_is_on = true;
	M_saved_outline_visibility_status = outline_visibility_status();
	set_outline_visibility_status(true);
}

void J_Text_Box::set_left_click_off(){
	M_left_click_is_on = false;
	set_outline_visibility_status(M_saved_outline_visibility_status);
}

void J_Text_Box::broadcast_current_state()const{
	J_UI_Box::broadcast_current_state();
	s_model->notify_text_string(get_ID(), M_multi_string);
	s_model->notify_clickable_status(get_ID(), clickable_status());
	s_model->notify_text_cursor(get_ID(), M_cursor_pos);
	s_model->notify_text_cursor_color(get_ID(), M_cursor_color);

}

void standard_text_box_input_parser(J_Text_Box_Object_Shared_t i_text_box
	, int i_key, int , int i_action, int ){
	
	if((J_PRESS != i_action) && (J_REPEAT != i_action)){
		return;
	}

	if(i_text_box->read_only_status()){
		return;
	}

	switch(i_key){
	case J_KEY_BACKSPACE:
		i_text_box->backspace();
		break;
	case J_KEY_TAB:
		i_text_box->insert_char('\t');
		break;
	case J_KEY_ENTER:
		i_text_box->insert_char('\n');
		break;
	case J_KEY_LEFT:
		i_text_box->move_cursor(-1);
		break;
	case J_KEY_RIGHT:
		i_text_box->move_cursor(1);
		break;
	case J_KEY_DELETE:
		i_text_box->delete_char();
		break;
	default:
		;
	}
}


void J_Text_Box::key_input_cmd(j_window_t , int i_key, int i_scancode, int i_action, int i_modifiers){
	if(!M_key_input_command){
		return;
	}

	M_key_input_command(shared_from_this(), i_key, i_scancode, i_action, i_modifiers);
}

void J_Text_Box::char_input_cmd(j_window_t, int i_charcode){
	if(read_only_status()){
		return;
	}

	M_char_input_command(shared_from_this(), i_charcode);
}

/*void set_read_only_status(bool)*/
void J_Text_Box::set_read_only_status(bool i_status){
	M_text_state = i_status ? (M_text_state & ~WRITE) : (M_text_state | WRITE);
}

void J_Text_Box::set_colors(J_Color_RGBA<j_float> i_cursor_color
			, J_Color_RGBA<j_float> i_fill_color, J_Color_RGBA<j_float> i_outline_color){

	set_cursor_color(i_cursor_color);
	set_fill_color(i_fill_color);
	set_outline_color(i_outline_color);
}

/*bool read_only_status()*/
bool J_Text_Box::read_only_status()const{
	return !(M_text_state & WRITE);
}

const J_UI_Multi_String& J_Text_Box::get_string()const{
	return M_multi_string;
}

void Multi_State_Text_Box::set_string(const J_UI_String& irk_string){
	M_current_text_box->set_string(irk_string);
}

void J_Text_Box::change_color_at_pos(int i_pos, J_UI_Color i_color){
	M_multi_string.get_string_holding_index(i_pos)->set_color(i_color);
	s_model->notify_text_string(get_ID(), M_multi_string);
}

void J_Text_Box::add_flags(unsigned i_flags){
	M_text_state |= i_flags;
}

void J_Text_Box::set_flags(unsigned i_flags){
	M_text_state = i_flags;
}

unsigned J_Text_Box::get_flags()const {
	return M_text_state;
}

j_size_t J_Text_Box::get_cursor_pos()const {
	return M_cursor_pos;
}

void J_Text_Box::clear_string(){

	assert(!"The method or operation is not yet check.");
}

void J_Text_Box::set_left_bound(){

	assert(!"The method or operation is not yet check.");
}

void J_Text_Box::set_right_bound(){

	assert(!"The method or operation is not yet check.");
}

void J_Text_Box::set_top_bound(){

	assert(!"The method or operation is not yet check.");
}

void J_Text_Box::set_bottom_bound(){

	assert(!"The method or operation is not yet check.");
}

void J_Text_Box::set_key_input_command(Key_Input_Cmd_Func_t i_command){
	M_key_input_command = i_command;
}

void J_Text_Box::set_char_input_command(Char_Input_Cmd_Func_t i_command){
	M_char_input_command = i_command;
}


J_Text_Box_Object::J_Text_Box_Object(const J_Rectangle& i_rec):J_UI_Box(i_rec){}

J_Text_Box_Object::~J_Text_Box_Object(){}

void Multi_State_Text_Box::set_state(j_size_t i_index){
	assert(between_inclusive(i_index, J_SIZE_T_ZERO, M_states.size()));
	M_current_state = i_index;
	M_current_text_box = M_states[i_index];
	J_Rectangle::set_rectangle(*M_current_text_box);
	s_model->notify_multi_text_state(get_ID(), i_index);
}



J_Text_Box_Object::J_Text_Box_Object(const J_Rectangle& i_rec, j_uint i_obj_id) 
	: J_UI_Box(i_rec, i_obj_id){}


Multi_State_Text_Box::Multi_State_Text_Box():J_Text_Box_Object(J_Rectangle()){
	M_current_state = 0;
}

void Multi_State_Text_Box::clear_string(){
	M_current_text_box->clear_string();
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

void Multi_State_Text_Box::move_cursor(j_size_t i_move_val){
	M_current_text_box->move_cursor(i_move_val);
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


J_Text_Box_Shared_t J_Text_Box::shared_from_this(){
	auto this_text_box_ptr
		= dynamic_pointer_cast<J_Text_Box>(J_UI_Object::shared_from_this());
	assert(this_text_box_ptr);
	return this_text_box_ptr;
}

void J_Text_Box::initialize(){
	M_cursor_color = J_Color::Clear;
	M_left_click_is_on = false;
	M_text_state = Text_Flags::SCROLL;
	M_cursor_visibility_status = false;
	enable_default_key_char_processing();
}

void J_Text_Box::set_cursor_visibility_status(bool i_status){
	if(M_cursor_visibility_status == i_status){
		return;
	}
	M_cursor_visibility_status = i_status;
	s_model->notify_cursor_visibility_status(get_ID(), cursor_visibility_status());
}

bool J_Text_Box::cursor_visibility_status()const{
	return M_cursor_visibility_status;
}


J_Text_Box_Object_Shared_t J_Text_Box_Object::shared_from_this(){
	auto this_text_box_ptr
		= dynamic_pointer_cast<J_Text_Box_Object>(J_UI_Object::shared_from_this());
	assert(this_text_box_ptr);
	return this_text_box_ptr;
}

void J_Text_Box_Object::enable_default_key_char_processing(){
	set_key_input_command(standard_text_box_input_parser);
	set_char_input_command(standard_char_input_cmd);
}


void standard_char_input_cmd(J_Text_Box_Object_Shared_t i_text_box, int i_charcode){
	i_text_box->insert_char(i_charcode);
}

void numeric_entry_char_input_cmd(J_Text_Box_Object_Shared_t i_text_box, int i_charcode){
	if(('.' != i_charcode) && !J_UI_Char(i_charcode).is_number()){
		return;
	}
	standard_char_input_cmd(i_text_box, i_charcode);
}

class J_Text_Cursor_Blinker_Updater : public J_UI_Object_Update_Callback{
public:
	J_Text_Cursor_Blinker_Updater();
	void operator()(J_UI_Object_Shared_t)override;
	void set_blink_rate(j_dbl i_rate);
	j_dbl blink_rate()const;
	void reset_timer();
private:
	const int M_ID;
	static j_dbl get_blink_rate(int i_id);
	typedef decltype(std::bind(&J_UI_Model::time, (J_UI_Model*)0)) Time_Retrieval_t;
	typedef
		decltype(std::bind(&J_Text_Cursor_Blinker_Updater::get_blink_rate
		, J_INT_ZERO)) Rate_Retrieval_t;

	J_Duration_Tester<Time_Retrieval_t, Rate_Retrieval_t> M_timer;

	static ex_array<j_dbl> S_blink_rates;
};

ex_array<j_dbl> J_Text_Cursor_Blinker_Updater::S_blink_rates;

void J_Text_Box::set_cursor_on(){
	M_cursor_visibility_status = true;
	if(M_blinker_updater){
		M_blinker_updater->reset_timer();
	}
	s_model->notify_cursor_visibility_status(get_ID(), cursor_visibility_status());
}

void J_Text_Box::enable_blinking_cursor(){
	if(M_blinker_updater){
		return;
	}
	M_blinker_updater 
		= J_Text_Cursor_Blinker_Updater_Shared_t(new J_Text_Cursor_Blinker_Updater);

	add_update_callback(M_blinker_updater);
}



void J_Text_Cursor_Blinker_Updater::operator()(J_UI_Object_Shared_t i_obj){
	auto text_box
		= dynamic_pointer_cast<J_Text_Box_Object>(i_obj);

	assert(text_box);
	if(M_timer.time_exceeded()){
		text_box
			->set_cursor_visibility_status(!text_box->cursor_visibility_status());
		M_timer.reset_timer();
	}
}

static int s_cursor_blinker_ids = 0;

j_dbl J_Text_Cursor_Blinker_Updater::blink_rate()const{
	return S_blink_rates[M_ID];
}

void J_Text_Cursor_Blinker_Updater::set_blink_rate(j_dbl i_rate){
	S_blink_rates[M_ID] = i_rate;
}

J_Text_Cursor_Blinker_Updater::J_Text_Cursor_Blinker_Updater()
	:M_ID(++s_cursor_blinker_ids), M_timer(bind(&J_UI_Model::time, &*s_model)
	, bind(&J_Text_Cursor_Blinker_Updater::get_blink_rate, M_ID)){
	if(M_ID >= S_blink_rates.size()){
		S_blink_rates.resize(M_ID + 1);
	}
	S_blink_rates[M_ID] = DEFAULT_CURSOR_REFESH_TIME;
}

j_dbl J_Text_Cursor_Blinker_Updater::get_blink_rate(int i_id){
	return S_blink_rates[i_id];
}

void J_Text_Cursor_Blinker_Updater::reset_timer(){
	M_timer.reset_timer();
}


}


#include "J_Text_Box.h"
//
#include <J_Utile.h>
//Algorithms
#include <algorithm>
//
#include <functional>
//
#include <J_UI/J_Font_Manager.h>
//
#include <J_UI/J_Font_Face.h>
//
#include "../j_cursor_management.h"
//
#include <J_OpenGL.h>
//
#include <J_Shader_Program.h>
//
#include "../J_UI_Line.h"
//
#include "../J_UI.h"
//
#include "J_UI_Letter_Box.h"
//
#include <J_Open_GL.h>

using std::any_of; using std::bind; using std::equal_to; using std::mem_fn; using std::not1;
using std::remove_if; using std::transform; using std::static_pointer_cast;
using std::dynamic_pointer_cast; using std::max; using std::min; using std::mem_fn;
using std::function;
using namespace std::placeholders;

//
#include <algorithm>
//
#include <functional>

using std::mem_fn; using std::bind; using std::for_each; using std::transform;
using namespace std::placeholders; using std::lower_bound;

//Containers
#include <iostream>
using std::cerr;

//Utilities
#include <cassert>

namespace jomike{


static Instance_Pointer<Contexts_Handler> s_contexts;
static int image_shader_id(){
	static Image_Shader_Program s_image_shader(Image_Format::RGBA32_UBYTE);
	return s_image_shader.program_id();
}
const j_float FLOAT_DELTA = 0.0001f;
const j_float K_LEFT_PADDING = 0.01f;

j_dbl DEFAULT_CURSOR_REFESH_TIME = 1.0/1.8;
static J_Open_GL s_open_gl;
class Text_Line_Order_Comp{
public:
	Text_Line_Order_Comp(j_float i_start_line, j_float i_line_size):M_line_size(i_line_size)
		, M_starting_pos(i_start_line){}

	bool operator()(const Pen_Pos_FL_t, const Pen_Pos_FL_t)const;
private:
	int get_line_index(j_float i_float)const;
	j_float M_line_size;
	j_float M_starting_pos;

};

int Text_Line_Order_Comp::get_line_index(j_float i_float)const{
	int line_number = 0;

	while(i_float < (M_starting_pos - line_number*M_line_size) - FLOAT_DELTA){
		++line_number;
	}

	if(line_number){
		return line_number;
	}

	if(i_float < M_starting_pos){
		return line_number;
	}

	while(i_float >(M_starting_pos - line_number*M_line_size) - FLOAT_DELTA){
		--line_number;
	}

	return line_number;
}

bool Text_Line_Order_Comp::operator()(const Pen_Pos_FL_t ik_left, const Pen_Pos_FL_t ik_right)const{
	int compare_val = get_line_index(ik_right.second) - get_line_index(ik_left.second);
	if(compare_val < 0){
		return false;
	} else if(compare_val > 0){
		return true;
	} else if((ik_right.first - ik_left.first) > 0){
		return true;
	} else{
		return false;
	}
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
	case J_KEY_DELETE:
		i_text_box->delete_char();
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
	case J_KEY_UP:
		i_text_box->move_cursor_line_pos_up(1);
		break;
	case J_KEY_DOWN:
		i_text_box->move_cursor_line_pos_down(1);
		break;
	case J_KEY_HOME:
		i_text_box->move_cursor_to_line_begin();
		break;
	case J_KEY_END:
		i_text_box->move_cursor_to_line_end();
		break;
	default:
		;
	}
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
	typedef decltype(std::bind(&J_UI::time, &J_UI::get_instance())) Time_Retrieval_t;
	typedef
		decltype(std::bind(&J_Text_Cursor_Blinker_Updater::get_blink_rate
		, J_INT_ZERO)) Rate_Retrieval_t;

	J_Duration_Tester<Time_Retrieval_t, Rate_Retrieval_t> M_timer;

	static ex_array<j_dbl> Ms_blink_rates;
};

J_Text_Box::J_Text_Box(const J_Rectangle& irk_rec_pos, const J_UI_Multi_String& irk_string)
	: J_Text_Box_Object(irk_rec_pos), M_multi_string(irk_string)
	, M_letter_box_string(bind(&J_Text_Box::alert_changed, this))
	, M_cursor_pos(static_cast<int>(M_multi_string.size())){
	
	
	
	for(auto& f_string : M_multi_string){
		auto letter_boxes = make_letter_boxes(f_string);

		M_letter_box_string->insert(
			M_letter_box_string->end(), letter_boxes.begin(), letter_boxes.end());
									
	}

	initialize();

}


void J_Text_Box::initialize(){
	initialize_frame_buffer();
	M_text_state = Text_Flags::SCROLL;
	M_cursor_line = J_UI_Line_Shared_t(
		new J_UI_Line(default_pen_pos().first, default_pen_pos().second
		, default_pen_pos().first, default_pen_pos().second + new_line_screen_size())
		);


	M_cursor_line->set_fill_visibility_status(true);
	M_cursor_line->set_fill_color(J_CLEAR);
	enable_default_key_char_processing();

	M_pen_poses.resize(1);
	M_pen_poses.front() = default_pen_pos();
	recalculate_letter_poses();

	M_selection.set_first_unordered_elem(M_cursor_pos);
	M_selection.set_second_unordered_elem(M_cursor_pos);
}

void J_Text_Box::initialize_frame_buffer(){
	s_open_gl.bind_framebuffer(M_framebuffer);
	s_open_gl.bind_texture_2D(M_texture_render_buffer);

	auto window = s_contexts->get_active_window();
	j_uint width = get_x_res(window);
	j_uint height = get_y_res(window);

	s_open_gl.tex_storage_2D(
		Texture_Target::TEXTURE_2D, 1
		, GL_Sized_Internal_Formats::RGBA8, width, height);

	{
	const ex_array<j_ubyte> image_data(4 * width*height, 0);

	s_open_gl.tex_sub_image_2D_ubyte(
		Texture_Target::TEXTURE_2D, 0, 0, 0, width, height
		, GL_Pixel_Formats::RGBA, image_data.data());

	}

	s_open_gl.attach_draw_framebuffer_texture_2D(
		GL_Attachment_Points::COLOR_ATTACHMENT0
		, Texture_Target::TEXTURE_2D, M_texture_render_buffer, 0);


	s_open_gl.debind_texture(Texture_Target::TEXTURE_2D);

	s_open_gl.bind_draw_framebuffer(J_GL_Framebuffer::null_object());
	s_open_gl.bind_read_framebuffer(J_GL_Framebuffer::null_object());

}



void J_Text_Box::key_input_cmd(int i_key, int i_scancode, int i_action, int i_modifiers){
	if(!M_key_input_command){
		return;
	}

	M_key_input_command(shared_from_this(), i_key, i_scancode, i_action, i_modifiers);
}

void J_Text_Box::char_input_cmd(int i_charcode){
	if(read_only_status()){
		return;
	}

	if(M_char_input_command){
		M_char_input_command(shared_from_this(), i_charcode);
	}
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

void J_Text_Box::change_color_at_pos(int i_pos, J_UI_Color i_color){
	auto string_it = M_multi_string.get_string_holding_index(i_pos);
	string_it->set_color(i_color);

	Pen_Pos_t indices_of_string = M_multi_string.get_string_indices(string_it);

	for(int i = indices_of_string.first; i < indices_of_string.second; ++i){
		J_Char_t char_chode = M_multi_string[i].charcode();
		J_Font_Face font_face = string_it->font_face();
		int char_width = font_face->bitmap_metric(char_chode).width;
		int char_height = font_face->bitmap_metric(char_chode).height;
		const j_ubyte* bitmap_data = font_face->get_data(char_chode);
		M_letter_box_string[i]->set_buffer_data(char_width, char_height, i_color, bitmap_data);
	}

	assert(!"This needs to be implemented");
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

void J_Text_Box::set_cursor_on(){
	M_cursor_line->set_fill_visibility_status(true);
	if(M_blinker_updater){
		M_blinker_updater->reset_timer();
	}
}

void J_Text_Box::enable_blinking_cursor(){
	if(M_blinker_updater){
		return;
	}
	M_blinker_updater 
		= J_Text_Cursor_Blinker_Updater_Shared_t(new J_Text_Cursor_Blinker_Updater);

	add_update_callback(M_blinker_updater);
}

void J_Text_Box::calculate_letter_boxes(j_size_t i_pos){
	assert(M_letter_box_string->size() >= M_multi_string.size());
	auto pos = M_multi_string.get_insert_pos(i_pos);
	M_pen_poses.resize(i_pos + 1);
	auto char_pos = pos.first;
	j_size_t index = i_pos;
	while(pos.second != M_multi_string.end()){
		J_UI_Color color = pos.second->color();
		J_Font_Face font_face = pos.second->font_face();
		while(char_pos != pos.second->end()){
			J_Char_t char_code = char_pos->charcode();
			auto& bitmap_metric = font_face->bitmap_metric(char_code);
			M_letter_box_string[index]
				->set_buffer_data(bitmap_metric
				, color, font_face->get_data(char_code));

			


			M_letter_box_string[index]->set_image_box(M_pen_poses[index], bitmap_metric);
			++index;
			
			calculate_next_pen_pos(char_code, bitmap_metric);
			

			++char_pos;
		}
		++pos.second;
		char_pos = pos.second->begin();
	}
	assert(M_pen_poses.size() == (M_multi_string.size() + 1));
}

void J_Text_Box::calculate_next_pen_pos(J_Char_t i_char_code, const Bitmap_Metrics& irk_bitmap){
	auto window = s_contexts->get_active_window();

	j_float width = to_x_screen(window, irk_bitmap.width);




	if('\n' == i_char_code){
		M_pen_poses.push_back(new_line_pen_pos(M_pen_poses.back()));
	} else if((M_pen_poses.back().first + width) > x2()){
		Pen_Pos_FL_t new_pos(new_line_pen_pos(M_pen_poses.back()));
		M_pen_poses.push_back(calculate_pen_advance(new_pos
			, irk_bitmap.advance_x));
	} else{
		M_pen_poses.push_back(calculate_pen_advance(M_pen_poses.back()
			, irk_bitmap.advance_x));
	}
}

void J_Text_Box::calculate_remaining_letter_poses(){
	assert(M_pen_poses.size() <= M_multi_string.size()+1);
	assert(M_multi_string.size() <= M_letter_box_string->size());

	for(j_size_t i = M_pen_poses.size()-1; i < M_multi_string.size(); i++){
		
		J_UI_String& cur_string = *M_multi_string.get_string_holding_index(i);
		J_Char_t charcode = M_multi_string.at_pos(i)->charcode();

		Bitmap_Metrics& bitmap_metric = cur_string.font_face()
			->bitmap_metric(charcode);

		M_letter_box_string[i]->set_image_box(M_pen_poses.back(), bitmap_metric);
		calculate_next_pen_pos(charcode, bitmap_metric);
		

		
	}
}

j_uint J_Text_Box::get_cursor_line_id()const{
	return M_cursor_line->get_ID();
}

void J_Text_Box::scroll_selection_boxes(j_float i_x_scroll, j_float i_y_scroll){
	for(auto f_box : M_selection_boxes){
		f_box->set_x(f_box->x1() + i_x_scroll);
		f_box->set_y(f_box->y2() + i_y_scroll);
	}
	set_selection_box_visibility_statuses();
}

Pen_Pos_FL_t J_Text_Box::default_pen_pos()const{
	return Pen_Pos_FL_t(x1() + K_LEFT_PADDING, y1()
						- to_y_screen(s_contexts->get_active_window(), M_new_line_size));
}

void J_Text_Box::auto_scroll_window(j_size_t i_pos){
	if(!auto_scrolling_status()){
		return;
	}
	if(is_cursor_pos_in_view(i_pos)){
		return;
	}

	auto pen_pos_y_lower = M_pen_poses[i_pos].second;
	auto pen_pos_y_upper = M_pen_poses[i_pos].second + new_line_screen_size();

	Pen_Pos_FL_t new_starting_pen_pos = M_pen_poses.front();
	if(pen_pos_y_upper > y1()){
		new_starting_pen_pos.second -= (pen_pos_y_upper - y1() + FLOAT_DELTA);
		new_starting_pen_pos.second
			= max(new_starting_pen_pos.second, default_pen_pos().second);
	} else{
		assert(pen_pos_y_lower < y2());
		new_starting_pen_pos.second += (y2() - pen_pos_y_lower + FLOAT_DELTA);
	}
	scroll_selection_boxes(0.0
						   , new_starting_pen_pos.second - M_pen_poses.front().second);

	set_starting_pen_pos(new_starting_pen_pos);

}

void J_Text_Box::set_starting_pen_pos(Pen_Pos_FL_t i_pen_pos){
	j_size_t cursor_pos = M_cursor_pos;

	M_pen_poses.resize(1);
	M_pen_poses.front() = i_pen_pos;

	recalculate_letter_poses();

	set_cursor_pos_no_scroll(cursor_pos);
}

void J_Text_Box::add_string(const J_UI_String& irk_string){
	if(M_multi_string.empty() 
	   || !M_multi_string.get_string_holding_index(M_cursor_pos)
	   ->is_same_type(irk_string)){

		M_multi_string.push_back(J_UI_String(irk_string.font_face(), irk_string.color()));
	}
	for(auto f_char : irk_string){
		insert_char(f_char);
	}

	set_cursor_pos_no_scroll(M_cursor_pos + static_cast<int>(irk_string.size()));
}

/*Pen_Pos_t calculate_pen_advance(const J_UI_String_Data*,Pen_Pos_t, int)*/
Pen_Pos_FL_t J_Text_Box::calculate_pen_advance(Pen_Pos_FL_t i_cur_pen, int i_advance_val_pixels)const{
	i_cur_pen.first += to_x_screen(s_contexts->get_active_window(), i_advance_val_pixels);

	return i_cur_pen;
}

/*Pen_Pos_t calculate_pen_advance(const J_UI_String_Data*,Pen_Pos_t, int)*/
Pen_Pos_FL_t J_Text_Box::new_line_pen_pos(Pen_Pos_FL_t i_pen)const{
	i_pen.first = x1() + K_LEFT_PADDING;
	i_pen.second -= to_y_screen(s_contexts->get_active_window(), M_new_line_size);
	return i_pen;
}

//
//void J_Text_Box::add_char(J_UI_Char i_char){
//	//assert(get_context() == s_contexts->get_active_context());
//	if(M_vao_ids.size() <= M_string.size()){
//		add_new_vao_and_vbo(safe_int_cast(2 * (M_vao_ids.size() + 1)));
//	}
//
//	auto& bitmap_metric = M_string.back().font_face()->M_bitmap_metrics[i_char.charcode()];
//
//	if('\n' == i_char.charcode()){
//		M_string.push_back(i_char);
//		M_pen_poses.push_back(new_line_pen_pos(M_pen_poses.back()));
//		return;
//	}
//	auto window = s_contexts->get_active_window();
//
//	j_float width = to_x_screen(window, bitmap_metric.width);
//	j_float height = to_y_screen(window, bitmap_metric.height);
//
//	j_float x_start, y_start;
//
//
//
//	if((M_pen_poses.back().first + width) > x2()){
//		Pen_Pos_FL_t new_pos(new_line_pen_pos(M_pen_poses.back()));
//		x_start = new_pos.first;
//		y_start = new_pos.second;
//		M_pen_poses.push_back(calculate_pen_advance(new_pos
//			, bitmap_metric.advance_x));
//	} else{
//		x_start = M_pen_poses.back().first;
//		y_start = M_pen_poses.back().second;
//		M_pen_poses.push_back(calculate_pen_advance(M_pen_poses.back()
//			, bitmap_metric.advance_x));
//	}
//	j_float x_screen_pos = x_start
//		+ to_x_screen(window, bitmap_metric.left);
//
//	j_float y_screen_pos = y_start
//		- to_y_screen(window, bitmap_metric.underreach);
//
//
//	//cerr << "\nx_screen_pos: " << x_screen_pos << " y_screen_pos: " << y_screen_pos 
//	//	<< " Text Width: " << width << " Text Height: " << height;
//
//	array<j_float, 24> vertex_array_data = {
//		x_screen_pos, y_screen_pos, 0.0f, 1.0f
//		, x_screen_pos + width, y_screen_pos, 0.0f, 1.0f
//		, x_screen_pos + width, y_screen_pos + height, 0.0f, 1.0f
//		, x_screen_pos, y_screen_pos + height, 0.0, 1.0f
//
//		, 0.0f, 1.0f
//		, 1.0f, 1.0f
//		, 1.0f, 0.0f
//		, 0.0f, 0.0f
//	};
//	j_size_t index = M_string.size();
//	M_string.push_back(i_char.charcode());
//
//	glBindVertexArray(M_vao_ids[index]);
//	glBindBuffer(GL_ARRAY_BUFFER, M_vao_buffer_ids[index]);
//
//	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertex_array_data)
//					, vertex_array_data.data());
//
//	glBindVertexArray(0);
//
//}

bool J_Text_Box::auto_scrolling_status()const {
	return M_auto_scrolling_status;
}

void J_Text_Box::set_auto_scrolling_status(bool i_status){
	M_auto_scrolling_status = i_status;
}

bool J_Text_Box::is_cursor_pos_in_view(j_size_t i_pos)const{
	auto y_bottom_pos = M_pen_poses[i_pos].second;
	auto y_top_pos = y_bottom_pos + new_line_screen_size();
	return is_y_inside(y_bottom_pos) && is_y_inside(y_top_pos);
}

j_float J_Text_Box::new_line_screen_size()const{
	return to_y_screen(s_contexts->get_active_window(), M_new_line_size);
}

void J_Text_Box::set_cursor_line_position(j_size_t i_cursor_pos){
	assert(between_inclusive(i_cursor_pos, J_SIZE_T_ZERO, M_multi_string.size()));

	M_cursor_pos = i_cursor_pos;


	M_cursor_line->set_line(get_cursor_line(M_pen_poses[M_cursor_pos]));

	set_cursor_on();
}

void J_Text_Box::recalculate_letter_poses(){
	M_pen_poses.resize(1);
	//assert(M_pen_poses.back() == default_pen_pos());
	calculate_remaining_letter_poses();
}

J_Line J_Text_Box::get_cursor_line(const Pen_Pos_FL_t& i_pen_pos)const{
	return J_Line(i_pen_pos.first, i_pen_pos.second
				  , i_pen_pos.first, i_pen_pos.second + new_line_screen_size());
}

void J_Text_Box::notify_letter_box_poses(j_size_t i_pos /*= J_SIZE_T_ZERO*/)const{
	assert(!"This needs to be removed");
	ex_array<Pen_Pos_FL_t> pen_poses(M_pen_poses.begin() + i_pos, M_pen_poses.end());
	for(j_size_t i = i_pos; i < M_pen_poses.size(); i++){
		auto char_pos = M_multi_string.get_insert_pos(i);
		const Bitmap_Metrics& metric= char_pos.second->font_face()
			->bitmap_metric(char_pos.first->charcode());

		int underreach = metric.underreach;
		int left = metric.left;

		pen_poses[i - i_pos].first += to_x_screen(s_contexts->get_active_window(), left);

		pen_poses[i - i_pos].second
			-= to_y_screen(s_contexts->get_active_window(), underreach);
	}

}


void J_Text_Box::insert_string(
	J_UI_Multi_String::const_iterator i_pos, const J_UI_String& irk_string){
	j_size_t index = M_multi_string.get_string_indices(i_pos).first;

	M_multi_string.insert(i_pos, irk_string);

	expand_num_letter_boxes();
	calculate_letter_boxes(index);

}
void J_Text_Box::insert_string_silent(j_size_t i_index, const J_UI_Multi_String& irk_string){

	if(M_multi_string.empty()){
		assert(0 == i_index);
		M_multi_string = irk_string;
	}else{
		M_multi_string.insert(i_index, irk_string);
	}
	


	ex_array<J_UI_Letter_Box_Shared_t> letter_boxes = make_letter_boxes(irk_string);

	M_letter_box_string->insert(M_letter_box_string->begin() + i_index
								, letter_boxes.begin(), letter_boxes.end());



	M_pen_poses.resize(i_index+1);

	calculate_remaining_letter_poses();

	set_cursor_pos_no_scroll(i_index + irk_string.size());

}

ex_array<J_UI_Letter_Box_Shared_t> 
	J_Text_Box::make_letter_boxes(const J_UI_Multi_String& irk_string){

	ex_array<J_UI_Letter_Box_Shared_t> letter_boxes;
	for(const auto& f_string : irk_string){
		auto next_letter_box =  make_letter_boxes(f_string);
		letter_boxes.insert(letter_boxes.end(), next_letter_box.begin(), next_letter_box.end());
	}

	return letter_boxes;
}

ex_array<J_UI_Letter_Box_Shared_t> J_Text_Box::make_letter_boxes(const J_UI_String& irk_string){
	ex_array<J_UI_Letter_Box_Shared_t> letter_boxes;
	J_UI_Color color = irk_string.color();
	J_Font_Face font_face = irk_string.font_face();

	for(int i = 0; i < irk_string.size(); i++){
		J_Char_t char_code = irk_string[i].charcode();
		J_UI_Letter_Box_Shared_t new_letter_box(new J_UI_Letter_Box(J_Rectangle()));
		
		new_letter_box->set_buffer_data(font_face->bitmap_metric(char_code)
										, color, font_face->get_data(char_code));
		letter_boxes.push_back(new_letter_box);
	}
	return letter_boxes;
}

void J_Text_Box::mouse_button_press(int i_button, int , Pen_Pos_FL_t i_pos){
	
	switch(i_button){
	case J_LEFT_MOUSE_BUTTON:{
		clear_selection_boxes();
		set_cursor_pos(get_cursor_index(i_pos));
		M_selection.set_first_unordered_elem(M_cursor_pos);
		M_selection.set_second_unordered_elem(M_cursor_pos);
		set_left_click_on();
		}
		break;
	case J_MOUSE_WHEEL_UP:
		scroll(lines_scrolled_per_tick());
		break;
	case J_MOUSE_WHEEL_DOWN:
		scroll(-lines_scrolled_per_tick());
		break;
	default:
		;
	}
}

void J_Text_Box::mouse_button_press_n(int i_button, int, Pen_Pos_FL_t i_pos, int i_count){

	switch(i_button){
	case J_LEFT_MOUSE_BUTTON:{
								 clear_selection_boxes();
								 set_cursor_pos(get_cursor_index(i_pos));
								 M_selection.set_first_unordered_elem(M_cursor_pos);
								 M_selection.set_second_unordered_elem(M_cursor_pos);
								 set_left_click_on();
	}
		break;
	case J_MOUSE_WHEEL_UP:
		scroll(i_count*lines_scrolled_per_tick());
		break;
	case J_MOUSE_WHEEL_DOWN:
		scroll(i_count*-lines_scrolled_per_tick());
		break;
	default:
		;
	}
}

void J_Text_Box::mouse_button_release(int i_button, int , Pen_Pos_FL_t i_pos){

	switch(i_button){
	case J_LEFT_MOUSE_BUTTON:
		set_left_click_off();
		set_cursor_pos(get_cursor_index(i_pos));
		break;
	default:
		;
	}
}

J_Text_Box_Shared_t J_Text_Box::shared_from_this(){
	auto this_text_box_ptr
		= dynamic_pointer_cast<J_Text_Box>(J_UI_Object::shared_from_this());
	assert(this_text_box_ptr);
	return this_text_box_ptr;
}

void J_Text_Box::set_cursor_visibility_status(bool i_status){
	if(M_cursor_line->fill_visibility_status() == i_status){
		return;
	}
	M_cursor_line->set_fill_visibility_status(i_status);
}

bool J_Text_Box::cursor_visibility_status()const{
	return M_cursor_line->fill_visibility_status();
}

bool J_Text_Box::insert_char(J_UI_Char i_char){
	M_multi_string.insert(M_cursor_pos, i_char);
	auto& cur_string = *M_multi_string.get_string_holding_index(M_cursor_pos);
	Bitmap_Metrics& bitmap_metrics
		= cur_string.font_face()->bitmap_metric(i_char.charcode());

	auto& cur_pen_pos = M_pen_poses[M_cursor_pos];

	J_UI_Letter_Box_Shared_t new_letter_box(new J_UI_Letter_Box(J_Rectangle()));
	new_letter_box->set_image_box(cur_pen_pos, bitmap_metrics);
	new_letter_box->set_buffer_data(
		bitmap_metrics, cur_string.color()
		, cur_string.font_face()->get_data(i_char.charcode()));



	M_pen_poses.resize(M_cursor_pos+1);
	M_letter_box_string
		->insert(M_letter_box_string->begin() + M_cursor_pos, new_letter_box);

	calculate_remaining_letter_poses();

	
	set_cursor_pos(M_cursor_pos + 1);
	set_cursor_on();

	return true;
}

/*int get_cursor_index(Pen_Pos_FL_t)const*/
j_size_t J_Text_Box::get_cursor_index(Pen_Pos_FL_t i_pen_pos)const{
	j_float line_screen_size = to_y_screen(s_contexts->get_active_window(), M_new_line_size);

	Text_Line_Order_Comp text_pos_compare_fo(M_pen_poses.front().second
											 + line_screen_size, line_screen_size);

	assert(is_sorted(M_pen_poses.begin(), M_pen_poses.end()
		, text_pos_compare_fo));

	ptrdiff_t pos = lower_bound(M_pen_poses.begin(), M_pen_poses.end(), i_pen_pos
								, text_pos_compare_fo)
								- M_pen_poses.begin();

	j_size_t string_size = M_multi_string.size();
	return max(min(pos - 1, string_size), J_SIZE_T_ZERO);
}

void J_Text_Box::move_cursor_line_pos_up(j_size_t i_move_val){
	Pen_Pos_FL_t new_pen_pos = M_last_set_cursor_pos;
	new_pen_pos.second += i_move_val*new_line_screen_size();
	set_cursor_pos(get_cursor_index(new_pen_pos));
	M_last_set_cursor_pos = M_pen_poses[M_cursor_pos];
	M_last_set_cursor_pos.first = new_pen_pos.first;
}

void J_Text_Box::move_cursor_line_pos_down(j_size_t i_move_val){
	Pen_Pos_FL_t new_pen_pos = M_last_set_cursor_pos;
	new_pen_pos.second -= (i_move_val*new_line_screen_size() + FLOAT_DELTA);
	set_cursor_pos(get_cursor_index(new_pen_pos));
	M_last_set_cursor_pos = M_pen_poses[M_cursor_pos];
	M_last_set_cursor_pos.first = new_pen_pos.first;

}

void J_Text_Box::move_cursor_to_line_begin(){
	j_size_t cursor_pos = M_cursor_pos;
	
	const j_float init_line_pos = M_pen_poses[cursor_pos].second;
	while(0 < cursor_pos){
		assert(init_line_pos == M_pen_poses[cursor_pos].second);
		if(init_line_pos == M_pen_poses[cursor_pos - 1].second){
			--cursor_pos;
		} else{
			break;
		}
	}

	set_cursor_pos(cursor_pos);
}

void J_Text_Box::move_cursor_to_line_end(){
	j_size_t cursor_pos = M_cursor_pos;

	const j_float init_line_pos = M_pen_poses[cursor_pos].second;
	while(cursor_pos < M_pen_poses.size() - 1){
		assert(init_line_pos == M_pen_poses[cursor_pos].second);
		if(init_line_pos == M_pen_poses[cursor_pos + 1].second){
			++cursor_pos;
		} else{
			break;
		}
	}

	set_cursor_pos(cursor_pos);
}

void J_Text_Box::scroll(int i_scroll_val){
	if(!i_scroll_val){
		return;
	}

	j_float scroll_size = -i_scroll_val*new_line_screen_size();

	if((scroll_size < 0) && M_pen_poses.front() == default_pen_pos()){
		return;
	}

	if(scroll_size < 0){
		j_float scroll_size_to_default
			= default_pen_pos().second - M_pen_poses.front().second;
		scroll_size = max(scroll_size, scroll_size_to_default);
	} else if(M_pen_poses.back().second > y2()){
		return;
	} else{
		scroll_size = min(scroll_size, y2() - M_pen_poses.back().second);
	}
	auto new_starting_pen_pos = M_pen_poses.front();
	new_starting_pen_pos.second += scroll_size;

	set_starting_pen_pos(new_starting_pen_pos);
	scroll_selection_boxes(0.0f, scroll_size);
	if(!is_cursor_pos_in_view(M_cursor_pos)){
		M_cursor_line->set_line(J_Line());
	} else{
		set_cursor_pos(M_cursor_pos);
	}
}

void J_Text_Box::alert_cursor_pos(Pen_Pos_FL_t i_pos){
	if(!left_click_on_status()){
		return;
	}
	j_set_cursor_type(J_I_BEAM_CURSOR_ID);
	j_size_t cursor_index = get_cursor_index(i_pos);
	set_cursor_pos(cursor_index);
	M_selection.set_second_unordered_elem(cursor_index);
	j_size_t low_bound = M_selection.first();
	j_size_t right_bound = M_selection.second();



	if(low_bound == right_bound){
		clear_selection_boxes();
		return;
	}

	int num_display_boxes = 0;

	auto window = s_contexts->get_active_window();
	auto cur_cursor_pos = low_bound;
	while(cur_cursor_pos < right_bound){
		J_Rectangle rectangle(M_pen_poses[cur_cursor_pos].first
							  , M_pen_poses[cur_cursor_pos].second
							  , 0.0f, new_line_screen_size());

		j_float end_x_pos = rectangle.x1()
			+ to_x_screen(window, M_multi_string.bitmap_metric(cur_cursor_pos).advance_x);


		while((cur_cursor_pos < right_bound)
			  && (M_pen_poses[cur_cursor_pos].second == rectangle.y2())){
			end_x_pos = M_pen_poses[cur_cursor_pos].first;
			end_x_pos += to_x_screen(window, M_multi_string.bitmap_metric(cur_cursor_pos).advance_x);

			++cur_cursor_pos;
		}
		rectangle.set_x2(end_x_pos);
		rectangle.set_y(rectangle.y2() - 0.01f);
		if(M_selection_boxes.size() <= num_display_boxes){
			assert(M_selection_boxes.size() == num_display_boxes);
			
			J_UI_Box_Shared_t display_box(
				new J_UI_Box(rectangle)
				);
			M_selection_boxes.push_back(display_box);
		}

		M_selection_boxes[num_display_boxes]->set_rectangle(rectangle);
		set_selection_box_settings(M_selection_boxes[num_display_boxes]);

		++num_display_boxes;
	}

	M_selection_boxes.resize(num_display_boxes);
	set_selection_box_visibility_statuses();

}

int J_Text_Box::lines_scrolled_per_tick()const{
	return 1;
}

void J_Text_Box::set_selection_box_settings(J_UI_Box_Shared_t i_box)const{
	i_box->set_fill_visibility_status(true);
	i_box->set_fill_color(J_BLUE*0.4f + J_RED*0.08f);
	i_box->set_outline_color(J_WHITE);

}

/*void insert_string(const J_UI_String&)*/
void J_Text_Box::insert_string(const J_UI_String& irk_string){
	insert_string(get_cursor_pos(), irk_string);
}

void J_Text_Box::insert_string(const J_UI_Multi_String& irk_string){
	insert_string(get_cursor_pos(), irk_string);
}

/*void insert_string(int pos, const J_UI_String&)*/
void J_Text_Box::insert_string(j_size_t i_pos, const J_UI_String& irk_string){
	
	insert_string_silent(i_pos, irk_string);

	set_cursor_pos(i_pos + irk_string.size());
}

/*void insert_string(int pos, const J_UI_String&)*/
void J_Text_Box::insert_string(j_size_t i_pos, const J_UI_Multi_String& irk_string){

	insert_string_silent(i_pos, irk_string);

	set_cursor_pos(i_pos + irk_string.size());
}




void J_Text_Box::set_string(const J_UI_String& irk_string){


	M_multi_string = J_UI_Multi_String(irk_string);
	expand_num_letter_boxes();

	recalculate_letter_boxes();
	


	set_cursor_on();
	set_cursor_pos(irk_string.size());
}

void J_Text_Box::move_cursor(j_size_t i_amt){
	j_size_t new_cursor =  M_cursor_pos + i_amt;
	if(new_cursor < 0){
		new_cursor = 0;
	}else if(new_cursor > M_multi_string.size()){
		new_cursor = static_cast<int>(M_multi_string.size());
	}

	M_last_set_cursor_pos = M_pen_poses[new_cursor];
	M_cursor_line->set_line(get_cursor_line(M_pen_poses[M_cursor_pos]));
	set_cursor_pos(new_cursor);
	set_cursor_on();
	
}

void J_Text_Box::set_cursor_pos(j_size_t i_cursor_pos){
	set_cursor_line_position(i_cursor_pos);

	M_last_set_cursor_pos = M_pen_poses[i_cursor_pos];
	auto_scroll_window(i_cursor_pos);
}

void J_Text_Box::set_cursor_pos_no_scroll(j_size_t i_pos){
	set_cursor_line_position(i_pos);
}

void J_Text_Box::silent_set_cursor_pos(j_size_t i_cursor_pos){
	assert(between_inclusive(i_cursor_pos, J_SIZE_T_ZERO, M_multi_string.size()));

	M_cursor_pos = i_cursor_pos;
	set_cursor_on();
}

void J_Text_Box::set_cursor_color(const J_Color_RGBA<j_float> i_color){
	M_cursor_line->set_fill_color(i_color);
	set_cursor_on();
}

void J_Text_Box::backspace(){
	if(0 == M_cursor_pos){
		return;
	}

	M_multi_string.erase(--M_cursor_pos, 1);


	set_cursor_pos(M_cursor_pos);


	
	calculate_letter_boxes(M_cursor_pos);
	set_cursor_on();
}

void J_Text_Box::delete_char(){
	if(M_multi_string.size() == M_cursor_pos){
		return;
	}

	erase_chars(M_cursor_pos, 1);
}

/*void erase_chars(int pos, int size)*/
void J_Text_Box::erase_chars(j_size_t i_pos, j_size_t i_size){
	set_cursor_pos(i_pos);

	M_multi_string.erase(i_pos, i_size);
	M_letter_box_string->erase(i_pos, i_size);

	M_pen_poses.resize(i_pos+1);
	calculate_remaining_letter_poses();
}



void J_Text_Box::clear_selection_boxes(){
	M_selection_boxes.clear();
}

void J_Text_Box::set_selection_box_visibility_statuses(){

	for(J_UI_Box_Shared_t f_box : M_selection_boxes){
		bool is_visible = true;
		if(!is_y_inside(f_box->y1())){
			is_visible = false;
		}
		if(!is_y_inside(f_box->y2())){
			is_visible = false;
		}
		f_box->set_fill_visibility_status(is_visible);
	}
}

j_size_t J_Text_Box::selection_start()const{
	return M_selection.first();
}

j_size_t J_Text_Box::selection_end()const{
	return M_selection.second();
}

J_Text_Box_Object::J_Text_Box_Object(const J_Rectangle& i_rec):J_UI_Box(i_rec){}

J_Text_Box_Object::~J_Text_Box_Object(){}



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

static int s_cursor_blinker_ids = 0;

ex_array<j_dbl> J_Text_Cursor_Blinker_Updater::Ms_blink_rates;

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

j_dbl J_Text_Cursor_Blinker_Updater::blink_rate()const{
	return Ms_blink_rates[M_ID];
}

void J_Text_Cursor_Blinker_Updater::set_blink_rate(j_dbl i_rate){
	Ms_blink_rates[M_ID] = i_rate;
}

J_Text_Cursor_Blinker_Updater::J_Text_Cursor_Blinker_Updater()
	:M_ID(++s_cursor_blinker_ids), M_timer(bind(&J_UI::time, &J_UI::get_instance())
	, bind(&J_Text_Cursor_Blinker_Updater::get_blink_rate, M_ID)){
	if(M_ID >= Ms_blink_rates.size()){
		Ms_blink_rates.resize(M_ID + 1);
	}
	Ms_blink_rates[M_ID] = DEFAULT_CURSOR_REFESH_TIME;
}

j_dbl J_Text_Cursor_Blinker_Updater::get_blink_rate(int i_id){
	return Ms_blink_rates[i_id];
}

void J_Text_Cursor_Blinker_Updater::reset_timer(){
	M_timer.reset_timer();
}

void J_Text_Box::alert_changed(){
	M_changed_flag = true;
}



/*void J_Text_Box::draw()const*/
void J_Text_Box::draw()const{
	J_UI_Box::draw();
	M_cursor_line->draw();
	if(M_changed_flag){
		render_frame_buffer();
	}


	assert(M_framebuffer.valid());
	assert(M_framebuffer.get_ID());
	s_open_gl.bind_draw_framebuffer(J_GL_Framebuffer::null_object());



	s_open_gl.use_program(image_shader_id());

	

	s_open_gl.bind_texture_2D(M_texture_render_buffer);

	s_open_gl.bind_vertex_array(s_contexts->screen_box_vao());

	s_open_gl.bind_texture_2D(M_texture_render_buffer);

	s_open_gl.draw_arrays(Array_Draw_Mode::TRIANGLE_FAN, 0, 4);

	s_open_gl.debind_buffer(GL_Buffer_Targets::ARRAY_BUFFER);
	s_open_gl.debind_texture(Texture_Target::TEXTURE_2D);

	

	s_open_gl.debind_program();


}

void J_Text_Box::render_frame_buffer()const{
	j_uint prev_width = get_x_res(s_contexts->get_active_window());
	j_uint prev_height = get_y_res(s_contexts->get_active_window());;


	s_open_gl.bind_draw_framebuffer(M_framebuffer);

	s_open_gl.viewport(0, 0, prev_width, prev_height);

	s_open_gl.set_clear_color(0.0f, 0.0f, 0.0f, 0.0f);

	j_clear();


	auto letter_draw_end = M_letter_box_string->begin() + M_multi_string.size();

	auto start_view_pos
		= lower_bound(M_letter_box_string->begin(), letter_draw_end
		, J_Rectangle_Shared_t(new J_Rectangle(0.0f, y1(), 0.0f, 0.0f))
		, [](J_Rectangle_Shared_t i_left, J_Rectangle_Shared_t i_right){
		j_float compare_val = i_left->y1() - i_right->y1();
		//hot fix
		if(compare_val > 0.01f){
			return true;
		} else if(compare_val < -0.01f){
			return false;
		} else{
			return i_left->x1() < i_left->x2();
		}
	});

	auto end_view_pos
		= lower_bound(M_letter_box_string->begin(), letter_draw_end
		, J_Rectangle_Shared_t(new J_Rectangle(0.0f, y2(), 0.0f, 0.0f))
		, [](J_Rectangle_Shared_t i_left, J_Rectangle_Shared_t i_right){
		j_float compare_val = i_left->y2() - i_right->y2();
		//hot fix
		if(compare_val > 0.01f){
			return true;
		} else if(compare_val < -0.01f){
			return false;
		} else{
			return i_left->x1() < i_left->x2();
		}
	});




	while(start_view_pos != end_view_pos){
		(*start_view_pos++)->draw();
	}

	s_open_gl.bind_draw_framebuffer(J_GL_Framebuffer::null_object());

	s_open_gl.viewport(0, 0, prev_width, prev_height);
	M_changed_flag = false;

}

J_Text_Box::~J_Text_Box(){

}

void J_Text_Box::expand_num_letter_boxes(){
	while(M_letter_box_string->size() < M_multi_string.size()){
		M_letter_box_string->push_back(J_UI_Letter_Box_Shared_t(new J_UI_Letter_Box(J_Rectangle())));
	}
}

void J_Text_Box::recalculate_letter_boxes(){
	M_pen_poses.front() = default_pen_pos();
	calculate_letter_boxes(0);
}

const J_UI_Multi_String& J_Text_Box::multi_string()const{
	return M_multi_string;
}

void J_Text_Box::alert_resize(int /*i_width*/, int /*i_length*/){
	reset_frame_buffer();
	recalculate_letter_boxes();
}

void J_Text_Box::reset_frame_buffer(){
	M_framebuffer = J_GL_Framebuffer();
	M_texture_render_buffer = J_GL_Texture();
	initialize_frame_buffer();
}








}




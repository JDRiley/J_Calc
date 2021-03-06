#include "Math_Input_Box.h"

#include "J_Calc_Error.h"
//
#include <J_UI/J_UI_Controller.h>

#include <iostream>
//Algorithms
#include <algorithm>
//
#include <functional>
//
#include <J_UI/J_System_Signals.h>
using std::bind; using std::is_sorted; using std::less; using std::lower_bound; using std::mem_fn;
using namespace std::placeholders; using std::dynamic_pointer_cast;

//Utilities,
#include <cassert>
//
#include <vector>
using namespace J_UI;
using J_UI::Char_Press_Signal;
namespace jomike{


//Math Input Text Box Functions**********************************************************

extern const char LINE_END_SYMBOL = ';';

extern const J_UI_Color G_DEFAULT_OUTPUT_COLOR = J_Color::White;

//Constructors--------------------------------------------------
Math_Input_Box::Math_Input_Box(const J_UI::J_UI_String& irk_input_string) :J_Text_Box(irk_input_string)
	, M_line_inputs(1, Line_Input(0, irk_input_string)){
	insert_string(J_UI_String(GK_DEFAULT_OUTPUT_STRING, irk_input_string.font_face() ));
	
	set_key_input_command(math_key_input_cmd);
	set_char_input_command(math_box_char_input_cmd);

	
	link_signal(view_char_press_signal(), &Math_Input_Box::char_press_signal_receiver);
	link_signal(view_key_press_signal(), &Math_Input_Box::key_press_signal_receiver);

}


void Math_Input_Box::key_press_signal_receiver(J_UI::Key_Press_Signal* i_signal){
	
	int action = i_signal->action();
	int modifiers = i_signal->modifiers();
	int charcode = i_signal->key();
	int scancode = i_signal->scancode();


	if((J_PRESS != action) && (J_REPEAT != action)){
		return;
	}

	j_size_t cursor_pos = get_cursor_pos();



	assert(cursor_pos >= 0);

	auto line_input_pos = get_line_pos_at_pos(cursor_pos);

	if(modifiers & J_MOD_CONTROL){
		switch(charcode){
		case J_KEY_I:
			//insert_integration_subroutine(cursor_pos);
			break;
		default:
			;
		}
	} else if(modifiers & J_MOD_SHIFT){
		switch(charcode){
		case J_KEY_ENTER:
			eval_and_break_line_input();
			return;
		default:
			;
		}
	} else{
		switch(charcode){
		case J_KEY_BACKSPACE:
			key_backspace();
			return;
		case J_KEY_ENTER:
			insert_char('\n');
			return;
		case J_KEY_TAB:
			insert_char('\t');
			return;
		case J_KEY_DELETE:
			key_delete();
			return;
		case J_KEY_LEFT:
			move_cursor(-1);
			return;
		case J_KEY_RIGHT:
			move_cursor(1);
			return;
		case J_KEY_KP_ENTER:
			eval_and_break_line_input();
			return;
		default:
			;
		}
	}

	standard_text_box_input_parser(this, charcode, scancode, action, modifiers);
}

/*void insert_line_input(const Line_Input&)*/
void Math_Input_Box::insert_line_input(const Line_Input& irk_line_input){
	auto insert_pos = lower_bound(M_line_inputs.begin(), M_line_inputs.end()
		, irk_line_input.start_pos(), Line_Cursor_Comp());
	insert_pos = M_line_inputs.insert(insert_pos, irk_line_input);

	J_Text_Box::insert_string(
		irk_line_input.start_pos(), irk_line_input.input_str() + irk_line_input.output_str());

	for_each(insert_pos + 1, M_line_inputs.end()
		, bind(&Line_Input::increment_pos, _1, irk_line_input.size()));
	if(irk_line_input.output_status()){
		set_cursor_pos(irk_line_input.end_pos());
	}else{
		set_cursor_pos(irk_line_input.start_pos());
	}
}

void Math_Input_Box::math_key_input_cmd(J_Text_Box_Object* i_text_box, int i_charcode
	, int i_scancode, int i_action, int i_modifiers){
	auto math_box = safe_dynamic_cast<J_Text_Box_Object, Math_Input_Box>(i_text_box);

	if((J_PRESS != i_action ) && (J_REPEAT != i_action)){
		return;
	}

	j_size_t cursor_pos = math_box->get_cursor_pos();


	assert(cursor_pos >= 0 );

	auto line_input_pos = math_box->get_line_pos_at_pos(cursor_pos);

	if(i_modifiers & J_MOD_CONTROL){
		switch(i_charcode){
		case J_KEY_I:
			//insert_integration_subroutine(cursor_pos);
			break;
		default:	
			;
		}
	} else if(i_modifiers & J_MOD_SHIFT){
		switch(i_charcode){
		case J_KEY_ENTER:
			math_box->eval_and_break_line_input();
			return;
		default:
			;
		}
	}else{
		switch(i_charcode){
		case J_KEY_BACKSPACE:
			math_box->key_backspace();
			return;
		case J_KEY_ENTER:
			math_box->insert_char('\n');
			return;
		case J_KEY_TAB:
			math_box->insert_char('\t');
			return;
		case J_KEY_DELETE:
			math_box->key_delete();
			return;
		case J_KEY_LEFT:
			math_box->move_cursor(-1);
			return;
		case J_KEY_RIGHT:
			math_box->move_cursor(1);
			return;
		case J_KEY_KP_ENTER:
			math_box->eval_and_break_line_input();
			return;
		default:
			;
		}
	}
	
	standard_text_box_input_parser(i_text_box, i_charcode, i_scancode, i_action, i_modifiers);
}

void Math_Input_Box::key_backspace(){
	if(0 == get_cursor_pos()){
		//No Characters to delete
		return;
	}
	auto line_input_pos = get_line_pos_at_pos(get_cursor_pos()-1);
	j_size_t char_pos = get_cursor_pos() - 1 - line_input_pos->start_pos();

	if(line_input_pos->read_only_status(char_pos + 1) || read_only_status()){
		return;
	}


	//j_size_t initial_line_start_pos = line_input_pos->start_pos();
	if(line_input_pos->is_inside_input(char_pos)){
		line_input_pos = clear_output(line_input_pos);
		//line_input = *line_input_pos;
	}else{
		for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::decrement_pos, _1, 1));
		J_Text_Box::backspace();
		return;
	}
	//J_Text_Box::move_cursor(line_input_pos->start_pos() - initial_line_start_pos);
	if(char_pos != line_input_pos->input_str().size()){
		line_input_pos->delete_char(char_pos);
		for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::decrement_pos, _1, 1));
		J_Text_Box::backspace();
	}
	

}

void Math_Input_Box::key_delete(){
	if(get_string().size() <= get_cursor_pos()){
		//No Characters to delete
		return;
	}
	auto line_input_pos = get_line_pos_at_pos(get_cursor_pos());

	//j_size_t initial_line_start_pos = line_input_pos->start_pos();

	j_size_t char_pos = get_cursor_pos() - line_input_pos->start_pos();
	
	if(line_input_pos->read_only_status(char_pos + 1) || read_only_status()){
		return;
	}

	if(line_input_pos->output_status() && (get_cursor_pos() == line_input_pos->output_start_pos())){
		return;
	}

	line_input_pos->delete_char(char_pos);
	for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::decrement_pos, _1, 1));
	J_Text_Box::delete_char();

	line_input_pos = clear_output(line_input_pos);
	

}

void Math_Input_Box::char_press_signal_receiver(Char_Press_Signal* i_signal){
	if(is_read_only_status(get_cursor_pos())){
		return;
	}

	insert_char(i_signal->get_char());
	//insert_char('A');
}

/*void char_input_cmd(j_window_t, j_ulint)*/
void Math_Input_Box::math_box_char_input_cmd(J_Text_Box_Object* i_math_box
	, int i_charcode){
	auto math_box = safe_dynamic_cast<J_Text_Box_Object,Math_Input_Box>(i_math_box);

	if(math_box->is_read_only_status(math_box->get_cursor_pos())){
		return;
	}

	//auto line_input_pos = math_box->get_line_pos_at_pos(i_math_box->get_cursor_pos());


	//line_input_pos = math_box->clear_output(line_input_pos);
	//switch(i_charcode){

	//default:
		math_box->insert_char(i_charcode);
	//	break;
	//}
}


/*void eval_and_break_line_input(Line_Input_Shared_t, int)*/
void Math_Input_Box::eval_and_break_line_input(){
	j_size_t old_cursor_pos = get_cursor_pos();

	auto line_input_pos = lower_bound_line_input(old_cursor_pos);
	if(line_input_pos != M_line_inputs.end()){
		line_input_pos = line_input_pos->start_pos() == old_cursor_pos 
			?  line_input_pos : line_input_pos - 1;
	}else{
		--line_input_pos;
	}


	j_size_t pos = old_cursor_pos - line_input_pos->start_pos();
	if(!pos){
		//Empty String
		return;
	}
	assert(!line_input_pos->read_only_status(pos));

	J_UI_String new_input_string(line_input_pos->input_str().substr(pos));

	assert(pos <= line_input_pos->input_str().size());


	assert(!line_input_pos->output_status());



	//line_input_pos->advance_whitespace();
	j_size_t input_str_size = line_input_pos->input_str().size();
	j_size_t output_str_size = line_input_pos->output_str().size();
	line_input_pos->set_input_str(line_input_pos->input_str().substr(0, pos));

	line_input_pos->evaluate_output();
	j_size_t start_pos_move_size = line_input_pos->input_str().size() - input_str_size 
		+ line_input_pos->output_str().size() - output_str_size;
	

	J_Text_Box::erase_chars(line_input_pos->start_pos(), input_str_size + output_str_size);
//#error insert string must take a valid font face. right now it's not.'
	J_Text_Box::insert_string(line_input_pos->start_pos(), line_input_pos->input_str() + line_input_pos->output_str());

	assert(get_cursor_pos() 
		== (line_input_pos->start_pos()	+ line_input_pos->input_str().size() + line_input_pos->output_str().size()));
	
	assert(get_cursor_pos() == (line_input_pos->end_pos()));

	for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::increment_pos, _1, start_pos_move_size));
	
	Line_Input new_input(line_input_pos->make_empty_from(get_cursor_pos()));
	
	new_input.set_input_str(new_input_string);

	if(new_input_string.size() || ((line_input_pos + 1) == M_line_inputs.end())){
		insert_line_input(new_input);
	}
	
	assert(is_sorted(M_line_inputs.begin(), M_line_inputs.end()
		, bind(less<j_size_t>(), bind(&Line_Input::start_pos, _1), bind(&Line_Input::start_pos, _2))));
}

/*Line_Input_it clear_output(Line_Input_Cont_t::iterator)*/
Math_Input_Box::Line_Input_it Math_Input_Box::clear_output(Line_Input_it i_pos){
	if(!i_pos->output_status()){
		return i_pos;
	}
	j_size_t return_cursor_pos = get_cursor_pos();//i_pos->input_end_pos() == get_cursor_pos() ?  get_cursor_pos() - 1 : get_cursor_pos();

	
	j_size_t deletetion_size = i_pos->output_str().size();// + 1/*The "+1" is cause we added the Line_END_SYMBOL char for deletion*/;



	J_Text_Box::erase_chars(i_pos->output_start_pos()// - 1 /*The "-1" is to delete the LINE_END_SYMBOL char*/
		, deletetion_size);

	i_pos->clear_output();
	//i_pos->delete_char(i_pos->input_str().size() -1);

	J_Text_Box::insert_string(i_pos->output_str());
	J_Text_Box::set_cursor_pos(return_cursor_pos);
	


	for_each(i_pos+1, M_line_inputs.end()
		, bind(&Line_Input::decrement_pos, _1, deletetion_size - DEFAULT_OUTPUT_STRING_SIZE));

	if((M_line_inputs.begin() != i_pos) && !(i_pos-1)->output_status()){
		i_pos = join_to_next(i_pos-1);
		
	}
	if((M_line_inputs.end() != (i_pos+1)) && !(i_pos+1)->output_status()){
		i_pos = join_to_next(i_pos);
	}
	return i_pos;
}

/*Line_Input_it join_to_next(Line_Input_it)*/
Math_Input_Box::Line_Input_it Math_Input_Box::join_to_next(Line_Input_it i_pos){
	auto next_pos = i_pos + 1;
	assert(i_pos != M_line_inputs.end());
	assert(next_pos != M_line_inputs.end());
	assert(!i_pos->output_status());
	assert(!next_pos->output_status());
	
	
	j_size_t return_cursor_pos = get_cursor_pos() >= i_pos->end_pos() 
		? (get_cursor_pos() - DEFAULT_OUTPUT_STRING_SIZE) : get_cursor_pos() ;

	J_Text_Box::erase_chars(next_pos->start_pos(), next_pos->size());
	
	J_Text_Box::insert_string(i_pos->output_start_pos(), next_pos->input_str());
	i_pos->set_input_str(i_pos->input_str() + next_pos->input_str());
	assert(safe_int_cast(DEFAULT_OUTPUT_STRING_SIZE)
		== (next_pos->end_pos() - next_pos->output_start_pos()));
	for_each(next_pos+1, M_line_inputs.end()
	, bind(&Line_Input::decrement_pos, _1, DEFAULT_OUTPUT_STRING_SIZE));

	J_Text_Box::set_cursor_pos(return_cursor_pos);
	return --M_line_inputs.erase(next_pos);
}



/*bool insert_char(J_UI_Char)*/
bool Math_Input_Box::insert_char(J_UI_Char i_char){
	auto line_input_pos = get_line_pos_at_pos(get_cursor_pos());
	if(M_line_inputs.front().output_str().size() == 2){
		M_line_inputs.begin();
	}
	j_size_t pos = get_cursor_pos() - line_input_pos->start_pos();
	
	if(line_input_pos->read_only_status(pos)){
		return false;
	}

	if(line_input_pos->output_status()){
		line_input_pos = clear_output(line_input_pos);

	}

	if(!line_input_pos->insert_char(get_cursor_pos() - line_input_pos->start_pos(), i_char)){
		return false;
	}

	for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::increment_pos, _1, 1));
	
	if(!J_Text_Box::insert_char(i_char)){
		assert("Could Not Insert Character into textbox from Math text box");
		throw J_Error<jc_string_t::value_type>(L"Could Not Insert Character into textbox from Math text box");
	}
	
	return true;
}

/*Line_Input_Shared_t::iterator lower_bound_line_input(int pos)*/
Math_Input_Box::Line_Input_Cont_t::iterator Math_Input_Box::lower_bound_line_input(j_size_t i_pos){

	return lower_bound(M_line_inputs.begin(), M_line_inputs.end(), i_pos, Line_Cursor_Comp());
}

Math_Input_Box::Line_Input_Cont_t::const_iterator Math_Input_Box::lower_bound_line_input(j_size_t i_pos)const{

	return lower_bound(M_line_inputs.begin(), M_line_inputs.end(), i_pos, Line_Cursor_Comp());
}

/*Line_Input_Shared_t get_line_at_pos(int pos)*/
Line_Input& Math_Input_Box::get_line_at_pos(j_size_t i_pos){
	assert(!M_line_inputs.empty());
	return *get_line_pos_at_pos(i_pos);
}

/*Line_Input_Shared_t::iterator lower_bound_line_input(int pos)*/
Math_Input_Box::Line_Input_Cont_t::iterator Math_Input_Box::get_line_pos_at_pos(j_size_t i_pos){

	auto line_input_pos = lower_bound_line_input(i_pos);

	if((line_input_pos == M_line_inputs.end()) 
		|| (line_input_pos->start_pos() != i_pos)){--line_input_pos;}

	return line_input_pos;
}

Math_Input_Box::Line_Input_Cont_t::const_iterator Math_Input_Box::get_line_pos_at_pos(j_size_t i_pos)const{

	auto line_input_pos = lower_bound_line_input(i_pos);

	if((line_input_pos == M_line_inputs.end())
	   || (line_input_pos->start_pos() != i_pos)){
		--line_input_pos;
	}

	return line_input_pos;
}

void Math_Input_Box::move_cursor(j_size_t i_amount){
	J_Text_Box::move_cursor(i_amount);
	j_size_t cursor_pos = get_cursor_pos();
	if(!is_read_only_status(cursor_pos)){
		return;
	}
	auto line_input_pos = get_line_pos_at_pos(cursor_pos);

	if(i_amount < 0){
		set_cursor_pos(line_input_pos->input_end_pos());
	} else{
		set_cursor_pos(line_input_pos->end_pos());
	}
}

bool Math_Input_Box::is_read_only_status(j_size_t i_cursor_pos)const{
	auto line_input_pos = get_line_pos_at_pos(i_cursor_pos);
	return line_input_pos
		->read_only_status(i_cursor_pos - line_input_pos->start_pos());

}

void Math_Input_Box::mouse_button_press(int i_key, int i_modifiers, Pen_Pos_FL_t i_pen_pos){
	J_Text_Box::mouse_button_press(i_key, i_modifiers, i_pen_pos);


	move_cursor_to_input_end();
	return;
}

void Math_Input_Box::mouse_button_release(int i_key, int i_modifiers, Pen_Pos_FL_t i_pen_pos){
	J_Text_Box::mouse_button_release(i_key, i_modifiers, i_pen_pos);


	move_cursor_to_input_end();
	return;
}

void Math_Input_Box::move_cursor_to_input_end(){
	if(!is_read_only_status(get_cursor_pos())){
		return;
	}

	auto line_input_pos = get_line_pos_at_pos(get_cursor_pos());
	set_cursor_pos(line_input_pos->input_end_pos());
}

}


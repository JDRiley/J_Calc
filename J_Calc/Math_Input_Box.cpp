#include "Math_Input_Box.h"

#include "J_Calc_Error.h"
#include <iostream>
//Algorithms
#include <algorithm>
//
#include <functional>
using std::bind; using std::is_sorted; using std::less; using std::lower_bound; using std::mem_fn;
using namespace std::placeholders; using std::dynamic_pointer_cast;

//Utilities
#include <cassert>
#include <vector>

namespace jomike{


//Math Input Text Box Functions**********************************************************

extern const char LINE_END_SYMBOL = ';';
const char* DEFAULT_INPUT_STRING = "{}";


//Constructors--------------------------------------------------
Math_Input_Box::Math_Input_Box(const J_Rectangle& irk_rectangle
	, const J_UI_Multi_String& irk_string):J_Text_Box(irk_rectangle, irk_string + DEFAULT_INPUT_STRING)
	, M_line_inputs(1, Line_Input(0)){

	M_line_inputs.front().set_input_str(irk_string.std_str());
	set_key_input_command(math_key_input_cmd);
	set_char_input_command(math_box_char_input_cmd);
}



/*void insert_line_input(const Line_Input&)*/
void Math_Input_Box::insert_line_input(const Line_Input& irk_line_input){
	auto insert_pos = lower_bound(M_line_inputs.begin(), M_line_inputs.end()
		, irk_line_input.start_pos(), Line_Cursor_Comp());
	insert_pos = M_line_inputs.insert(insert_pos, irk_line_input);

	J_Text_Box::insert_string(irk_line_input.start_pos(), irk_line_input.input_str() + irk_line_input.output_str());

	for_each(insert_pos + 1, M_line_inputs.end()
		, bind(&Line_Input::increment_pos, _1, irk_line_input.size()));
	if(irk_line_input.output_status()){
		set_cursor_pos(irk_line_input.end_pos());
	}else{
		set_cursor_pos(irk_line_input.start_pos());
	}
}

void Math_Input_Box::math_key_input_cmd(J_Text_Box_Object_Shared_t i_text_box, int i_charcode
	, int i_scancode, int i_action, int i_modifiers){
	auto math_box = dynamic_pointer_cast<Math_Input_Box>(i_text_box);

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



	if(line_input_pos->is_inside_input(char_pos)){
		line_input_pos = clear_output(line_input_pos);
		//line_input = *line_input_pos;
	}else{
		for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::decrement_pos, _1, 1));
		J_Text_Box::backspace();
		return;
	}

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


	j_size_t char_pos = get_cursor_pos() - line_input_pos->start_pos();
	
	if(line_input_pos->read_only_status(char_pos) || read_only_status()){
		return;
	}

	if(line_input_pos->output_status() && (get_cursor_pos() == line_input_pos->output_start_pos())){
		return;
	}

	line_input_pos = clear_output(line_input_pos);

	if(line_input_pos->input_str().size() != char_pos){
		line_input_pos->delete_char(char_pos);
		for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::decrement_pos, _1, 1));
		J_Text_Box::delete_char();
	}
}

/*void char_input_cmd(j_window_t, j_ulint)*/
void Math_Input_Box::math_box_char_input_cmd(J_Text_Box_Object_Shared_t i_math_box
	, int i_charcode){
	auto math_box = dynamic_pointer_cast<Math_Input_Box>(i_math_box);

	auto line_input_pos = math_box->get_line_pos_at_pos(i_math_box->get_cursor_pos());
	if(line_input_pos
		->read_only_status(math_box->get_cursor_pos() - line_input_pos->start_pos())){
		return;
	}

	line_input_pos = math_box->clear_output(line_input_pos);
	switch(i_charcode){
	case LINE_END_SYMBOL:
		math_box->eval_and_break_line_input();
		break;
	default:
		math_box->insert_char(i_charcode);
		break;
	}
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
	line_input_pos->set_input_str(
		J_UI_String(line_input_pos->input_str().begin(), line_input_pos->input_str().begin() + pos));

	line_input_pos->evaluate_output();
	j_size_t start_pos_move_size = line_input_pos->input_str().size() - input_str_size 
		+ line_input_pos->output_str().size() - output_str_size;
	

	J_Text_Box::erase_chars(line_input_pos->start_pos(), input_str_size + output_str_size);
	
	J_Text_Box::insert_string(line_input_pos->start_pos(), line_input_pos->input_str() + line_input_pos->output_str());

	assert(get_cursor_pos() 
		== (line_input_pos->start_pos()	+ line_input_pos->input_str().size() + line_input_pos->output_str().size()));
	
	assert(get_cursor_pos() == (line_input_pos->end_pos()));

	for_each(line_input_pos + 1, M_line_inputs.end(), bind(&Line_Input::increment_pos, _1, start_pos_move_size));
	
	Line_Input new_input(get_cursor_pos());
	
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
	j_size_t return_cursor_pos = i_pos->input_end_pos() == get_cursor_pos() ?  get_cursor_pos() - 1 : get_cursor_pos();

	
	j_size_t deletetion_size = i_pos->output_str().size() + 1/*The "+1" is cause we added the Line_END_SYMBOL char for deletion*/;



	J_Text_Box::erase_chars(i_pos->output_start_pos() - 1 /*The "-1" is to delete the LINE_END_SYMBOL char*/
		, deletetion_size);

	i_pos->clear_output();
	i_pos->delete_char(i_pos->input_str().size() -1);

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
	
	
	j_size_t return_cursor_pos = get_cursor_pos() > i_pos->end_pos() 
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
		throw J_Error("Could Not Insert Character into textbox from Math text box");
	}
	
	return true;
}

/*Line_Input_Shared_t::iterator lower_bound_line_input(int pos)*/
Math_Input_Box::Line_Input_Cont_t::iterator Math_Input_Box::lower_bound_line_input(j_size_t i_pos){

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

}


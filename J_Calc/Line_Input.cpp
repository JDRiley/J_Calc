#include "Line_Input.h"
//
#include "J_Calc_Error.h"
//
#include <J_Calc_Parser.h>
//
#include <J_Utile.h>
//
#include <j_type.h>

#ifndef RELEASE_BUILD

#include <iostream>
using std::cerr;

#endif
namespace jomike{
extern const char* const GK_DEFAULT_OUTPUT_STRING = "{}";
extern const j_size_t DEFAULT_OUTPUT_STRING_SIZE 
	= static_cast<j_size_t>(strlen(GK_DEFAULT_OUTPUT_STRING));

J_FWD_DECL(Line_Input)

static Instance_Pointer<J_Calc_Math_Input_Parser> gs_parser;

const char LINE_END_SYMBOL = ' ';


void Line_Input::set_start_pos(j_size_t i_pos){M_start_pos = i_pos;}

/*int size()const*/
j_size_t Line_Input::size()const{return M_input.size() + M_output.size();}

//Constructors
Line_Input::Line_Input(
	j_size_t i_start_pos, const J_UI_Multi_String& irk_input
	, J_Font_Face i_font_face, const J_UI_Color& irk_color)
	:M_start_pos(i_start_pos), M_input(irk_input), M_output(J_UI_String(GK_DEFAULT_OUTPUT_STRING,  i_font_face, irk_color)){}

/*int start_pos()const*/
j_size_t Line_Input::start_pos()const{return M_start_pos;}

/*int input_end_pos()const*/
j_size_t Line_Input::input_end_pos()const{return M_start_pos + M_input.size();}

/*int input_end_pos()const*/
j_size_t Line_Input::end_pos()const{
	assert(input_end_pos() == output_start_pos()); return input_end_pos() + M_output.size();
}

/*void insert_char(int pos, j_ulint charcode, int font_index)*/
bool Line_Input::insert_char(j_size_t i_pos, J_UI_Char i_ui_char){
	if(read_only_status(i_pos)){
		return false;
	}
	M_input.insert(i_pos, i_ui_char);

#ifndef RELEASE_BUILD
	//cerr << "\nAt pos: " << i_pos << " After insert: " << M_input;
#endif
	return true;
}

/*void delete_char_ex(int pos, j_ulint charcode, int font_index)*/
void Line_Input::delete_char(j_size_t i_pos){
	if(read_only_status(i_pos)){
		throw J_Syntax_Error("Cannot input char in this location");
	}
	M_input.erase(i_pos);
}
/*bool output_status()const*/
bool Line_Input::output_status()const{assert(M_output.size() > 1); return !(M_output.size() == 2);}

/*void input_str(const J_UI_String&, const ex_array<int>&)*/
void Line_Input::set_input_str(const J_UI_Multi_String& irk_string){
	M_input	= irk_string;
}

/*void clear_output()*/
void Line_Input::clear_output(){M_output = J_UI_String(GK_DEFAULT_OUTPUT_STRING, M_output.front().font_face(), M_output.front().color());}
/*int output_start_pos()const;*/
j_size_t Line_Input::output_start_pos()const{return M_start_pos + M_input.size();}

/*const J_UI_String& output_str()const*/
const J_UI_Multi_String& Line_Input::output_str()const{return M_output;}

/*bool read_only_status(int)*/
bool Line_Input::read_only_status(j_size_t i_pos)const{

	return between_inclusive(i_pos, M_input.size() + 1
		, M_input.size() + M_output.size()); 
}

/*bool is_inside_input(int i_pos)*/
bool Line_Input::is_inside_input(j_size_t i_pos){
	return between_inclusive(i_pos, 0ll, M_input.size() - 1);
}

/*const J_UI_String& input_str()const*/
const J_UI_Multi_String& Line_Input::input_str()const{return M_input;}

/*void evaluate_output()*/
void Line_Input::evaluate_output(){



#if _DEBUG	
	cerr << "\nInput_Str:" << M_input;
#endif //_DEBUGS
	J_UI_Multi_String input_string(M_input);
//	gs_parser->convert_to_proper_math_input(&input_string);

	if(M_input.empty()){
		return;
	}

	M_input.push_back(LINE_END_SYMBOL);

	input_string.push_back(LINE_END_SYMBOL);

	
	J_UI_String output_string(evaluate_math_input(input_string));
	output_string.set_font_face(M_output.front().font_face());
	M_output = output_string;
	M_output.push_front(GK_DEFAULT_OUTPUT_STRING[0]);
	M_output.push_back(GK_DEFAULT_OUTPUT_STRING[1]);
	M_output.set_color(J_WHITE);
}

/*void advance_whitespace()*/
//void Line_Input::advance_whitespace(){
//	j_size_t old_string_size = M_input.size();
//
//	auto string_pos = M_input.begin();
//	while((string_pos != M_input.end()) && string_pos->is_space()){++string_pos;}
//
//	M_input = M_input.substr(string_pos - M_input.begin());
//
//	set_start_pos(start_pos() + (old_string_size - M_input.size()));
//}

/*void increment_pos(int)*/
void Line_Input::increment_pos(j_size_t i_inc_size){
	M_start_pos += i_inc_size;
}

/*void decrement_pos(int)*/
void Line_Input::decrement_pos(j_size_t i_dec_size){
	M_start_pos -= i_dec_size;
}

jomike::Line_Input Line_Input::make_empty_from(j_size_t i_pos)const{
	return Line_Input(i_pos, M_input.front()
					  , M_output.front().font_face(), M_output.front().color());
}


bool Line_Cursor_Comp::operator()(const Line_Input& i_left, int i_pos)const{
	return i_left.start_pos() < i_pos;
}
bool Line_Cursor_Comp::operator()(int i_pos, const Line_Input& i_right)const{
	return i_pos < i_right.start_pos();
}

	
}
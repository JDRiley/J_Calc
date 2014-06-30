#ifndef LINE_INPUT_H
#define LINE_INPUT_H

#include <J_UI/J_UI_Multi_String.h>

namespace jomike{

extern const char* const GK_DEFAULT_OUTPUT_STRING;
extern const j_size_t DEFAULT_OUTPUT_STRING_SIZE;
extern const J_UI_Color G_DEFAULT_OUTPUT_COLOR;
class Line_Input{
public:
	//Constructors
	Line_Input(
		j_size_t, const J_UI_Multi_String&,  J_Font_Face i_output_font_face
		, const J_UI_Color& irk_output_color = G_DEFAULT_OUTPUT_COLOR);

	Line_Input make_empty_from(j_size_t i_pos)const;

	//Accessors
	j_size_t start_pos()const;
	j_size_t end_pos()const;

	j_size_t input_end_pos()const;
	j_size_t output_start_pos()const;
	
	j_size_t size()const;
	const J_UI_Multi_String& input_str()const;

	void set_input_str(const J_UI_Multi_String&);

	const J_UI_Multi_String& output_str()const;

	bool output_status()const;

	//Modifiers
	void clear_output();
	
	void set_start_pos(j_size_t);
	void increment_pos(j_size_t i_size = 1);
	void decrement_pos(j_size_t i_size = 1);
	/*void insert_char(int pos, j_ulint charcode, int font_index)*/
	/*
		Throws J_Syntax_Error if a problem occurs in insertion
	*/
	bool insert_char(j_size_t pos, J_UI_Char);
	void delete_char(j_size_t pos);
	//Actions
	void evaluate_output();

	//void advance_whitespace();

	//Test
	bool read_only_status(j_size_t pos);
	bool is_inside_input(j_size_t pos);
private:
	j_size_t M_start_pos;
	J_UI_Multi_String M_input;
	J_UI_Multi_String M_output;
};


class Line_Cursor_Comp{
public:
	bool operator()(const Line_Input& i_left, int i_pos)const;
	bool operator()(int i_pos, const Line_Input& i_right)const;
};


}

#endif
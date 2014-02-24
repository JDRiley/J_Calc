#ifndef LETTER_BOX_STRING_H
#define LETTER_BOX_STRING_H

#include "../J_UI_Fwd_Decl.h"
//
#include <ex_array.h>

namespace jomike{

class Letter_Box_String{
public:
	typedef ex_array<J_Display_Letter_Box_Shared_t>::const_iterator const_iterator;

	void push_back(J_Display_Letter_Box_Shared_t i_letter);
	void draw(j_size_t i_start_pos, j_size_t i_end_pos)const;
	J_Display_Letter_Box_Shared_t operator[](j_size_t i_index);
	const_iterator begin()const;
	const_iterator end()const;

	void resize(j_size_t i_size);
private:
	ex_array<J_Display_Letter_Box_Shared_t> M_letter_boxes;
};


class Letter_Box_Pos_Order_Comp{
public:
	Letter_Box_Pos_Order_Comp(j_float i_reference_line, j_float i_line_size);
	bool operator()(J_Display_Letter_Box_Shared_t i_left, const Pen_Pos_FL_t& i_right)const;
	bool operator()(const Pen_Pos_FL_t& i_left, J_Display_Letter_Box_Shared_t i_right)const;
private:
	j_float M_reference_line;
	j_float M_line_size;
	int get_line_index(j_float i_float)const;
};

}

#endif //LETTER_BOX_STRING_H


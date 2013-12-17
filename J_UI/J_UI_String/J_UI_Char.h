#ifndef J_UI_CHAR_H
#define J_UI_CHAR_H

#include "../J_UI_Fwd_Decl.h"

namespace jomike{

typedef j_ulint J_Char_t;

class J_UI_Char{
public:
	J_UI_Char();
	J_UI_Char(J_Char_t);

	J_Char_t charcode()const;
	//int color_id()const;
	//int font_id()const;

	void set_charcode(J_Char_t);
	//void set_color_id(int);
	//void set_font_id(int);

	bool is_number()const;
	bool is_alpha()const;
	bool is_space()const;
	bool is_alpha_numeric()const;

	void normalize_white_space();

	bool operator==(const J_UI_Char&)const;
	bool operator!=(const J_UI_Char&)const;
	bool operator<(const J_UI_Char&)const;
private:
	J_Char_t M_charcode;
};

}

#endif
#include "J_UI_Char.h"

#include <cctype>
namespace jomike{

//J_UI_Char Functions*********************************************

//Constructors

J_UI_Char::J_UI_Char():M_charcode(0){}

J_UI_Char::J_UI_Char(J_Char_t i_charcode):M_charcode(i_charcode){}




/*J_Char_t charcode()const*/
J_Char_t J_UI_Char::charcode()const{return M_charcode;}



/*void set_charcode(J_Char_t)*/
void J_UI_Char::set_charcode(J_Char_t i_charcode){M_charcode = i_charcode;}

/*bool is_number()const*/
bool J_UI_Char::is_number()const{return isdigit(static_cast<int>(M_charcode));}

/*bool is_alpha()const*/
bool J_UI_Char::is_alpha()const{return isalpha(static_cast<int>(M_charcode));}

/*bool is_alpha_numberic()const*/
bool J_UI_Char::is_alpha_numeric()const{return isalpha(static_cast<int>(M_charcode));}

/*bool is_space()const*/
bool J_UI_Char::is_space()const{return isspace(static_cast<int>(M_charcode));}

/*bool operator<(const J_UI_Char&)const*/
bool J_UI_Char::operator<(const J_UI_Char& i_char)const{
	return M_charcode < i_char.M_charcode;
}

/*bool operator<(const J_UI_Char&)const*/
bool J_UI_Char::operator==(const J_UI_Char& i_char)const{
	return M_charcode == i_char.M_charcode;
}

bool J_UI_Char::operator!=(const J_UI_Char& i_char)const{
	return M_charcode != i_char.M_charcode;
}



}
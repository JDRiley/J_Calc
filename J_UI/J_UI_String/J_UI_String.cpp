#include "J_UI_String.h"
#include <J_String.h>
#include <j_type.h>
#include "../J_Font_Manager.h"
//
#include "../J_Font_Face.h"

//Algorithm
#include <algorithm>
#include <numeric>
using std::copy; using std::bind; using std::max;using std::mem_fn; 
using std::accumulate; using std::transform; using std::make_pair;
using namespace std::placeholders;

//Container
using std::string; 

using std::max_element;

//IO Facilities
#include <iostream>
using std::ostream;

//Iterator
#include <iterator>

using std::back_inserter;
using std::swap; using std::pair;

using std::min;

//Utilities
using std::pair; using std::move;

namespace jomike{
const int INIT_DEFAULT_COLOR_ID = 0;
const int INIT_DEFAULT_FONT_ID = 0;

static Instance_Pointer<J_Font_Manager> s_font_manager;
const j_dbl J_UI_String::S_DEFAULT_FRONT_BUFFER_RATIO = 0.1;
const J_Color_RGBA<j_float> DEFAULT_FONT_COLOR = J_Color::Black;
const int DEFAULT_FONT_SIZE = 12;

//Constructors

/*
J_UI_String::J_UI_String()
	:M_UI_Chars(1,J_UI_Char(0)), M_color(DEFAULT_FONT_COLOR),M_font_face(nullptr){

#if _DEBUG
		M_string = std_str();
#endif
}
*/
J_UI_String::J_UI_String(J_Font_Face i_font_face, J_UI_Color i_color)
	:M_font_face(i_font_face), M_color(i_color){
	M_UI_Chars.push_back(0);
	M_UI_Chars.set_front_buffer(S_DEFAULT_FRONT_BUFFER_RATIO);
}

J_UI_String::J_UI_String(const char* const ik_string, J_Font_Face i_font_face
						 , J_UI_Color i_color){
	*this = J_UI_String(string(ik_string), i_font_face, i_color);
#if _DEBUG
		M_string = std_str();
#endif
}

J_UI_String::J_UI_String(const wchar_t* const ik_string, J_Font_Face i_font_face
						 , J_UI_Color i_color): M_font_face(i_font_face), M_color(i_color){
	std::wstring str(ik_string);
	*this = J_UI_String(str.begin(), str.end());
	M_font_face = i_font_face;
	M_color = i_color;
#if _DEBUG
		M_string = std_str();
#endif
}

J_UI_String::J_UI_String(const string& irk_source, J_Font_Face i_font_face
						 , J_UI_Color i_color){
	*this = to_display_string(irk_source);
	M_font_face = i_font_face;
	M_color = i_color;
#if _DEBUG
		M_string = std_str();
#endif
}

void J_UI_String::swap(J_UI_String& ir_source){
	std::swap(M_color, ir_source.M_color);
	std::swap(M_font_face, ir_source.M_font_face);
	M_UI_Chars.swap(ir_source.M_UI_Chars);
#if _DEBUG
	M_string.swap(ir_source.M_string);
#endif
}

J_UI_String::J_UI_String(J_UI_String&& irr_source){swap(irr_source);}

J_UI_String& J_UI_String::operator=(J_UI_String&& irr_right){swap(irr_right); return *this;}


//J_UI_String::J_UI_String(const LU_String& irk_string): M_font_face(nullptr), M_color(DEFAULT_FONT_COLOR){
//	auto push_back_iterator = back_inserter(M_UI_Chars);
//	copy(irk_string.begin(), irk_string.end(), push_back_iterator);
//	M_UI_Chars.push_back('\0');
//	M_UI_Chars.set_front_buffer(S_DEFAULT_FRONT_BUFFER_RATIO);
//#if _DEBUG
//		M_string = std_str();
//#endif
//}

/*void insert(int i_pos, J_UI_Char i_ui_char)*/
void J_UI_String::insert(j_size_t i_pos, J_UI_Char i_ui_char){
	assert(i_pos >= 0);
	assert(i_pos < M_UI_Chars.size());
	M_UI_Chars.insert(M_UI_Chars.begin() + i_pos, i_ui_char);
#if _DEBUG
		M_string.insert(i_pos, 1, safe_char_cast(i_ui_char.charcode()));
#endif
}

/*void insert(iterator i_pos, J_UI_Char i_ui_char)*/
void J_UI_String::insert(iterator i_pos, J_UI_Char i_ui_char){
#if _DEBUG
	ptrdiff_t index = i_pos - begin();
#endif

	assert((i_pos - M_UI_Chars.begin()) < M_UI_Chars.size());
	assert(i_pos >= M_UI_Chars.begin());
	M_UI_Chars.insert(i_pos, i_ui_char);
#if _DEBUG
		M_string.insert(M_string.begin() + index, safe_char_cast(i_ui_char.charcode()));
#endif
}

/*void insert(int pos, const J_UI_String&)*/
void J_UI_String::insert(j_size_t i_pos, const J_UI_String& irk_string){
	M_UI_Chars.insert(M_UI_Chars.begin() + i_pos, irk_string.begin(), irk_string.end());
#if _DEBUG
		M_string.insert(i_pos, irk_string.std_str());
#endif
}

/*void erase(int pos)*/
void J_UI_String::erase(j_size_t i_pos){
	M_UI_Chars.erase(begin() + i_pos);
#if _DEBUG
	M_string.erase(i_pos, 1);
#endif
}
void J_UI_String::erase(j_size_t i_pos, j_size_t i_size){
	M_UI_Chars.erase(begin() + i_pos, i_size);
#if _DEBUG
	M_string.erase(i_pos, i_size);
#endif
}
void J_UI_String::erase(const_iterator i_pos, const_iterator i_end_pos){


#if _DEBUG
	assert(M_string.size() == safe_uns_cast(size()));
	ptrdiff_t index = i_pos - begin();
	ptrdiff_t end_index = i_end_pos - begin();
#endif
	M_UI_Chars.erase(i_pos, i_end_pos);
#if _DEBUG
		M_string.erase(M_string.begin() + index, M_string.begin() + end_index);
#endif
}

void J_UI_String::erase(const_iterator i_pos){
	erase(i_pos, i_pos + 1);
}

/*void clear()*/
void J_UI_String::clear(){
	M_UI_Chars.clear();
	M_UI_Chars.push_back('\0');
#if _DEBUG
		M_string.clear();
#endif
}

/*bool empty()const;*/
bool J_UI_String::empty()const{return M_UI_Chars.size() == 1;}

/*int size()const*/
j_size_t J_UI_String::size()const{return M_UI_Chars.size()-1;}



/*J_UI_String substr(int pos, int end = j_npos)const*/
J_UI_String J_UI_String::substr(j_size_t i_pos, j_size_t i_end)const{
	J_UI_String new_string(begin() + i_pos, j_npos == i_end ? end() : begin() + i_end);
	new_string.M_font_face = M_font_face;
	new_string.M_color = M_color;
	return move(new_string);
}

//J_UI_String Iterators*****************************************************
J_UI_String::char_iterator J_UI_String::char_begin(){
	return char_iterator(&*begin(), mem_fn(&J_UI_Char::charcode));
}

J_UI_String::char_iterator J_UI_String::char_end(){
	return char_iterator(&*end(), mem_fn(&J_UI_Char::charcode));
}

J_UI_String::char_const_iterator J_UI_String::char_begin()const{
	return char_const_iterator(&*begin(), mem_fn(&J_UI_Char::charcode));
}

J_UI_String::char_const_iterator J_UI_String::char_end()const{
	return char_const_iterator(&*end(), mem_fn(&J_UI_Char::charcode));
}

J_UI_String::iterator J_UI_String::begin(){return M_UI_Chars.begin();}
J_UI_String::iterator J_UI_String::end(){return M_UI_Chars.end()-1;}

J_UI_String::const_iterator J_UI_String::begin()const{return M_UI_Chars.begin();}
J_UI_String::const_iterator J_UI_String::end()const{return M_UI_Chars.end()-1;}

J_UI_String::char_const_iterator J_UI_String::const_char_begin()const{
	return char_const_iterator(&*begin(), mem_fn(&J_UI_Char::charcode));
}

J_UI_String::char_const_iterator J_UI_String::const_char_end()const{
	return char_const_iterator(&*end(), mem_fn(&J_UI_Char::charcode));
}

J_UI_String::const_iterator J_UI_String::cbegin()const{return M_UI_Chars.cbegin();}
J_UI_String::const_iterator J_UI_String::cend()const{return M_UI_Chars.cend()-1;}


const J_UI_Color& J_UI_String::color()const{return M_color;}

bool J_UI_String::is_same_type(const J_UI_String& irk_string)const{
	return (font_face() == irk_string.font_face())
		&& (color() == irk_string.color());
}


void white_space_normalize(J_UI_String* ir_string){
	if(ir_string->empty()){
		return;
	}

	bool copy_whitespace = false;
	J_UI_String normalized_str(ir_string->font_face(), ir_string->color());


	for_each(ir_string->begin(), ir_string->end()
		,[&normalized_str, &copy_whitespace](const J_UI_Char ik_char){

			if(!ik_char.is_space()){
				copy_whitespace = true;
				normalized_str.push_back(ik_char);
			}else if(copy_whitespace){
				copy_whitespace = false;	
				normalized_str.push_back(' ');
			}
		});

	if(normalized_str.back().is_space()){
		normalized_str.pop_back();
	}
	
	normalized_str.swap(*ir_string);
}
J_UI_String operator+(const char* ik_chars, const J_UI_String& irk_string){
	return J_UI_String(ik_chars) + irk_string;

}

/*LU_String lu_str()const*/
LU_String J_UI_String::lu_str()const{return LU_String(const_char_begin(), const_char_end());}

/*string std_str()const*/
string J_UI_String::std_str()const{
	string new_string;
	new_string.resize(size());
	transform(const_char_begin(), const_char_end(), new_string.begin(), bind(safe_char_cast<J_Char_t>, _1));
	return new_string;
}

/*J_UI_String operator+(const J_UI_String&)const*/
J_UI_String J_UI_String::operator+(const J_UI_String& irk_string)const{
	J_UI_String new_string(*this);
	return new_string.append(irk_string);
}

/*bool operator<(const J_UI_String&)const*/
bool J_UI_String::operator<(const J_UI_String& irk_string)const{
	return (M_UI_Chars < irk_string.M_UI_Chars);
}

ostream& operator<<(ostream& ir_stream, const jtl::J_UI_String& irk_string){
	ir_stream << irk_string.std_str();
	return ir_stream;
}

/*J_UI_Char& operator[](j_size_t)*/
J_UI_Char& J_UI_String::operator[](j_size_t i_index){return M_UI_Chars[i_index];}

/*J_UI_Char& operator[](j_size_t)const*/
J_UI_Char J_UI_String::operator[](j_size_t i_index)const{return M_UI_Chars[i_index];}


/*void push_back(const J_UI_Char&)*/
void J_UI_String::push_back(const J_UI_Char& i_char){
	M_UI_Chars.insert(end(), i_char);
#if _DEBUG
		M_string.push_back(safe_char_cast(i_char.charcode()));
#endif
}

/*void pop_back()*/
void J_UI_String::pop_back(){
	M_UI_Chars.erase(end());
#if _DEBUG
		M_string.pop_back();
#endif
}

/*void push_front(const J_UI_Char&)*/
void J_UI_String::push_front(const J_UI_Char& i_char){
	M_UI_Chars.push_front(i_char);
#if _DEBUG
		M_string.insert(M_string.begin(), safe_char_cast(i_char.charcode()));
#endif
}

/*void pop_front()*/
void J_UI_String::pop_front(){
	M_UI_Chars.pop_front();
#if _DEBUG
		M_string.erase(M_string.begin());
#endif
}

/*J_UI_String& append(const J_UI_String&)*/
J_UI_String& J_UI_String::append(const J_UI_String& irk_string){
	assert(is_same_type(irk_string));
	M_UI_Chars.insert(end(),irk_string.begin(), irk_string.end());
#if _DEBUG
		M_string.append(irk_string.M_string);
#endif
	return *this;
}


/*static char_iterator to_char_iterator(iterator)*/
J_UI_String::char_iterator J_UI_String::to_char_iterator(iterator i_pos){
	return char_iterator(&*i_pos, mem_fn(&J_UI_Char::charcode));
}

/*static char_const_iterator to_char_iterator(iterator)*/
J_UI_String::char_const_iterator J_UI_String::to_char_iterator(const_iterator i_pos){
	return char_const_iterator(&*i_pos, mem_fn(&J_UI_Char::charcode));
}

/*static char_const_iterator to_char_const_iterator(const_iterator)*/
J_UI_String::char_const_iterator J_UI_String::to_char_const_iterator(const_iterator i_pos){
	return char_const_iterator(&*i_pos, mem_fn(&J_UI_Char::charcode));
}

/*void resize(j_size_t)*/
void J_UI_String::resize(j_size_t i_size){
	M_UI_Chars.resize(i_size+1); M_UI_Chars.back() = '\0';
#if _DEBUG
		M_string.resize(i_size);
#endif
}

/*J_UI_Char& front()*/
J_UI_Char& J_UI_String::front(){return M_UI_Chars.front();}

/*J_UI_Char front()const*/
const J_UI_Char& J_UI_String::front()const{return M_UI_Chars.front();}



/*J_UI_Char& back()*/
J_UI_Char& J_UI_String::back(){return *(end()-1);}

/*J_UI_Char back()const*/
const J_UI_Char& J_UI_String::back()const{return *(end()-1);}


const J_Font_Face J_UI_String::font_face()const{
	return M_font_face;
}

void J_UI_String::set_font_face(J_Font_Face i_font_face){M_font_face = i_font_face;}

void J_UI_String::set_color(J_UI_Color i_color){
	M_color = i_color;
}

bool J_UI_String::operator==(const J_UI_String& irk_other)const{
	if(!is_same_type(irk_other)){
		return false;
	}
	return M_UI_Chars == irk_other.M_UI_Chars;
}

bool J_UI_String::operator!=(const J_UI_String& irk_other)const{
	return !(*this == irk_other);
}

std::string J_UI_String::to_std_string()const{
	std::string new_string = std_str();
	assert(new_string == M_string);
	return new_string;
}


}// namespace jomike


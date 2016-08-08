#ifndef J_UI_STRING_H
#define J_UI_STRING_H

//
#include "../J_Image/J_Image.h"
//
#include"J_UI_Char.h"
//
#include "../J_UI_Fwd_Decl.h"
//
#include <J_Fwd_Decl.h>
//
#include <j_limits.h>
//
#include <iosfwd>

//Algorithm
#include <functional>

//Containers
#include <ex_array.h>


//Iterator
#include <j_iterator.h>


#define J_UI_STRING_DEBUG
namespace jomike{


EX_ARRAY_TYPEDEF_UPPER(J_UI_String)
EX_ARRAY_TYPEDEF_UPPER(J_UI_Char)

typedef std::basic_string<j_ulint> LU_String;



class J_UI_String{
public:
	typedef J_UI_Char value_type;


	//Constructors 
	J_UI_String(J_Font_Face font_face 
		, J_UI_Color i_color = J_Color::Black);
	J_UI_String(const char* const, J_Font_Face font_face = nullptr
		, J_UI_Color i_color = J_Color::Black);
	J_UI_String(const wchar_t* const, J_Font_Face font_face = nullptr
		, J_UI_Color i_color = J_Color::Black);

	J_UI_String(const std::string&, J_Font_Face font_face = nullptr
		, J_UI_Color i_color = J_Color::Black);

	
	J_UI_String(J_UI_String&&);
	J_UI_String(const J_UI_String&) = default;


	template<typename Iter>
	J_UI_String(Iter, Iter);

	J_UI_String& operator=(J_UI_String&&);
	J_UI_String& operator=(const J_UI_String&) = default;

	void swap(J_UI_String&);


	void resize(j_size_t);
	J_UI_String substr(j_size_t pos, j_size_t end = j_npos)const;

	typedef J_UI_Char_Arr_t::iterator iterator;
	typedef J_UI_Char_Arr_t::const_iterator const_iterator;

	typedef value_ptr_iterator<J_Char_t, iterator::pointer, J_UI_String, decltype(std::mem_fn(&J_UI_Char::charcode))> char_iterator;
	//typedef value_ptr_iterator<int, iterator, J_UI_Char, J_UI_String, decltype(std::mem_fn(&J_UI_Char::font_id))> font_id_iterator;
	//typedef value_ptr_iterator<int, iterator, J_UI_Char, J_UI_String, decltype(std::mem_fn(&J_UI_Char::color_id))> color_id_iterator;

	typedef value_ptr_iterator<J_Char_t, const_iterator::pointer, J_UI_String, decltype(std::mem_fn(&J_UI_Char::charcode))> 
		char_const_iterator;
	//typedef value_ptr_iterator<int, const_iterator, J_UI_Char, J_UI_String, decltype(std::mem_fn(&J_UI_Char::font_id))> 
	//	font_id_const_iterator;
	//typedef value_ptr_iterator<int, const_iterator, J_UI_Char, J_UI_String, decltype(std::mem_fn(&J_UI_Char::color_id))> 
	//	color_id_const_iterator;


	//Accessors
	J_UI_Char& operator[](j_size_t);
	J_UI_Char operator[](j_size_t)const;

	J_UI_Char& front();
	const J_UI_Char& front()const;
	
	J_UI_Char& back();
	const J_UI_Char& back()const;



	void push_back(const J_UI_Char&);
	void pop_back();

	void push_front(const J_UI_Char&);
	void pop_front();

	LU_String lu_str()const;
	std::string std_str()const;

	J_UI_String operator+(const J_UI_String&)const;
	
	//Status
	bool empty()const;
	j_size_t size()const;

	//Character Insertions/Deletion
	void insert(j_size_t pos, J_UI_Char);
	void insert(iterator, J_UI_Char);
	void insert(j_size_t pos, const J_UI_String&);


	template<typename Iter>
	void insert(const_iterator, Iter i_first, Iter i_last);

	void erase(j_size_t pos);
	void erase(j_size_t pos, j_size_t size);
	void erase(const_iterator, const_iterator);
	void erase(const_iterator i_pos);
	void clear();

	J_UI_String& append(const J_UI_String&);

	const J_UI_Color& color()const;
	void set_color(J_UI_Color);
	const J_Font_Face font_face()const;
	void set_font_face(J_Font_Face);

	bool is_same_type(const J_UI_String&)const;

	//Iterators
	iterator begin();
	iterator end();

	static char_iterator to_char_iterator(iterator);
	static char_const_iterator to_char_iterator(const_iterator);
	static char_const_iterator to_char_const_iterator(const_iterator);
	
	
	J_UI_String& operator+=(const char*);

	const_iterator begin()const;
	const_iterator end()const;
	const_iterator cbegin()const;
	const_iterator cend()const;

	char_iterator char_begin();
	char_iterator char_end();

	char_const_iterator char_begin()const;
	char_const_iterator char_end()const;

	char_const_iterator const_char_begin()const;
	char_const_iterator const_char_end()const;

	bool operator<(const J_UI_String&)const;

	void white_space_normalize();

	ex_array<Bitmap_Metrics*> metrics_array()const;
	ex_array<const j_ubyte*> bitmap_data_array()const;

	bool operator==(const J_UI_String& irk_other)const;

	bool operator!=(const J_UI_String& irk_other)const;
private:
	const static j_dbl S_DEFAULT_FRONT_BUFFER_RATIO;
	J_UI_Char_Arr_t M_UI_Chars;
	J_Font_Face M_font_face;
	J_Color_RGBA<j_float> M_color;

#if _DEBUG
	std::string M_string;
#endif
#ifdef J_UI_STRING_DEBUG
	
	friend class J_Test_Suite;
	void test_ui_string();
	std::string to_std_string()const;
#endif
};

J_UI_String operator+(const char* ik_chars, const J_UI_String& irk_string);


template<typename Iter>
void J_UI_String::insert(const_iterator i_pos, Iter i_first, Iter i_last){
#if _DEBUG
	ptrdiff_t index = i_pos - begin();
#endif

	M_UI_Chars.insert(i_pos, i_first, i_last);
#if _DEBUG
	J_UI_String std_string_helper(i_first, i_last);
	M_string.insert(M_string.begin() + index, std_string_helper.const_char_begin()
		, std_string_helper.const_char_end());
#endif
}

/*template<typename Iter>*/
/*J_UI_String(Iter, Iter)*/
template<typename Iter>
J_UI_String::J_UI_String(Iter i_start_pos, Iter i_end_pos):M_UI_Chars(i_start_pos, i_end_pos){
	M_UI_Chars.push_back('\0');
	M_UI_Chars.set_front_buffer(S_DEFAULT_FRONT_BUFFER_RATIO);
#if _DEBUG
		M_string = std_str();
#endif
}

typedef J_UI_String::const_iterator J_UI_Const_Iter;

j_dbl read_double_and_advance(J_UI_String::const_iterator* i_string_pos, ptrdiff_t i_max_len);
void white_space_normalize(J_UI_String* irk_string);


std::ostream& operator<<(std::ostream&, const jtl::J_UI_String&);




J_UI_String to_display_string(const std::string& irk_string);

template<typename Iter>
void advance_white_space(Iter* irk_pos, Iter i_end_pos){
	while((*irk_pos != i_end_pos) && (*irk_pos)->is_space()){ ++*irk_pos; }
}

}



#endif
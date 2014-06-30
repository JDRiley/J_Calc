#ifndef J_UI_MULTI_STRING_H
#define J_UI_MULTI_STRING_H

//
#include "J_UI/J_UI_String.h"

namespace jomike{


class J_UI_Multi_String{
public:
	typedef J_UI_String value_type;
	typedef ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	J_UI_Multi_String();
	J_UI_Multi_String(const char* const);
	J_UI_Multi_String(const wchar_t* const);
	J_UI_Multi_String(const LU_String&);
	J_UI_Multi_String(const std::string&);
	J_UI_Multi_String(const J_UI_String&);

	J_UI_Multi_String(J_UI_Multi_String&&);

	template<typename Iter>
	J_UI_Multi_String(Iter, Iter);

	J_UI_Multi_String& operator=(J_UI_Multi_String&&);

	void swap(J_UI_Multi_String&);
	const Bitmap_Metrics& bitmap_metric(j_size_t i_index)const;
	int new_line_size()const;
	void resize(j_size_t);
	J_UI_Multi_String substr(j_size_t pos, j_size_t end = j_npos)const;

	template<typename Value_Type>
	class multi_string_iterator{
	public:
		friend J_UI_Multi_String;
		typedef typename std::random_access_iterator_tag iterator_category;
		typedef typename Value_Type  				value_type;
		typedef typename ptrdiff_t				 	difference_type;
		typedef typename Value_Type&			 	reference;
		typedef typename Value_Type*			   	pointer;

		multi_string_iterator(const multi_string_iterator
								<typename std::remove_const<typename Value_Type>::type>&);

		multi_string_iterator(j_size_t i_pos, const J_UI_Multi_String& irk_cont);

		reference operator*()const;
		pointer operator->()const;

		multi_string_iterator& operator++(){ ++M_pos; return *this; }
		multi_string_iterator operator++(int){ return multi_string_iterator(M_pos++, M_cont); }

		multi_string_iterator operator--(){ --M_pos; return *this; }
		multi_string_iterator operator--(int){ return multi_string_iterator(M_pos--, M_cont); }

		reference operator[](const difference_type& _ik_iX)const{ return *(M_pos + _ik_iX); }

		multi_string_iterator& operator+=(const difference_type& _ik_iX){ M_pos += _ik_iX; return *this; }

		multi_string_iterator operator+(const difference_type& _irk_iX){ return multi_string_iterator(M_pos + _irk_iX, M_cont); }

		multi_string_iterator& operator-=(const difference_type& _irk_iX){ M_pos -= _irk_iX; return *this; }

		multi_string_iterator operator-(const difference_type& _irk_iX){ return multi_string_iterator(M_pos - _irk_iX, M_cont); }

		const j_size_t pos()const{ return M_pos; }
	private:
		j_size_t M_pos;
		const J_UI_Multi_String& M_cont;
	};

	typedef multi_string_iterator<J_UI_Char> j_ui_char_iterator;
	typedef multi_string_iterator<const J_UI_Char> j_ui_char_const_iterator;

	typedef value_ptr_iterator<J_Char_t, j_ui_char_iterator
		, J_UI_Multi_String, decltype(std::mem_fn(&J_UI_Char::charcode))> char_iterator;

	typedef value_ptr_iterator<J_Char_t, j_ui_char_const_iterator, J_UI_Multi_String
		, decltype(std::mem_fn(&J_UI_Char::charcode))> char_const_iterator;

	typedef J_UI_String_Arr_t::iterator iterator;
	typedef J_UI_String_Arr_t::const_iterator const_iterator;

	iterator insert(j_size_t i_pos, const J_UI_Multi_String& irk_string);

	J_UI_Multi_String::const_iterator get_string_holding_index(j_size_t i_index)const;
	J_UI_Multi_String::iterator get_string_holding_index(j_size_t i_index);

	J_UI_String::const_iterator at_pos(j_size_t)const;
	J_UI_String::iterator at_pos(j_size_t);

	Pen_Pos_t get_string_indices(const_iterator i_pos)const;

	//Accessors
	J_UI_Char& operator[](j_size_t i_index);
	const J_UI_Char& operator[](j_size_t i_index)const;

	J_UI_String& front();
	const J_UI_String& front()const;

	J_UI_String& back();
	const J_UI_String& back()const;


	void push_back(const J_UI_String&);
	void push_back(const J_UI_Char&);
	void pop_back();

	void push_front(const J_UI_Char& i_char);
	void pop_front();

	LU_String lu_str()const;
	std::string std_str()const;

	J_UI_Multi_String operator+(const J_UI_Multi_String& irk_string)const;
	J_UI_Multi_String operator+(const J_UI_String&)const;
	J_UI_Multi_String operator+(const char*)const;
	//Status
	bool empty()const;
	j_size_t size()const;

	//Character Insertions/Deletion
	j_ui_char_iterator insert(j_size_t pos, J_UI_Char);
	j_ui_char_iterator insert(j_ui_char_iterator, J_UI_Char);
	void insert(j_size_t pos, const J_UI_String&);

	void erase(j_size_t pos);
	void erase(j_size_t pos, j_size_t size);
	void erase(iterator, iterator);
	void clear();

	J_UI_Multi_String& append(const J_UI_String&);
	J_UI_Multi_String& append(const J_UI_Multi_String);
	//Iterators
	iterator begin();
	iterator end();


	static char_iterator to_char_iterator(iterator);
	static char_const_iterator to_char_iterator(const_iterator);
	static char_const_iterator to_char_const_iterator(const_iterator);


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

	bool operator<(const J_UI_Multi_String&)const;
	std::pair<J_UI_String::const_iterator, J_UI_Multi_String::const_iterator>
		get_insert_pos(j_size_t i_index)const;

	std::pair<J_UI_String::iterator, J_UI_Multi_String::iterator>
		J_UI_Multi_String::get_insert_pos(j_size_t i_index);

	void set_color(const J_UI_Color& irk_color);
private:
	J_UI_String_Arr_t M_strings;


};


template<class Val_TypeL, class Val_TypeR>
typename std::enable_if<std::is_same<typename std::remove_cv<Val_TypeL>::type
	, typename std::remove_cv<Val_TypeR>::type>::value, J_UI_Multi_String::difference_type>::type
	operator-(const J_UI_Multi_String::multi_string_iterator<Val_TypeL>& _lhs
	, const J_UI_Multi_String::multi_string_iterator<Val_TypeL>&& _rhs){

		return (_lhs.pos() - _rhs.pos());
}


std::ostream& operator<<(std::ostream& ir_os, const J_UI_Multi_String& irk_string);
}

#endif //J_UI_MULTI_STRING_H



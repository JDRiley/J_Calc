#ifndef J_CALC_DATA_H
#define J_CALC_DATA_H

#include "J_Calc_Fwd_Decl.h"
//
#include <J_UI\J_UI_String.h>
//
#include <J_Calc_Symbol_Fwd_Decl.h>
//Containers
#include <j_tree.h>
//
#include <j_map.h>
namespace jomike{


class J_Calc_Data{
public:
	
	//Function Management
	void add_anonymous_function(j_function*);

	//View Management
	void attach_view(J_Calc_View_Shared_t);

	//Symbol Management
	void add_user_symbol(j_calc_symbol*);
	bool is_reserved_word(const jc_string_t&)const;
	bool is_reserved_symbol(const jc_string_t&)const;

	bool is_user_symbol(const jc_string_t&)const;
	j_calc_symbol* get_user_symbol(const jc_string_t&)const;
	void remove_user_symbol(const jc_string_t&);
	bool symbol_name_availability_status(const jc_string_t&);
	j_calc_symbol* get_symbol(const jc_string_t&)const;
	//Gets
	j_calc_symbol* get_reserved_symbol(const jc_string_t&)const;
	~J_Calc_Data();
	void clear_user_data();
	void update();
	void remove_view(J_Calc_View_Shared_t i_view);
private:
	static J_Calc_Data& get_instance();
	J_Calc_Data();
	void init_reserved_symbols();
	friend class Instance_Pointer<J_Calc_Data>;
	typedef j_tree<J_Calc_View_Shared_t> Calc_J_View_Cont_t;
	void clear_reserved();
	Calc_J_View_Cont_t M_calc_views;

	j_tree<jc_string_t> M_reserved_words;
	typedef j_map<jc_string_t, j_calc_symbol*> Symbol_Map_t;
	Symbol_Map_t M_reserved_symbols;
	Symbol_Map_t M_user_symbols;
};

}
#endif
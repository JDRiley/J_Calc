#ifndef J_CALC_DATA_H
#define J_CALC_DATA_H
#include <J_UI\J_UI_Model.h>
#include "J_Calc_Fwd_Decl.h"

//Containers
#include <j_tree.h>

namespace jomike{


class J_Calc_Data : public J_UI_Model{
public:
	

	//Box Management
	void add_math_text_box(Math_Input_Box_Shared_t i_math_box);

	//Function Management
	void add_anonymous_function(j_function*);

	//View Management
	void attach_view(J_View_Shared_t)override;

	//Symbol Management
	void add_user_symbol(const j_symbol*);
	bool is_reserved_word(const J_UI_String&)const;
	bool is_reserved_symbol(const J_UI_String&)const;

	bool is_user_symbol(const J_UI_String&)const;
	j_symbol* get_user_symbol(const J_UI_String&)const;
	void remove_user_symbol(const J_UI_String&);
	bool symbol_name_availability_status(const J_UI_String&);
	j_symbol* get_symbol(const J_UI_String&)const;
	//Gets
	j_symbol* get_reserved_symbol(const J_UI_String&)const;
	~J_Calc_Data();
	void clear_data()override;

	void remove_view(J_View_Shared_t i_view);
private:
	static J_Calc_Data& get_instance();
	J_Calc_Data();
	void init_reserved_symbols();
	friend class Instance_Pointer<J_Calc_Data>;
	typedef j_tree<J_Calc_View_Shared_t> Calc_J_View_Cont_t;

	Calc_J_View_Cont_t M_calc_views;
	typedef j_map<j_uint, Math_Input_Box_Shared_t> Math_Input_Box_Cont_t;
	Math_Input_Box_Cont_t M_math_input_boxes;
	j_tree<J_UI_String> M_reserved_words;
	typedef j_map<J_UI_String, j_symbol*> Symbol_Map_t;
	Symbol_Map_t M_reserved_symbols;
	Symbol_Map_t M_user_symbols;
};

}
#endif
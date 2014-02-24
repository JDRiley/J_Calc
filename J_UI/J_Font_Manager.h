#ifndef J_FONT_MANAGER_H
#define J_FONT_MANAGER_H

#include "J_UI_Fwd_Decl.h"
//
#include <J_Utile.h>

//Containers

//
#include <ex_array.h>
//
#include <map>
//
#include <RB_Tree.h>
//
#include <string>

namespace jomike{

J_FWD_DECL(Font_Data)

class Font_Data_Name_Comp{
public:
	bool operator()(Font_Data_Shared_t , const std::string&);
	bool operator()(const std::string&, Font_Data_Shared_t);
};

class J_Font_Manager{
public:
	void add_font(const std::string& font_filename
		, const std::string& font_name);
	void clear();
	~J_Font_Manager();
	J_Font_Face get_font_face(J_Context_Shared_t context_to_add_to, const std::string& font_name, int font_size);
	void delete_font_face(const std::string& font_name, int i_font_size);
	bool is_font_present(const std::string& i_font_name)const;
private:
	friend class Instance_Pointer<J_Font_Manager>;
	J_Font_Manager();
	static J_Font_Manager& get_instance();

	J_Context_Shared_t M_context;

	typedef std::map<std::string, Font_Data_Shared_t>
		Font_Face_Cont_t;

	FT_Library M_FT_library = nullptr;
	Font_Face_Cont_t M_font_data;

};








class J_FT_Init_Error : public J_Init_Error{
public:
	J_FT_Init_Error(const char* const ik_error_msg):J_Init_Error(ik_error_msg){}
};




}

#endif
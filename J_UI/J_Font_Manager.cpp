#include "J_Font_Manager.h"

//
#include <algorithm>
using std::fill;

//IO

//
#include <iostream>

using std::cerr; using std::endl;


//Libraries
#include <ft2build.h>
#include FT_FREETYPE_H
//
#include <GL\glew.h>
//
#include <J_OpenGl_Functions.h>
//
#include <J_Utile.h>
//
#include "J_Font_Face.h"
//
#include <j_map.h>
using std::string;

namespace jomike{

static Instance_Pointer<Contexts_Handler> s_contexts;

const int FT_POINT_TO_PARAMETER_CONVERSION = 64;
const int DEFAULT_DOTS_PER_INCH = 96;
const int DEFAULT_CHAR_POINT_SIZE = 24;
const int DEFAULT_NUMBER_OF_FT_LIBS = 1;
const int DEFAULT_NUMBER_OF_FT_FACES = 1;


class Font_Data{
public:
	std::string name;
	std::string filename;
	FT_Face ft_face;
	j_map<int, J_Font_Face> j_font_faces;
	~Font_Data();
};


J_Font_Manager::J_Font_Manager(){
	if(FT_Init_FreeType(&M_FT_library)){
		throw J_FT_Init_Error("Free Type initialization error occurred");
	}
	try{
		M_context = s_contexts->create_j_context(10, 10
			, "This J_Font_Manager Window is Invisible");

	} catch(...){
		clear();
		throw;
	}
}

Font_Data::~Font_Data(){
	for_each(j_font_faces.begin(), j_font_faces.end()
		, [](J_Font_Face y_font_face){delete y_font_face;});
	FT_Done_Face(ft_face);
	ft_face = nullptr;
}

static FT_GlyphSlot get_glyph(FT_Face i_face, j_ulint i_charcode
		, FT_Int32 i_load_flags);






J_Font_Manager& J_Font_Manager::get_instance(){
	static J_Font_Manager font_manager;
	return font_manager;
}


bool J_Font_Manager::is_font_present(const string& i_font_name)const{
	return M_font_data.count(i_font_name);
}

void J_Font_Manager::delete_font_face(const string& irk_font_name, int i_font_size){
	assert(is_font_present(irk_font_name));
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	delete M_font_data[irk_font_name]->j_font_faces[i_font_size];
}

void J_Font_Manager::delete_font_face(J_Font_Face i_fontface){
	delete i_fontface;
}





J_Font_Manager::~J_Font_Manager(){
	clear();
}

void J_Font_Manager::clear(){
	

	if (!M_context){
		return;
	}
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	M_font_data.clear();
	
	FT_Done_FreeType(M_FT_library);

	
	M_context.reset();
	s_contexts->detach_active_context();
	M_FT_library = nullptr;
}

J_Font_Face J_Font_Manager::get_font_face(const std::string& i_font_name
										  , int i_font_size){
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	if(!M_font_data.count(i_font_name)){
		throw J_Argument_Error("Font Does Not Exists!");
	}

	J_Font_Face source_font_face;

	if(auto found_font_face = M_font_data[i_font_name]->j_font_faces[i_font_size]){
		source_font_face = found_font_face;
	}else{
		source_font_face = new J_Font_Face__(M_context, M_font_data[i_font_name]->ft_face
		, i_font_size, FT_LOAD_RENDER);
		M_font_data[i_font_name]->j_font_faces[i_font_size] = source_font_face;
	}

	J_Font_Face font_face_to_return = new J_Font_Face__(*source_font_face);

	return font_face_to_return;

}

FT_Face create_face(FT_Library i_lib, const char* ik_pathname, int i_level){
	FT_Face new_face;
	if(FT_New_Face(i_lib, ik_pathname, i_level
		, &new_face)){
		throw J_FT_Init_Error("Free Type Face initialization error occurred");
	}
	return new_face;
}



void J_Font_Manager::add_font(const string& i_font_filename, const string& i_font_name){
	if(M_font_data.count(i_font_name)){
		return;
	}
	Font_Data_Shared_t new_font_data(new Font_Data);
	new_font_data->ft_face = create_face(M_FT_library, i_font_filename.c_str(), 0);
	new_font_data->filename = i_font_filename;
	new_font_data->name = i_font_name;
	

	M_font_data[i_font_name] = new_font_data;
}



}
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
#include <J_OpenGl.h>
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





//J_Font_Face__::J_Font_Face__(J_Context_Shared_t i_context, FT_Face i_face, int i_font_size, int i_render_mode):tab_distance(4)
//	, new_line_size(static_cast<int>(i_face->height/64*11.0/10)){
//	
//
//	if(FT_Set_Char_Size(i_face, i_font_size*FT_POINT_TO_PARAMETER_CONVERSION,
//		i_font_size*FT_POINT_TO_PARAMETER_CONVERSION, DEFAULT_DOTS_PER_INCH, DEFAULT_DOTS_PER_INCH)){
//		throw J_Init_Error("Could Not Set Character Size Properly!");
//	}
//	
//	int num_glyphs = i_face->num_glyphs;
//
//	M_bitmap_metrics.reserve(num_glyphs);
//
//	//Already Size texture ids cause they are passed in by reference to opengl
//	M_texture_ids.resize(num_glyphs);
//	glGenTextures(num_glyphs, &M_texture_ids[0]);
//	//used to prevent excessive allocations
//	j_bitmap<j_ubyte, 1> expanded_bitmap;
//	
//	for(int i=0; i < i_face->num_glyphs; i++){
//		FT_GlyphSlot glyph;
//		if('\t' == i){
//			glyph = get_rendered_glyph(i_face, ' ', i_render_mode);
//			text_bitmap_formatter(&expanded_bitmap, glyph->bitmap.width, glyph->bitmap.rows
//			, glyph->bitmap.buffer);
//			expanded_bitmap.resize(expanded_bitmap.width()*tab_distance,expanded_bitmap.height());
//			FT_Bitmap& bitmap = glyph->bitmap;
//			M_bitmap_metrics.push_back(Bitmap_Metrics(expanded_bitmap.width(), expanded_bitmap.height()
//				, glyph->bitmap_left, bitmap.rows - glyph->bitmap_top, glyph->advance.x*tab_distance/64, glyph->advance.y/64));
//
//		}else{
//			glyph = get_rendered_glyph(i_face, i, i_render_mode);
//			text_bitmap_formatter(&expanded_bitmap, glyph->bitmap.width, glyph->bitmap.rows
//			, glyph->bitmap.buffer);
//			FT_Bitmap& bitmap = glyph->bitmap;
//			M_bitmap_metrics.push_back(Bitmap_Metrics(expanded_bitmap.width(), expanded_bitmap.height()
//				, glyph->bitmap_left, bitmap.rows - glyph->bitmap_top, glyph->advance.x/64, glyph->advance.y/64));
//
//		}
//
//		glBindTexture(GL_TEXTURE_2D, M_texture_ids[i]);
//		glTexStorage2D(GL_TEXTURE_2D, 1, GL_R8, expanded_bitmap.width(), expanded_bitmap.height());
//		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, expanded_bitmap.width(), expanded_bitmap.height(), GL_RED
//			, GL_UNSIGNED_BYTE, expanded_bitmap.data());
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//		glBindTexture(GL_TEXTURE_2D, 0);
//		
//		
//		if(!(i%500)){
//			cerr << "\nGlyph: " << i << " created!";
//		}
//
//	}
//}

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

J_Font_Face J_Font_Manager::get_font_face(J_Context_Shared_t i_context_to_add_to
										  ,const std::string& i_font_name
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
		throw J_FT_Init_Error("Free Type Default Face initialization error occurred");
	}
	return new_face;
}



void J_Font_Manager::add_font(const string& i_font_filename, const string& i_font_name){
	Font_Data_Shared_t new_font_data(new Font_Data);
	new_font_data->ft_face = create_face(M_FT_library, i_font_filename.c_str(), 0);
	new_font_data->filename = i_font_filename;
	new_font_data->name = i_font_name;
	assert(!M_font_data.count(i_font_name));

	M_font_data[i_font_name] = new_font_data;
}



}
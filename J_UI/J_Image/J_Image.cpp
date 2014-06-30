#include "../../J_Error/J_Error.h"
#include "J_Image.h"


using std::numeric_limits;
//Math
#include <cmath>
using std::log;using std::pow;using std::max;

//Utilities 
#include <cassert>
//
#include <j_map.h>
//
#include <unordered_map>
namespace jomike{



int next_power_of_two(int i_x){
	assert(i_x > 0);
	for(int  i=0 ; i < log(numeric_limits<int>::max()); i++){
		if(i_x <= (1 << i)){
			return (1 << i);
		}
	}
	throw J_Error("Next Power Of Two Did Not Find A Value");
}

j_int j_num_channels(Image_Format i_format){
	switch(i_format){
	case Image_Format::BGR24_UBYTE:
	case Image_Format::RGB24_UBYTE:
		return 3;
	case Image_Format::RGBA32_UBYTE:
		return 4;
	default:
		assert(!"Unhandled Image Format");
		return 0;
	}
}





const j_ubyte* convert_to_rgba32(int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap){
	static std::unordered_map<const j_ubyte*, j_map<J_UI_Color, j_ubyte_arr_t>> s_rgba32_cache;
	
	const int bitmap_size = i_width*i_height * 4;
	auto& cache_bitmap_arr = s_rgba32_cache[i_bitmap][i_color];
	if(!cache_bitmap_arr.empty()){
		assert(bitmap_size == cache_bitmap_arr.size());
		
		return cache_bitmap_arr.data();
	}
	
	cache_bitmap_arr.resize(bitmap_size);



	//int width_offset = i_width * 4;

	j_ubyte red_val = static_cast<j_ubyte>(i_color.red()*numeric_limits<j_ubyte>::max());
	j_ubyte green_val = static_cast<j_ubyte>(i_color.green()*numeric_limits<j_ubyte>::max());
	j_ubyte blue_val = static_cast<j_ubyte>(i_color.blue()*numeric_limits<j_ubyte>::max());
	j_ubyte alpha_val = static_cast<j_ubyte>(i_color.alpha()*numeric_limits<j_ubyte>::max());

	j_ubyte* bitmap_ptr = cache_bitmap_arr.data();
#define bitmap bitmap_ptr
	int offset = 0;
	int source_offset = 0;
	for(int i = 0; i < i_height; i++){
		for(int b = 0; b < i_width; ++b){
			//j_float ratio = byte_normalized(i_bitmap[i_width*i + b]);
			static const j_ubyte max_val = ((1 << (sizeof(j_ubyte)) * 8) - 1);
			j_float ratio = static_cast<j_float>(i_bitmap[source_offset++]) / max_val;
			//int offset = width_offset*i + 4 * b;
			bitmap[offset++] = static_cast<j_ubyte>(ratio*red_val);
			bitmap[offset++] = static_cast<j_ubyte>(ratio*green_val);
			bitmap[offset++] = static_cast<j_ubyte>(ratio*blue_val);
			bitmap[offset++] = static_cast<j_ubyte>(ratio*alpha_val);
		}
	}
#undef bitmap
	
	assert(bitmap_size == cache_bitmap_arr.size());

	return cache_bitmap_arr.data();
}

const J_UI_Color& J_BLACK = J_Color::Black;
const J_UI_Color& J_WHITE = J_Color::White;
const J_UI_Color& J_CLEAR = J_Color::Clear;
const J_UI_Color& J_CYAN = J_Color::Cyan;
const J_UI_Color& J_YELLOW = J_Color::Yellow;
const J_UI_Color& J_RED = J_Color::Red;
const J_UI_Color& J_BLUE = J_Color::Blue;
const J_UI_Color& J_GREEN = J_Color::Green;


}

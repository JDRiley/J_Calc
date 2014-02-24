#include "../../J_Error/J_Error.h"
#include "J_Image.h"


using std::numeric_limits;
//Math
#include <cmath>
using std::log;using std::pow;using std::max;

//Utilities 
#include <cassert>

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

j_ubyte_arr_t convert_to_rgba32(int i_width, int i_height, const J_UI_Color& i_color, const j_ubyte* i_bitmap){
	j_ubyte_arr_t bitmap(i_width*i_height * 4);

	int width_offset = i_width * 4;

	j_ubyte red_val = static_cast<j_ubyte>(i_color.red()*numeric_limits<j_ubyte>::max());
	j_ubyte green_val = static_cast<j_ubyte>(i_color.green()*numeric_limits<j_ubyte>::max());
	j_ubyte blue_val = static_cast<j_ubyte>(i_color.blue()*numeric_limits<j_ubyte>::max());
	j_ubyte alpha_val = static_cast<j_ubyte>(i_color.alpha()*numeric_limits<j_ubyte>::max());

	for(int i = 0; i < i_height; i++){
		for(int b = 0; b < i_width; ++b){
			j_float ratio = byte_normalized(i_bitmap[i_width*i + b]);
			bitmap[width_offset*i + 4*b] = static_cast<j_ubyte>(ratio*red_val);
			bitmap[width_offset*i + 4*b + 1] = static_cast<j_ubyte>(ratio*green_val);
			bitmap[width_offset*i + 4*b + 2] = static_cast<j_ubyte>(ratio*blue_val);
			bitmap[width_offset*i + 4*b + 3] = static_cast<j_ubyte>(ratio*alpha_val);
		}
	}

	return bitmap;
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

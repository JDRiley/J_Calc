#include "J_Calc_View.h"
//
#include <JGL/J_OpenGl_Functions.h>

using namespace J_UI;
namespace jomike{

J_Calc_View::J_Calc_View( int i_width, int i_height, const char* i_title
			, Contexts_Handler::Window_Flags i_window_flags
									 , j_monitor_t i_monitor, j_window_t i_share_window)
									 :J_View(i_width, i_height, i_title, i_window_flags, i_monitor, i_share_window){

	Context_RAII context_saver;
	make_active_context();
	j_clear_color(J_BLACK);
}


}
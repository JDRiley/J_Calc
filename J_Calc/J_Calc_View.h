#ifndef J_INTEGRATOR_VIEW_H
#define J_INTEGRATOR_VIEW_H

#include <J_UI\J_View.h>

#include "J_Calc_Fwd_Decl.h"
#include <J_Fwd_Decl.h>



namespace jomike{


class J_Calc_View : public J_View{
public:
	J_Calc_View(int i_width, int i_height, const char* i_title = "A J_Calc_View Object!"
									, j_monitor_t i_monitor = nullptr
									, j_window_t i_share_window = nullptr);
	~J_Calc_View(){}
private:

};



}

#endif
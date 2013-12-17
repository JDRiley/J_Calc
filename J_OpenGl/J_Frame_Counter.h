#ifndef J_FRAME_COUNTER_H
#define J_FRAME_COUNTER_H


#include "J_OpenGL_Fwd_Decl.h"
//
#include <ex_array.h>
namespace jomike{

class J_Frame_Counter{
public:
	J_Frame_Counter(int frame_threshold = 10);
	void count_frame();
	j_dbl fps()const;
private:
	int M_frame_count;
	int M_frame_threshold;
	j_dbl M_time_since_last_count;
	j_dbl M_fps;
	j_size_t M_memory_size = 20;
	ex_array<j_dbl> M_fps_memory;

};


}

#endif
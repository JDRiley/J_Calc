#ifndef J_CAMERA_STREAM_H
#define J_CAMERA_STREAM_H

//
#include "J_PXC_Fwd_Decl.h"
//
#include <J_UI\J_Image_Pane.h>


namespace jomike{

class J_Camera_Stream : public J_Image_Pane{
public:
	J_Camera_Stream(const J_Rectangle&, Image_Format i_format = Image_Format::BGR24_UBYTE);
	
	void update()override;
	
	
	~J_Camera_Stream();
private:
#ifdef VS_2013
	bool M_camera_filter_fade_status = false;
#else
	bool M_camera_filter_fade_status;
	void default_initialization();
#endif // VS_2013

	
	void camera_filter_fade();
};


}

#endif
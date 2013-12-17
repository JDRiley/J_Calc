#ifndef J_PXC_H
#define J_PXC_H

#include <J_Fwd_Decl.h>

#pragma  comment(lib, "J_PXC_Interface"J_LIB_APPEND)
//
#include <ex_array.h>
//
#include <J_Image.h>

J_POINTER_DECL(PXCAccelerator, PXC_Accelerator)
J_POINTER_DECL(PXCCapture, PXC_Capture)
J_POINTER_DECL(PXCSession, PXC_Session)
J_POINTER_DECL(PXCImage, PXC_Image)
J_POINTER_DECL(PXCGesture, PXC_Gesture)

enum pxcStatus;

namespace jomike{

#define J_PXC_RELEASE(pointer) if(pointer){pointer->Release();} pointer = nullptr

J_FWD_DECL(J_Stream_Processor)
J_FWD_DECL(J_PXC_Device)
J_FWD_DECL(J_PXC_Gesture)
J_FWD_DECL(J_PXC_Depth_Stream_Image_Processor)
J_FWD_DECL(J_PXC_Depth_Stream_Depth_Processor)
J_FWD_DECL(J_PXC_Color_Stream_Processor)
J_FWD_DECL(J_Face_Stream_Processor)
J_FWD_DECL(J_PXC_Stream)
J_FWD_DECL(J_PXC_Face_Processor)
J_FWD_DECL(J_PXC_Gesture_Processor)

#define NO_PXC_ERROR assert(no_pxc_error())


const int NUM_GEO_SIDES = 2;
const int NUM_NODES_PER_GEO_SIDE = 11;
const int NUM_DEPTH_CHANNELS = 4;
const int MAX_FACES = 8;
const int NUM_LANDMARKS = 7;

J_FWD_DECL(J_Camera_Stream)

pxcStatus& pxc_status();

bool is_profile_color(const Image_Format& i_format);
class J_PXC_Error_Manager{
public:
	J_PXC_Error_Manager();
	pxcStatus& operator=(pxcStatus);
	static pxcStatus& error();
	static void clear();
private:
	static ex_array<pxcStatus> Ms_errors;
	pxcStatus& M_pxc_error;
};

void no_error_exec(pxcStatus);

bool pxc_error();

bool pxc_status_item_unavailable();

bool no_pxc_error();

bool no_pxc_warning();
bool pxc_exec_timeout();
void pxc_clear_error();

bool pxc_status_clear();

bool pxc_warning();

bool pxc_status_waiting_execution();
enum class Stream_Formats{COLOR, DEPTH};




}

#endif
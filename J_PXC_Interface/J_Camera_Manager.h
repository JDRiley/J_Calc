#ifndef J_CAMERA_MANAGER_H
#define J_CAMERA_MANAGER_H
//
#include "J_PXC_Fwd_Decl.h"

//Container

//
#include <ex_array.h>
//
#include <string>
//
#include <functional>
//
#include "pxcsmartptr.h"
//
#include <J_Frame_Counter.h>
//
#include "J_PXC_Face.h"
//
#include "J_PXC_Device.h"
//
#include "J_PXC_Face_Processor.h"
//
#include "J_PXC_Gesture_Processor.h"
//
#include "J_PXC_Color_Stream_Processor.h"
//
#include "J_PXC_Depth_Stream_Processor.h"
//
#include <map>
//
#include <J_Image.h>
#include "J_PXC_Face_Processor.h"
namespace jomike{


class J_Camera_Error : public J_Error{
public:
	J_Camera_Error(const std::string& irk_string);
	J_Camera_Error();
	operator bool()const;
private:

};


class J_Camera_Manager{
public:
//Constructors---------------------------------------------------------------------------------------

	static J_Camera_Manager& get_instance();
	
	///void init()
	/*																			*/
	/*      Initializes the J_Cam_View And decides camera source				*/
	//void init();
		
//Stream Mangement---------------------------------------------------------------------------------------
	

	///void create_video_stream(int width, int height, j_dbl fps, bool depth = false)
	/*																			*/
	/*    Creates a stream closest to the given parameters.						*/  
	/*																			*/
	void create_video_stream(int width, int height, j_dbl fps, bool depth = false);


	///void render_frame(int stream_index, j_ubyte*)
	/*																			*/
	/*	Renders Data to image buffer											*/    
	/*																			*/
	void render_frame(int stream_index, const j_ubyte*);

//Device Management---------------------------------------------------------------------------------------
	
	///void init_device_list()
	/*																			*/
	/*      Initializes the list of Devices and displays them on the screen		*/
	void init_device_list();



//Destruction---------------------------------------------------------------------------------------

	///~J_Camera_Manager()
	/*																			*/
	/*     Destructor of the J_Camera_Manager									*/ 
	~J_Camera_Manager();



	///void set_stream(int device_id)
	/*																			*/
	/*      Sets the source device for the stream object.						*/
	/*																			*/
	void set_stream(std::wstring i_device_name);

	///void mouse_button_cmd(j_window_t, int i_key, int i_action, int i_modifiers)
	/*																			*/
	/*      Does work and calls mouse_button_command type stuff					*/
	/*																			*/
	void mouse_button_cmd(j_window_t, int i_key, int i_action, int i_modifiers);

	///void cursor_pos_input_cmd(j_window_t, j_dbl, j_dbl)
	/*																			*/
	/*      cursor_pos_input_cmd												*/
	/*																			*/
	void cursor_pos_input_cmd(j_window_t, j_dbl, j_dbl);



	///void notify_geonode(PXC_GeoNode)
	/*																			*/
	/*		Notify the manager of a geonode										*/
	/*																			*/
	void notify_geonodes(PXC_GeoNode* primary, PXC_GeoNode* secondary);

	///void poll_face_data()
	/*																			*/
	/*		Polls the face data stored in the manager							*/
	/*																			*/
	void push_face_data();

	///void push_color_stream()
	/*																			*/
	/*		Pushes out the current color stream									*/
	/*																			*/
	void push_color_stream();



	///void clear()
	/*																			*/
	/*		Clears Data															*/
	/*																			*/
	void clear();



	///void close_window()
	/*																			*/
	/*		Closes the window													*/
	/*																			*/
	void close_window();

	///PXC_Gesture_Data pop_gesture()
	/*																			*/
	/*		Pops the oldest gesture still in the manager and returns it			*/
	/*	If label body is zero not a gesture										*/
	PXC_Gesture_Data pop_gesture();

	///static PXCSession& get_pxc_session()
	/*																			*/
	/*		Returns the PXCSession being used by the manager					*/
	/*																			*/
	static PXCSession& get_pxc_session();

	///ex_array<std::wstring> get_device_names()const
	/*																			*/
	/*		Get a list of all device names in manager							*/
	/*																			*/
	ex_array<std::wstring> get_device_names()const;

	j_float depth_color_stream_fov_x_ratio()const;
	j_float depth_color_stream_fov_y_ratio()const;
	
	J_PXC_Gesture_Processor_Shared_t get_gesture_processor();
	J_PXC_Face_Processor_Shared_t get_face_processor();
	J_PXC_Color_Stream_Processor_Shared_t get_color_stream_processor();
	J_PXC_Depth_Stream_Depth_Processor_Shared_t get_depth_stream_depth_processor();

	const J_PXC_Stream& get_depth_stream()const;
	
	const J_PXC_Stream& get_color_stream()const;
	
	void set_device(J_PXC_Device_Shared_t i_device);
	void start_stream();
	bool has_stream()const;
	bool has_device()const;
	j_float get_aspect_ratio()const;
	bool gesture_detection_status()const;
	bool face_detection_status()const;
	void set_gesture_detection_status(bool);
	void set_face_detection_status(bool);
	j_dbl get_gesture_fps()const;
	
	void poll_events();
	void set_mirror_status(bool i_status);
	bool mirror_status()const;
	void notify_end();
private:// PRIVATE		PRIVATE		PRIVATE		PRIVATE		PRIVATE		PRIVATE		PRIVATE		
	///J_Camera_Manager(.....)
	/*																			*/
	/*      Constructs a J_Camera_Manager object to display what's on the screen*/
	/*	This creates it's own window for viewing a connected Camera				*/
	J_Camera_Manager();
	PXCSmartPtr<PXCSession> M_pxc_session;

	
	///void destroy()
	/*																			*/
	/*      destroys and deallocates all data									*/
	/*																			*/
	void destroy();


	



	///void update()
	/*																			*/
	/*      updates the state of the camera manager								*/
	/*																			*/
	void update();

	void find_devices_for_module(int i_capture_id, PXCCapture* i_capture);

	std::map<std::wstring, J_PXC_Device_Shared_t> M_devices_by_name;
	

	ex_array<PXC_Gesture_Data> M_gestures;
	void clear_devices();

	J_Face_Stream_Processor_Shared_t M_j_face_stream_processor;
	J_PXC_Device_Shared_t M_device;
	J_PXC_Stream_Shared_t M_depth_stream;	
	J_PXC_Stream_Shared_t M_color_stream;
	J_PXC_Face_Processor_Shared_t M_face_processor;
	J_PXC_Gesture_Processor_Shared_t M_gesture_processor;
	J_PXC_Depth_Stream_Depth_Processor_Shared_t M_depth_stream_processor;
	J_PXC_Color_Stream_Processor_Shared_t M_color_stream_processor;
	J_Frame_Counter M_gesture_fps_counter;
	void clear_device();

#ifdef VS_2013
	bool M_mirror_status = false;
#else
	bool M_mirror_status;
	void default_initialization();
#endif // VS_2013
	
};




}

#endif
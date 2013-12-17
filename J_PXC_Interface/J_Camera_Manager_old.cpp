#include "J_Camera_Manager.h"
//
#include <J_UI/J_FT_Text_Display.h>
//
#include "../J_OpenGl/J_Shader_Program.h"
//
#include <J_Error.h>
//
#include <J_OpenGl.h>
//
#include <J_UI\J_Font_Manager.h>
//
#include <J_UI/J_View.h>
//
#include <J_String.h>
//
#include <J_UI/J_Shape.h>
//
#include <J_UI/J_Text_Box.h>
//
#include <J_UI/J_UI_Model.h>
//
#include "J_Camera_Stream.h"

//Camera Utilities


//
#include <string>

using std::string;using std::wstring;

//
#include <gl/glew.h>
//
#include <GLFW/glfw3.h>
//
#include <array>
using std::array;
//
#include<thread>
#include <chrono>
namespace this_thread = std::this_thread;
namespace chrono = std::chrono;
//
#include <limits>

//IO Faciliities
#include <iostream>
using std::wostringstream;
using std::cerr; using std::endl;
using std::unique_ptr;
const int A_HIGH_NUMBER = 20000;
const int MAX_DEVICES = 50;
using std::fill;
using std::numeric_limits;
using std::max; using std::map;
using std::dynamic_pointer_cast;
namespace jomike{

static j_uint gen_texture(int i_width, int i_height){
	j_uint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glTexStorage2D(GL_TEXTURE_2D, 0, GL_RGB8, i_width, i_height);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texture_id;
}



static Instance_Pointer<J_Camera_Manager> s_camera_manager;
static Instance_Pointer<J_UI_Model> s_ui_model;
static Instance_Pointer<Contexts_Handler> s_contexts;
static Instance_Pointer<J_Font_Manager> s_font_manager;


J_Camera_Manager& J_Camera_Manager::get_instance(){
	static J_Camera_Manager camera_manager;
	return camera_manager;
}

static void camera_manager_cursor_pos_callback(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos){
	static map<j_window_t, j_dbl> callback_times;

	if((glfwGetTime() - callback_times[i_window]) > 0.02f){
		s_camera_manager->cursor_pos_input_cmd(i_window, i_x_pos, i_y_pos);
		callback_times[i_window] = glfwGetTime();
	}
}


static void camera_manager_mouse_button_callback(j_window_t i_window, int i_mouse_button, int i_action, int i_modifiers){
	
	s_camera_manager->mouse_button_cmd(i_window, i_mouse_button, i_action, i_modifiers);
}

void J_Camera_Manager::cursor_pos_input_cmd(j_window_t , j_dbl i_x_pos, j_dbl i_y_pos){
	//Context_RAII context_saver;
	M_cursor_pos.first = static_cast<int>(i_x_pos);
	M_cursor_pos.second = static_cast<int>(i_y_pos);

	M_view->set_cursor_pos(M_cursor_pos.first, M_cursor_pos.second);
}

void J_Camera_Manager::create_video_stream(int i_width, int i_height
									 , j_dbl i_fps, bool ){
										 assert(0);
	PXCCapture::VideoStream::DataDesc stream_desc 
		= zero_set<PXCCapture::VideoStream::DataDesc>();
	stream_desc.streams[0].format = PXCImage::COLOR_FORMAT_RGB32;
	
	PXCSession::ImplDesc stream_filter = zero_set<PXCSession::ImplDesc>();
	stream_filter.group = PXCSession::IMPL_GROUP_SENSOR;
	stream_filter.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;
	PXCCapture* capture;
	PXCSession::ImplDesc desc_filter = zero_set<PXCSession::ImplDesc>();
	desc_filter.group = PXCSession::IMPL_GROUP_SENSOR;
	desc_filter.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;
	pxcStatus error = PXC_STATUS_NO_ERROR;
	PXCFaceAnalysis* face_analysis;

	error = M_pxc_session->CreateImpl<PXCFaceAnalysis>(&face_analysis);

	assert(error >= PXC_STATUS_NO_ERROR);
 

	/* Configure the face module */

	PXCFaceAnalysis::ProfileInfo pinfo1;
	face_analysis->QueryProfile(0,&pinfo1);

	face_analysis->SetProfile(&pinfo1);

 

	/* Configure face detection */

	PXCFaceAnalysis::Detection *detection=face_analysis->DynamicCast<PXCFaceAnalysis::Detection>();

	PXCFaceAnalysis::Detection::ProfileInfo pinfo2;

	detection->QueryProfile(0,&pinfo2);

	detection->SetProfile(&pinfo2);

 

	PXCCapture::VideoStream::ProfileInfo used_profile = PXCCapture::VideoStream::ProfileInfo();
	for( int module_index = 0; ;module_index++){
		PXCSession::ImplDesc desc;
		

		error = M_pxc_session->QueryImpl(&desc_filter, module_index, &desc);
		if(error == PXC_STATUS_ITEM_UNAVAILABLE){
			break;
		}
		if(error < PXC_STATUS_NO_ERROR){
			continue;
		}
		error = M_pxc_session->CreateImpl<PXCCapture>(&desc, &capture);
		if(error == PXC_STATUS_ITEM_UNAVAILABLE){
			break;
		}
		if(error < PXC_STATUS_NO_ERROR){
			continue;
		}
		PXCCapture::DeviceInfo device_info;
		for(int device_index = 0 ;;device_index++){
			PXCSmartPtr<PXCCapture::Device> device_sp;
			error = capture->QueryDevice(device_index, &device_info);
			if(error < PXC_STATUS_NO_ERROR){
				break;
			}
			error = capture->CreateDevice(device_index, &device_sp);
			if(error < PXC_STATUS_NO_ERROR){continue;}
			bool stream_matched = false;
			for(int stream_index = 0; ; ++stream_index){
				PXCCapture::Device::StreamInfo stream_info;
				error = device_sp->QueryStream(stream_index, &stream_info);
				if(error < PXC_STATUS_NO_ERROR){break;}
				if((PXCCapture::VideoStream::CUID == stream_info.cuid)
					&& !stream_matched){
					PXCCapture::VideoStream* video_stream = nullptr;
					error = device_sp->CreateStream(stream_index, &video_stream);
					if(error < PXC_STATUS_NO_ERROR){break;}

					ex_array<PXCCapture::VideoStream::ProfileInfo> profiles;
					for(int profile_index = 0; ; ++profile_index){
						PXCCapture::VideoStream::ProfileInfo profile_info;
						error = video_stream->QueryProfile(profile_index, &profile_info);

						if(error < PXC_STATUS_NO_ERROR){break;}

						profiles.push_back(profile_info);
					}
					
					bool found_profile = false;
					for(auto f_profile : profiles){
						if(i_width != safe_int_cast(f_profile.imageInfo.width)){
							continue;
						}

						if(i_height != safe_int_cast(f_profile.imageInfo.height)){
							continue;
						}

						if(i_fps > static_cast<j_dbl>(f_profile.frameRateMax.numerator)
							/f_profile.frameRateMax.denominator){
								continue;
						}

						used_profile = f_profile;
						error = video_stream->SetProfile(&f_profile);
						if(error >= PXC_STATUS_NO_ERROR){
							M_video_streams.push_back(video_stream);
							M_stream_texture_ids.push_back(gen_texture(f_profile.imageInfo.width
								, f_profile.imageInfo.height));
							found_profile = true;
							break;
						}
					}
					
					if(error < PXC_STATUS_NO_ERROR){break;}
					assert(found_profile);


				}//stream not matched
				//check for proper profile

			}//Stream Iteration
			if(error >= PXC_STATUS_NO_ERROR){break;}
		}//Device Iteration
		
	}//Module Iteration

	error = PXC_STATUS_NO_ERROR;

	PXCCapture::VideoStream::ProfileInfo info;
	M_video_streams.back()->QueryProfile(&info);
	stream_desc.streams[0].sizeMin.width = info.imageInfo.width;
    stream_desc.streams[0].sizeMin.height = info.imageInfo.height;
    stream_desc.streams[0].sizeMax.width = info.imageInfo.width;
    stream_desc.streams[0].sizeMax.height = info.imageInfo.height;

	auto cur_video_stream = M_video_streams.back();

	assert(error >= PXC_STATUS_NO_ERROR);

	PXCImage* image;
	PXCScheduler::SyncPoint* sync_point;
	PXCScheduler::SyncPoint* face_sync_point;

	error = cur_video_stream->ReadStreamAsync(&image, &sync_point);
	error = face_analysis->ProcessImageAsync(&image, &face_sync_point);

	assert(error >=PXC_STATUS_NO_ERROR);
	pxcU32 idx = 0;
	error = sync_point->SynchronizeEx(1u, &sync_point, &idx, numeric_limits<pxcU32>::max());
	error = face_sync_point->SynchronizeEx(1u, &face_sync_point, &idx, numeric_limits<pxcU32>::max());
	assert(error >= PXC_STATUS_NO_ERROR);

	int image_height = info.imageInfo.height;
	int image_width = info.imageInfo.width;

	j_set_window_size(s_contexts->get_active_context(), image_width, image_height);

	assert(error >= PXC_STATUS_NO_ERROR);

	glUseProgram(M_image_shader->image_program_id());
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	PXCImage::ImageData image_data;	
	error = image->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::COLOR_FORMAT_RGB24, &image_data);
	assert(error >= PXC_STATUS_NO_ERROR);
	

	

	j_dbl last_update_time = -1.0f;

	cerr << "\nStarting Camera Cycle" << endl;

	
	
	//GLubyte val = 0;
	j_clear_color(J_Color::Clear);
	j_dbl start_time = j_get_time();
	j_dbl last_frame_check = start_time;
	int n_frames=0;


	J_UI_Box_Shared_t face_box(new J_UI_Box(J_Rectangle(-0.5f,-0.5f, 1, 1)));


	M_view->add_display_box(face_box->get_ID());

	J_Font_Face log_font_face = s_font_manager->get_font_face(M_context, "times_italics", 12);
	J_Text_Box_Shared_t face_data_box(
		new J_Text_Box(J_Rectangle(-1.0f,0.4f, 0.5f, 0.6f)));

	M_view->add_text_display(face_data_box->get_ID());



	face_data_box->set_fill_visibility_status(true);
	face_data_box->set_outline_visibility_status(true);
	face_data_box->set_outline_thickness(1.0f);
	
	
	face_box->set_outline_visibility_status(true);
	face_box->set_fill_visibility_status(true);
	face_box->set_outline_thickness(4.0f);

	s_ui_model->add_box(face_box);
	s_ui_model->add_text_box(face_data_box);


	while(last_update_time < 1000000.0){

		last_update_time = j_get_time();
		j_poll_events();
	
		if(n_frames++ > 120){
			cerr << "\nFPS:: " << j_dbl(n_frames)/(j_get_time() - last_frame_check) << endl;
			last_frame_check = last_update_time;
			n_frames = 0;
		}
		
		error = sync_point->Synchronize(2000000u);
		//error = face_sync_point->Synchronize(2000000u);
		/*
		for(unsigned i=0; i < info.imageInfo.height; i++){
			memcpy_s(image_buffer.data()+i*image_width*4
				, image_width*4, image_data.planes[0]+i*image_data.pitches[0], image_width*4);
			
		}*/
		
		//fill(image_buffer.begin(), image_buffer.end(), j_ubyte(val++));
		pxcBYTE* image_buffer_ptr = (image_data.pitches[0] >= 0) ? image_data.planes[0] :
			image_data.planes[0] + image_data.pitches[0]*image_height;

		render_frame(0, image_buffer_ptr);

		error = image->ReleaseAccess(&image_data);
		PXCFaceAnalysis::Detection::Data face_data = zero_set<PXCFaceAnalysis::Detection::Data>();

		pxcUID face_id;
		error = face_analysis->QueryFace(0, &face_id);
		if(error >= PXC_STATUS_NO_ERROR){
			detection->QueryData(face_id, &face_data);

			std::ostringstream ostr;
			int x = face_data.rectangle.x;
			int y = image_height - face_data.rectangle.y;
			ostr << "Face Position: " << x << ", " << y
				<< "\nHeight: " << face_data.rectangle.w 
				<< "\nWidth: " << face_data.rectangle.h
				<< "\nConfidence: " << face_data.confidence;
			
			face_data_box->set_string(J_UI_String(ostr.str(), log_font_face));
			face_data_box->set_outline_color(J_Color::Cyan);
			face_data_box->set_fill_color(J_Color::Black.alpha(0.2f));
			j_window_t window = s_contexts->get_window(s_contexts->get_active_context());
			face_box->set_outline_visibility_status(true);
			face_box->set_outline_color(
				J_Color::Cyan.alpha(face_data.confidence/100.0f));
			face_box->set_fill_color(J_Color::Black.alpha(0.2f));
			if(face_data.confidence > 0){

				face_box->set_box(convert_x_coord(window, x)
					,convert_x_coord(window, x + face_data.rectangle.w)
					, convert_y_coord(window, y )
					, convert_y_coord(window, y- face_data.rectangle.h));
			}

		}else{
				face_data_box->set_string(J_UI_String("No Detected Face", log_font_face));

				face_box->set_outline_color(J_Color::Yellow.alpha(0.4f));
				face_box->set_fill_color(J_Color_RGBA<j_float>(0.2f,0.0f,0.0f,0.3f));
				face_data_box->set_outline_color(J_Color::Yellow.alpha(0.4f));
				face_data_box->set_fill_color(J_Color_RGBA<j_float>(0.2f,0.0f,0.0f,0.3f));

		}

		sync_point->Release();
		sync_point = nullptr;
		face_sync_point->Release();
		face_sync_point = nullptr;

		image->Release();
		image = nullptr;
		error = cur_video_stream->ReadStreamAsync(&image, &sync_point);

		assert(error >=PXC_STATUS_NO_ERROR);

		error = face_analysis->ProcessImageAsync(&image, &face_sync_point);
		
		assert(error >=PXC_STATUS_NO_ERROR);
		pxcU32 idx2 = 0;
		error = sync_point->SynchronizeEx(1u, &sync_point, &idx2, numeric_limits<pxcU32>::max());
		error = face_sync_point->SynchronizeEx(1u, &face_sync_point, &idx2, numeric_limits<pxcU32>::max());
		assert(error >= PXC_STATUS_NO_ERROR);
		
		error = image->AcquireAccess(PXCImage::ACCESS_READ, PXCImage::COLOR_FORMAT_RGB24, &image_data);

		
	}
	
}


void J_Camera_Manager::mouse_button_cmd(j_window_t i_window, int i_mouse_button
										, int i_action, int i_modifiers){
	
	s_contexts->make_context_active(M_view->get_context());
	switch(i_action){
	case GLFW_PRESS:
		//cerr << "\nKey Press Event Happened\n";
		M_view->mouse_button_press(M_view, i_mouse_button, i_modifiers
			, screen_coords(i_window, M_cursor_pos));
		break;
	case GLFW_RELEASE:
		//cerr << "\nKey Release Event Happened\n";
		//M_view->mouse_button_release(i_mouse_button, i_modifiers
		//	, screen_coords(i_window, M_cursor_pos));
		break;
	default:
		;
	}
}

void J_Camera_Manager::render_frame(int i_stream_index, const j_ubyte* i_image_buffer){
	assert(i_stream_index < M_stream_texture_ids.size());
	PXCCapture::VideoStream::ProfileInfo profile_info;
	pxcStatus error = M_video_streams[i_stream_index]->QueryProfile(&profile_info);
	assert(!error);
	assert(glIsTexture(M_stream_texture_ids[i_stream_index]));

	glUseProgram(M_image_shader->image_program_id());
	glBindVertexArray(M_full_window_box_vao_id);
	glBindTexture(GL_TEXTURE_2D, M_stream_texture_ids[i_stream_index]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, profile_info.imageInfo.width
		, profile_info.imageInfo.height, GL_BGR
		, GL_UNSIGNED_BYTE, i_image_buffer);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);	
}

void gen_full_box_vao(j_uint* ir_vao_id, j_uint* ir_buffer_id){

	glGenVertexArrays(1, ir_vao_id);
	glGenBuffers(1, ir_buffer_id);

	array<j_float,24> vertex_array_data = {
		-1.0f, -1.0f, 0.0f, 1.0f
		,1.0f , -1.0f, 0.0f, 1.0f
		, 1.0f, 1.0f, 0.0f, 1.0f
		,-1.0f, 1.0f, 0.0, 1.0f

		,0.0f, 1.0f
		,1.0f, 1.0f
		,1.0f, 0.0f
		,0.0f, 0.0f
	};



	glBindVertexArray(*ir_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, *ir_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array_data)
		, vertex_array_data.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0
		, reinterpret_cast<void*>(16*sizeof(GLfloat)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

void gen_full_box_reverse_texture_vao(j_uint* ir_vao_id, j_uint* ir_buffer_id){

	glGenVertexArrays(1, ir_vao_id);
	glGenBuffers(1, ir_buffer_id);

	array<j_float,24> vertex_array_data_reversed_texture = {
		-1.0f, -1.0f, 0.0f, 1.0f
		,1.0f , -1.0f, 0.0f, 1.0f
		, 1.0f, 1.0f, 0.0f, 1.0f
		,-1.0f, 1.0f, 0.0, 1.0f

		,0.0f, 0.0f
		,1.0f, 0.0f
		,1.0f, 1.0f
		,0.0f, 1.0f
	};


	glBindVertexArray(*ir_vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, *ir_buffer_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_array_data_reversed_texture)
		, vertex_array_data_reversed_texture.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 0
		, reinterpret_cast<void*>(16*sizeof(GLfloat)));
	
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}

//View Management---------------------------------------------------------------------------------------

/*J_View_Shared_t get_view()const*/
J_View_Shared_t J_Camera_Manager::get_view()const{return M_view;}

/*void bring_view_to_focus()const*/
void J_Camera_Manager::bring_view_to_focus(){
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	j_focus_context(M_context);
	init_device_list();
	
}


//Constructors---------------------------------------------------------------------------------------

static void set_gesture_box_settings(J_Text_Box_Shared_t );

static void set_menu_box_settings(J_Text_Box_Shared_t);

J_Camera_Manager::J_Camera_Manager(): M_view(new J_Camera_View(200,200, "J_Camera_Manager")){
	
	s_ui_model->attach_view(M_view);
	Context_RAII context_saver;
	M_context = M_view->get_context();
	M_view->make_active_context();
	j_clear_color(J_Color_RGBA<j_float>(0.00f,0.01f,0.1f,1.0f));
	j_clear();
	j_swap_buffers(M_context);
	M_image_shader = new Image_Shader_Program;
	PXCSession_Create(&M_pxc_session);
	M_log_font_face = s_font_manager->get_font_face(M_context, "times", 14);
	M_notification_font_face = s_font_manager->get_font_face(M_context, "times_italic", 14);
	auto camera_stream = J_Camera_Stream_Shared_t(
		new J_Camera_Stream(M_pxc_session, J_Rectangle(-0.4f, -0.9f, 1.0f, 1.0f)));

	M_camera_stream_id = camera_stream->get_ID();
	s_ui_model->add_camera_stream(camera_stream);
	M_view->add_image_pane(M_camera_stream_id);
	camera_stream->set_outline_color(J_Color::Red.alpha(0.3f));
	camera_stream->set_outline_visibility_status(true);
	camera_stream->set_fill_visibility_status(true);
	camera_stream->set_fill_color(J_Color::Red.alpha(0.2f));
	
	glfwSetCursorPosCallback(M_view->get_window(), camera_manager_cursor_pos_callback);
	glfwSetMouseButtonCallback(M_view->get_window(), camera_manager_mouse_button_callback);


	auto gesture_textbox_primary
		= J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle(-1.0f,-1.0f,1.0f,0.15f)));

	auto gesture_textbox_secondary 
		= J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle(0.0f,-1.0f,1.0f,0.15f)));

	auto gesture_data_box
		= J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle(-1.0f, -0.85f, 0.5f, 0.30f)));

	M_view->add_text_display(gesture_textbox_primary->get_ID());
	s_ui_model->add_text_box(gesture_textbox_primary);

	M_view->add_text_display(gesture_textbox_secondary->get_ID());
	s_ui_model->add_text_box(gesture_textbox_secondary);

	M_view->add_text_display(gesture_data_box->get_ID());
	s_ui_model->add_text_box(gesture_data_box);

	set_gesture_box_settings(gesture_textbox_primary);
	set_gesture_box_settings(gesture_textbox_secondary);
	set_menu_box_settings(gesture_data_box);

	gesture_textbox_primary->broadcast_current_state();
	gesture_textbox_secondary->broadcast_current_state();
	gesture_data_box->broadcast_current_state();


	M_gesture_fps = camera_stream->get_gesture_fps();
	
	M_gesture_primary_text_id = gesture_textbox_primary->get_ID();
	M_gesture_secondary_text_id = gesture_textbox_secondary->get_ID();
	M_gesture_fps_text_id = gesture_data_box->get_ID();
}

void set_gesture_box_settings(J_Text_Box_Shared_t i_gesture_box){
	i_gesture_box->set_colors(J_Color::Clear, J_Color::Yellow, J_Color::Black);
	i_gesture_box->set_outline_visibility_status(true);
	i_gesture_box->set_outline_thickness(2.0f);
	i_gesture_box->set_fill_visibility_status(true);
}

void set_menu_box_settings(J_Text_Box_Shared_t i_text_box){
	i_text_box->set_outline_visibility_status(true);
	i_text_box->set_fill_visibility_status(true);
	i_text_box->set_colors(J_Color::Clear, J_Color::Cyan.alpha(0.2f), J_Color::Cyan);
	i_text_box->set_clickable_status(true); 
}

void J_Camera_Manager::init_device_list(){
	clear_devices();
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);

	PXCSession::ImplDesc device_description = zero_set<PXCSession::ImplDesc>();
	device_description.group = PXCSession::IMPL_GROUP_SENSOR;
	
	device_description.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;
	int x_res = max(M_view->get_x_resolution(), 1500);
	int y_res = max(M_view->get_y_resolution(), 800);
	j_set_window_size(M_context, x_res, y_res);	

	pxcStatus error;
	for(int module_ix = 0;;++module_ix){
		PXCSession::ImplDesc capture_description;
		error = M_pxc_session->QueryImpl(&device_description, module_ix, &capture_description);

		if(error == PXC_STATUS_ITEM_UNAVAILABLE){
			break;
		}

		if(error < PXC_STATUS_NO_ERROR){
			continue;
		}

		PXCCapture* capture;

		error = M_pxc_session->CreateImpl<PXCCapture>(&capture_description, &capture);
		if(error < PXC_STATUS_NO_ERROR){
			continue;
		}

		find_devices_for_module(capture);
		capture->Release();
		break;
	}

}

void J_Camera_Manager::set_gesture_notification_text(PXC_GeoNode::Label i_label
													 , const string& irk_string){
	J_Text_Box_Shared_t text_box_ptr;
	switch(i_label){
	case PXC_GeoNode::LABEL_BODY_HAND_PRIMARY:
		text_box_ptr = s_ui_model->get_text_box(M_gesture_primary_text_id);
		break;
	case PXC_GeoNode::LABEL_BODY_HAND_SECONDARY:
		text_box_ptr = s_ui_model->get_text_box(M_gesture_secondary_text_id);
		break;
	default:
		cerr << "\nUnknown Body Part Retrieved";
		break;
	}
	if(!text_box_ptr){
		return;
	}
	Context_RAII context_saver;
	M_view->make_active_context();

	text_box_ptr->set_string(
		J_UI_String(irk_string, M_notification_font_face, J_Color::Black));
}


void J_Camera_Manager::clear_devices(){
	for(auto device_pair : M_device_ids_by_text_box_id){
		//s_ui_model->remove_text_box(device_pair.first);
		delete device_pair.second;
	}
	M_device_ids_by_text_box_id.clear();
}

void set_device_command(J_UI_Object_Shared_t i_obj, int,int, Pen_Pos_FL_t){
	s_camera_manager->set_device_from_text_box_id(i_obj->get_ID());
}



void J_Camera_Manager::set_device_from_text_box_id(j_uint i_obj_id){
	assert(M_device_ids_by_text_box_id.count(i_obj_id));

	s_ui_model->get_camera_stream(M_camera_stream_id)
		->set_device(M_device_ids_by_text_box_id[i_obj_id]);
}


void J_Camera_Manager::find_devices_for_module(PXCCapture* i_capture){
	pxcStatus error = PXC_STATUS_NO_ERROR;

	for(int i=0;i < MAX_DEVICES;i++){
		PXCCapture::DeviceInfo device_info = zero_set<PXCCapture::DeviceInfo>();
		PXCCapture::Device* device = nullptr;
		error = i_capture->CreateDevice(i, &device);

		if(error == PXC_STATUS_ITEM_UNAVAILABLE){
			break;
		}

		if(error < PXC_STATUS_NO_ERROR){
			if(device){
				device->Release();
			}
			continue;
		}
		//this_thread::sleep_for(chrono::milliseconds(50));
		wostringstream wostr;
		wostr << ' ' << M_device_ids_by_text_box_id.size() << ":\t";
		error = device->QueryDevice(&device_info);
		assert(!error);

		wstring detail_string(wostr.str());

		detail_string +=  device_info.name;

		
		j_float y_height = to_y_screen(M_view->get_window(), 50);
		J_Text_Box_Shared_t new_text_box 
			= J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle(-0.9f, 0.8f
			- M_device_ids_by_text_box_id.size()*y_height
			, to_x_screen(M_view->get_window(), 300), y_height)
			, J_UI_String(detail_string.c_str(), M_log_font_face, J_Color::White)));


		M_view->add_device_text_display(new_text_box->get_ID());
		s_ui_model->add_text_box(new_text_box);

		set_menu_box_settings(new_text_box);
		new_text_box->broadcast_current_state();

		assert(!M_device_ids_by_text_box_id.count(new_text_box->get_ID()));

		M_device_ids_by_text_box_id[new_text_box->get_ID()] = device;

		new_text_box->set_mouse_button_press_callback(set_device_command);
		cerr << string(detail_string.begin(), detail_string.end()) <<endl;
		if(1 == M_device_ids_by_text_box_id.size()){
			M_view->mouse_button_press(M_view, J_LEFT_MOUSE_BUTTON, 0
				, Pen_Pos_FL_t(new_text_box->x1()+new_text_box->width()/2
				, new_text_box->y1() - new_text_box->height()/2));
		}
	}

}
//Destruction---------------------------------------------------------------------------------------


void J_Camera_Manager::set_stream(int i_device_id){
	auto stream_obj = s_ui_model->get_camera_stream(M_camera_stream_id);
	stream_obj->set_device(M_device_ids_by_text_box_id[i_device_id]);
	stream_obj->set_gesture_detection_status(true, M_pxc_session);
	//stream_obj->start_stream();
	
	
	

	j_float ratio = static_cast<j_float>(M_view->get_x_resolution())
		/M_view->get_y_resolution();

	j_float width = 1.2f;
	j_float aspect_ratio = stream_obj->get_aspect_ratio();
	stream_obj->set_width(width);
	stream_obj->set_height(width*ratio*aspect_ratio);
	stream_obj->set_outline_thickness(3.0f);
}

J_Camera_Manager::~J_Camera_Manager(){
	destroy();
}

void J_Camera_Manager::destroy(){
	M_video_streams.apply([](PXCCapture::VideoStream* y_stream){y_stream->Release();});
	M_video_streams.clear();
	
	for(auto f_device : M_device_ids_by_text_box_id){
		f_device.second->Release();
	}
	M_video_streams.clear();
	M_device_ids_by_text_box_id.clear();

	if(M_pxc_session){
		M_pxc_session->Release();
		M_pxc_session = nullptr;
	}


	delete M_log_font_face;
	delete M_notification_font_face;
	delete M_image_shader;

	M_log_font_face = M_notification_font_face = nullptr;
	M_image_shader = nullptr;

	J_PXC_Error_Manager().clear();
}


void J_Camera_View::display_object_pressed(J_Display_Object_Shared_t i_obj, int i_action
										   , int i_modifiers, Pen_Pos_FL_t i_pos){
	if(!M_Mouse_press_commands.count(i_obj)){
		return;
	}

	(this->*M_Mouse_press_commands[i_obj])(i_obj, i_action, i_modifiers, i_pos);
}

void J_Camera_View::add_device_text_display(j_uint i_text_id){
	add_text_display(i_text_id);
	J_FT_Text_Display_Shared_t i_text_display = get_text_display(i_text_id);
	assert(i_text_display);
	assert(!M_Mouse_press_commands.count(i_text_display));

	M_Mouse_press_commands[i_text_display] = &J_Camera_View::select_stream;

}

void J_Camera_View::select_stream(J_Display_Object_Shared_t i_obj, int i_key
								  , int, Pen_Pos_FL_t){
	if(J_LEFT_MOUSE_BUTTON != i_key){
		return;
	}
	J_FT_Text_Display_Shared_t i_text_obj 
		= dynamic_pointer_cast<J_FT_Text_Display>(i_obj);

	assert(i_text_obj);
	s_camera_manager->set_stream(i_obj->get_object_ID());

}

/*
J_Cam_Capture::J_Cam_Capture(){
	pxcStatus error = PXCSession_Create(&M_pxc_session);

	glGenTextures(1, &M_image_texture_id);
	glBindTexture(GL_TEXTURE_2D, M_image_texture_id);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	M_image_shader = new Image_Shader_Program;
	assert(!error);	
	bool found = false;
	for(int i=0; A_HIGH_NUMBER;i++){

		PXCSession::ImplDesc description;
		memset(&description, 0, sizeof(description));
		description.subgroup = PXCSession::IMPL_SUBGROUP_VIDEO_CAPTURE;
		description.group = PXCSession::IMPL_GROUP_SENSOR;
		PXCSession::ImplDesc capture_description;
		pxcStatus error = M_pxc_session->QueryImpl(&description, i, &capture_description);
		if(error){
			cerr << "\n" << error << endl;
			throw J_Camera_Error("Capture Description Camera Error");
		}

		error = M_pxc_session->CreateImpl<PXCCapture>(&capture_description, &M_capture); 
		if(error == PXC_STATUS_ITEM_UNAVAILABLE){
			cerr << "\n" << error << endl;
			throw J_Camera_Error("Camera Did Not Work");
		}
		if(!error){
			found = true;
			break;
		}
	}
	if(!found){
		throw J_Camera_Error("No Camera Found");
	}
}
*/

void J_Camera_Manager::update(){
	static string fps_string("Fps: ");
	static int original_string_size = safe_int_cast(fps_string.size());
	j_dbl fps = s_ui_model->get_camera_stream(M_camera_stream_id)->get_gesture_fps();
	if(fps != M_gesture_fps){
		M_gesture_fps = fps;
		fps_string.resize(original_string_size);
		fps_string += number_to_string(M_gesture_fps);

		s_ui_model->get_text_box(M_gesture_fps_text_id)
			->set_string(J_UI_String(fps_string, M_log_font_face, J_Color::White));
	}

}

J_Camera_View::J_Camera_View(int i_width, int i_height, const char* i_title
							 , j_monitor_t i_monitor, j_window_t i_share_window)
		:J_View(i_width, i_height, i_title, i_monitor, i_share_window){}


}
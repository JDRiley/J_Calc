#include "J_Camera_Manager.h"
//
#include <J_UI/J_UI_Controller.h>
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
//
#include "J:\J_Camera_Socket_Interface\J_Camera_Socket_Interface\J_Camera_Socket_Interface\J_Camera_Socket_Model.h"
//Camera Utilities

#include "j_pxc_gesture.h"
//
#include "pxcsession.h"
//
#include <string>
//
#include <sstream>
//
#include <GLFW/glfw3.h>

using std::string; using std::wstring; using std::ostringstream;

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
using std::cerr; using std::endl; using std::wcerr;
using std::unique_ptr;
const int A_HIGH_NUMBER = 20000;
const int MAX_DEVICES = 50;
using std::fill;
using std::numeric_limits;
using std::max; using std::map;
using std::dynamic_pointer_cast;
namespace jomike{



static Instance_Pointer<J_Camera_Manager> s_camera_manager;
static Instance_Pointer<J_Camera_Socket_Model> s_camera_model;
static Instance_Pointer<Contexts_Handler> s_contexts;
static Instance_Pointer<J_Font_Manager> s_font_manager;
static Instance_Pointer<J_UI_Controller> s_controller;

J_Camera_Manager& J_Camera_Manager::get_instance(){
	static J_Camera_Manager camera_manager;
	return camera_manager;
}

//static void camera_manager_cursor_pos_callback(j_window_t i_window, j_dbl i_x_pos, j_dbl i_y_pos){
//	s_camera_manager->cursor_pos_input_cmd(i_window, i_x_pos, i_y_pos);
//
//}

static void camera_manager_window_close_callback(j_window_t ){
	s_camera_manager->close_window();
}
static void camera_manager_mouse_button_callback(j_window_t i_window, int i_mouse_button, int i_action, int i_modifiers){
	
	s_camera_manager->mouse_button_cmd(i_window, i_mouse_button, i_action, i_modifiers);
}

void set_device_command(J_UI_Object_Shared_t i_obj, int,int, Pen_Pos_FL_t){
	s_camera_manager->set_stream(i_obj->get_ID());
}



//Constructors---------------------------------------------------------------------------------------

static void set_gesture_box_settings(J_Text_Box_Shared_t );

static void set_menu_box_settings(J_Text_Box_Shared_t);

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



J_Camera_Manager::J_Camera_Manager() : M_view(new J_Camera_View(1500, 800, "J_Camera_Manager")){
	

	Context_RAII context_saver;




	PXCSession_Create(&M_pxc_session);
	M_log_font_face = s_font_manager->get_font_face(M_context, "times", 14);
	M_notification_font_face = s_font_manager->get_font_face(M_context, "times_italic", 14);
	auto camera_stream = J_Camera_Stream_Shared_t(
		new J_Camera_Stream(M_pxc_session, J_Rectangle(-0.4f, -0.6f, 1.2f, 1.2f)));

	M_camera_stream_id = camera_stream->get_ID();
	M_view->add_image_pane(M_camera_stream_id);
	s_camera_model->add_camera_stream(camera_stream);
	camera_stream->set_outline_color(M_initial_background_color);
	camera_stream->set_outline_visibility_status(true);
	
	j_float ratio = s_contexts->ratio();

	j_float width = 1.2f;
	j_float aspect_ratio = 1280.0f / 720.0f;
	camera_stream->set_width(width);
	camera_stream->set_height(width*ratio / aspect_ratio);
	camera_stream->set_outline_thickness(3.0f);


	//glfwSetCursorPosCallback(M_view->get_window(), camera_manager_cursor_pos_callback);
	glfwSetMouseButtonCallback(M_view->get_window(), camera_manager_mouse_button_callback);
	glfwSetWindowCloseCallback(M_view->get_window(), camera_manager_window_close_callback);




	auto gesture_textbox_primary
		= J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle(-1.0f,-1.0f,1.0f,0.15f)));

	auto gesture_textbox_secondary 
		= J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle(0.0f,-1.0f,1.0f,0.15f)));

	auto gesture_data_box
		= J_Text_Box_Shared_t(new J_Text_Box(J_Rectangle(-1.0f, -0.85f, 0.5f, 0.30f)));

	M_view->add_text_display(gesture_textbox_primary->get_ID());
	s_camera_model->add_text_box(gesture_textbox_primary);

	M_view->add_text_display(gesture_textbox_secondary->get_ID());
	s_camera_model->add_text_box(gesture_textbox_secondary);

	M_view->add_text_display(gesture_data_box->get_ID());
	s_camera_model->add_text_box(gesture_data_box);

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

	init_circles_above_gesture_notification();
	init_geo_circles();
	init_face_boxes();
}

void J_Camera_Manager::cursor_pos_input_cmd(j_window_t , j_dbl i_x_pos, j_dbl i_y_pos){
	//Context_RAII context_saver;
	M_cursor_pos.first = static_cast<int>(i_x_pos);
	M_cursor_pos.second = static_cast<int>(i_y_pos);

	M_view->set_cursor_pos(M_cursor_pos.first, M_cursor_pos.second);
}

//Notify---------------------------------------------------------------

void J_Camera_Manager::notify_gesture(PXC_Gesture_Data i_gesture){
	if (!i_gesture.body){
		return;
	}

	M_gestures.push_front(i_gesture);
	ostringstream o_str;
	wstring gesture_string = get_gesture_name(i_gesture.label);
	o_str << string(gesture_string.begin(), gesture_string.end()) << '!';

	o_str << " Active value" << (i_gesture.active ? " true" : " false")
		<< " Confidence: " << i_gesture.confidence;

	//cerr << '\n' << o_str.str();
	s_camera_manager->set_gesture_notification_text(i_gesture.body, o_str.str());
}

void J_Camera_Manager::notify_geonodes(PXC_GeoNode* i_primary_nodes
	, PXC_GeoNode* i_secondary_nodes){
	PXC_GeoNode* i_geonodes[2] = { i_primary_nodes, i_secondary_nodes };
	
	
	
	if (!M_view){
		return;
	}
	auto stream_obj = s_camera_model->get_camera_stream(M_camera_stream_id);

	for (int i = 0; i < NUM_GEO_SIDES; i++){
		for (int b = 0; b < NUM_NODES_PER_GEO_SIDE; b++){
			PXC_GeoNode& geo_node = i_geonodes[i][b];
			auto ui_circle = s_camera_model->get_ui_circle(M_geocircles[i][b]);
			assert(ui_circle);
			if (!geo_node.body){
				ui_circle->set_outline_visibility_status(false);
				ui_circle->set_fill_visibility_status(false);
				continue;
			}
			Pen_Pos_FL_t new_center = get_screen_coords_from_relative(stream_obj, geo_node);


			ui_circle->set_center(new_center.first, new_center.second);
			ui_circle->set_outline_visibility_status(true);
			ui_circle->set_fill_visibility_status(true);
			
			//ui_circle->set_radius(geo_node.radiusImage);
		}
	}
}


Pen_Pos_FL_t J_Camera_Manager::get_screen_coords_from_relative
	(J_Camera_Stream_Shared_t i_pane, PXC_GeoNode i_geo_node){
	Pen_Pos_FL_t image_pane_pixel_pos
		= i_pane->depth_pane_to_image_pane_pixel_pos(Pen_Pos_FL_t(i_geo_node.positionImage.x
		, i_geo_node.positionImage.y));
	auto window = M_view->get_window();



	image_pane_pixel_pos.first = image_pane_pixel_pos.first
		*(i_pane->width()/to_x_screen(window, static_cast<int>(i_pane->image_width())));

	image_pane_pixel_pos.second = image_pane_pixel_pos.second
		*(i_pane->height() / to_y_screen(window, static_cast<int>(i_pane->image_height())));

	int image_pane_x = static_cast<int>(image_pane_pixel_pos.first);
	int image_pane_y = static_cast<int>(image_pane_pixel_pos.second);
	
	Pen_Pos_FL_t screen_coordinates;
	screen_coordinates.first = i_pane->x1() + to_x_screen(window, image_pane_x);
	screen_coordinates.second = i_pane->y1() - to_y_screen(window, image_pane_y);

	return screen_coordinates;

}



void J_Camera_Manager::mouse_button_cmd(j_window_t i_window, int i_mouse_button
										, int i_action, int i_modifiers){
	
	s_contexts->make_context_active(M_view->get_context());
	switch(i_action){
	case J_PRESS:
		//cerr << "\nKey Press Event Happened\n";
		M_view->mouse_button_press(M_view, i_mouse_button, i_modifiers
			, screen_coords(i_window, M_cursor_pos));
		break;
	case J_RELEASE:
		//cerr << "\nKey Release Event Happened\n";
		//M_view->mouse_button_release(i_mouse_button, i_modifiers
		//	, screen_coords(i_window, M_cursor_pos));
		break;
	default:
		;
	}
}

PXC_Gesture_Data J_Camera_Manager::pop_gesture(){
	if (M_gestures.empty()){
		return zero_set<PXC_Gesture_Data>();
	}

	PXC_Gesture_Data gesture = M_gestures.back();
	M_gestures.pop_back();
	return gesture;
}

//View Management---------------------------------------------------------------------------------------

/*J_View_Shared_t get_view()const*/
jomike::J_Camera_View_Shared_t J_Camera_Manager::get_view()const{ return M_view; }

/*void bring_view_to_focus()const*/
void J_Camera_Manager::bring_view_to_focus(){
	Context_RAII context_saver;
	s_contexts->make_context_active(M_context);
	j_focus_context(M_context);
	M_view->clear_window();
	M_view->draw();
	
	
	init_device_list();
	start_background_ramp();
}




void J_Camera_Manager::close_window(){
	if (!M_view){
		return;
	}
	M_view->close_window();
	s_controller->remove_view(M_view->get_window());
	M_view.reset();
	M_context.reset();
}

void J_Camera_Manager::init_circles_above_gesture_notification(){
	J_Circle circle_shape;
	circle_shape.set_radius(0.03f);
	j_float radius = circle_shape.radius();
	
	auto gesture_text_box = s_camera_model->get_text_box(M_gesture_fps_text_id);
	circle_shape.set_x(gesture_text_box->x2() + 1.05f*radius);
	circle_shape.set_y(gesture_text_box->y2() + 1.05f*radius*s_contexts->ratio());
	
	int circles_added = 0;
	while ((circle_shape.x1() + radius) < 1.0f){
		J_UI_Circle_Shared_t new_circle(
			new J_UI_Circle(circle_shape));
		add_circle(new_circle);
		new_circle->set_fill_color(J_Color::Cyan.alpha(0.2f));
		new_circle->set_fill_visibility_status(true);
		new_circle->set_outline_color(J_Color::Cyan);
		new_circle->set_outline_visibility_status(true);
		new_circle->set_outline_thickness(1.0f);
		circle_shape.set_x(circle_shape.x1() + 2.10f*radius);
		++circles_added;
	}
	cerr << "\n**** " << circles_added << " Circles Added!";
}

void J_Camera_Manager::init_geo_circles(){
	J_Circle base_circle(0.0f, 0.0f, 0.013f);

	for (int i = 0; i < NUM_GEO_SIDES; i++){
		for (int b = 0; b < NUM_NODES_PER_GEO_SIDE; b++){
			J_UI_Circle_Shared_t new_circle(new J_UI_Circle(base_circle));
			add_circle(new_circle);
			new_circle->set_fill_color(J_BLACK.alpha(0.3f));
			new_circle->set_fill_visibility_status(false);
			new_circle->set_outline_color(J_WHITE);
			new_circle->set_outline_visibility_status(false);
			new_circle->set_outline_thickness(1.0f);

			M_geocircles[i][b] = new_circle->get_ID();
		}
	}
}

void J_Camera_Manager::init_face_boxes(){
	J_Rectangle rectangle(0.0f, 0.0f, 1.0f, 1.0f);
	for (int i = 0; i < MAX_FACES; i++){
		J_UI_Box_Shared_t new_ui_box(new J_UI_Box(rectangle));
		add_box(new_ui_box);
		new_ui_box->set_fill_visibility_status(false);
		new_ui_box->set_outline_visibility_status(false);
		new_ui_box->set_fill_color(J_BLACK.alpha(0.2f));
		new_ui_box->set_outline_color(J_BLUE);
		M_face_id_boxes[i] = new_ui_box->get_ID();
	}
}

void J_Camera_Manager::add_circle(J_UI_Circle_Shared_t i_circle_ptr){
	assert(M_view);
	M_view->add_display_circle(i_circle_ptr->get_ID());
	s_camera_model->add_ui_circle(i_circle_ptr);
	i_circle_ptr->broadcast_current_state();

	
}


void J_Camera_Manager::add_box(J_UI_Box_Shared_t i_box_ptr){
	assert(M_view);
	M_view->add_display_box(i_box_ptr->get_ID());
	s_camera_model->add_box(i_box_ptr);
	i_box_ptr->broadcast_current_state();
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
	s_camera_model->broadcast_current_state();
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
		text_box_ptr = s_camera_model->get_text_box(M_gesture_primary_text_id);
		break;
	case PXC_GeoNode::LABEL_BODY_HAND_SECONDARY:
		text_box_ptr = s_camera_model->get_text_box(M_gesture_secondary_text_id);
		break;
	default:
		cerr << "\nUnknown Body Part Retrieved";
		break;
	}
	if(!text_box_ptr){
		return;
	}

	if (!M_view){
		return;
	}
	Context_RAII context_saver;
	M_view->make_active_context();

	text_box_ptr->set_string(
		J_UI_String(irk_string, M_notification_font_face, J_Color::Black));
}


void J_Camera_Manager::clear_devices(){
	for(auto device_pair : M_device_ids_by_text_box_id){
		//s_camera_model->remove_text_box(device_pair.first);
		device_pair.second->Release();
	}
	M_device_ids_by_text_box_id.clear();
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
		s_camera_model->add_text_box(new_text_box);

		set_menu_box_settings(new_text_box);
		new_text_box->broadcast_current_state();

		assert(!M_device_ids_by_text_box_id.count(new_text_box->get_ID()));

		M_device_ids_by_text_box_id[new_text_box->get_ID()] = device;

		new_text_box->set_mouse_button_press_callback(set_device_command);
		wcerr << '\n' << detail_string <<endl;
		if(!s_camera_model->get_camera_stream(M_camera_stream_id)
			->gesture_detection_status()){
			M_view->mouse_button_press(M_view, J_LEFT_MOUSE_BUTTON, 0
				, Pen_Pos_FL_t(new_text_box->x1()+new_text_box->width()/2
				, new_text_box->y1() - new_text_box->height()/2));
		}
	}

}
//Destruction---------------------------------------------------------------------------------------


void J_Camera_Manager::set_stream(int i_device_id){
	auto stream_obj = s_camera_model->get_camera_stream(M_camera_stream_id);
	stream_obj->set_device(M_device_ids_by_text_box_id[i_device_id]);
	stream_obj->set_gesture_detection_status(true);
	stream_obj->set_face_detection_status(true);
	stream_obj->start_stream();
	
	
	

	j_float ratio = s_contexts->ratio();

	j_float width = 1.2f;
	j_float aspect_ratio = stream_obj->get_aspect_ratio();
	stream_obj->set_width(width);
	stream_obj->set_height(width*ratio/aspect_ratio);

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

void J_Camera_Manager::update(){
	update_fps_box();
	background_ramp();
}


void J_Camera_Manager::update_fps_box(){
	static string fps_string("Fps: ");
	static int original_string_size = safe_int_cast(fps_string.size());
	j_dbl fps = s_camera_model->get_camera_stream(M_camera_stream_id)->get_gesture_fps();
	if (fps != M_gesture_fps){
		M_gesture_fps = fps;
		fps_string.resize(original_string_size);
		fps_string += number_to_string(M_gesture_fps);

		s_camera_model->get_text_box(M_gesture_fps_text_id)
			->set_string(J_UI_String(fps_string, M_log_font_face, J_Color::White));
	}
}


void J_Camera_Manager::start_background_ramp(){
	M_background_ramping_status = true;
}



void J_Camera_Manager::background_ramp(){
	static const j_dbl sk_ramp_down_time = 1.0;
	static const j_dbl sk_ramp_up_time = 1.5;
	static const j_dbl sk_ramp_comptete_time = 2.5;
	static j_dbl s_background_ramp_start_time= 0.0;
	static bool s_was_ramping = false;

	if (!M_background_ramping_status){
		return;
	}


	if(!s_was_ramping && M_background_ramping_status){
		s_was_ramping = true;
		s_background_ramp_start_time = j_get_time();
	}

	j_dbl time = j_get_time() - s_background_ramp_start_time;

	if(time > sk_ramp_comptete_time){
		s_was_ramping = false;
		M_background_ramping_status = false;
		j_clear_color(M_background_color);
	}else if (time > sk_ramp_down_time){
		j_float ratio = static_cast<j_float>((time - sk_ramp_down_time)
			/ sk_ramp_up_time);
		j_clear_color((M_background_color*ratio).alpha(1.0f));
	}else{
		j_clear_color((M_initial_background_color
			*static_cast<j_float>(1.0f- time/sk_ramp_down_time)).alpha(1.0f));
	}
}

PXCSession& J_Camera_Manager::get_pxc_session(){
	return *(get_instance().M_pxc_session);
}

void J_Camera_Manager::notify_face_data(PXC_Face_Data i_face_data, int i_index){
	if (i_index > MAX_FACES){
		cerr << "\nToo many faces!";
		return;
	}
	auto ui_box = s_camera_model->get_ui_box(M_face_id_boxes[i_index]);
	if (!i_face_data.confidence){
		ui_box->set_fill_visibility_status(false);
		ui_box->set_outline_visibility_status(false);
		return;
	}

	if (!M_view){
		return;
	}

	auto stream_obj = s_camera_model->get_camera_stream(M_camera_stream_id);
	auto window = M_view->get_window();
	j_float ratio = stream_obj->width() / to_x_screen(window, stream_obj->image_width());
	j_float height = ratio*to_y_screen(window, i_face_data.rectangle.h);
	ui_box->set_x(stream_obj->x1() + ratio*to_x_screen(window, i_face_data.rectangle.x));
	ui_box->set_y(stream_obj->y1() - ratio*to_y_screen(window, i_face_data.rectangle.y) - height);
	ui_box->set_width(ratio*to_x_screen(window, i_face_data.rectangle.w));
	ui_box->set_height(height);
	ui_box->set_fill_visibility_status(true);
	ui_box->set_outline_visibility_status(true);

}

J_Camera_View::J_Camera_View(int i_width, int i_height, const char* i_title
							 , j_monitor_t i_monitor, j_window_t i_share_window)
		:J_View(i_width, i_height, i_title, i_monitor, i_share_window){
	Context_RAII context_saver;
	s_contexts->make_context_active(get_context());
	j_clear_color(J_RED*0.12f + J_BLUE*0.02f);
	
}


}
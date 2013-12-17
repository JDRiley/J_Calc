#ifndef J_PXC_GESTURE_H
#define J_PXC_GESTURE_H

//
#include "J_PXC_Fwd_Decl.h"

//
#include "J_PXC_Stream.h"
//
#include "pxcgesture.h"
//
#include "pxcmetadata.h"
//
#include "pxcprojection.h"

#include <string>
#include <array>

J_POINTER_DECL(PXCGesture, PXC_Gesture)
J_STRUCT_POINTER_DECL(PXCGesture::Alert, PXC_Alert)


namespace jomike{

J_FWD_DECL(J_UI_Circle)

typedef PXCSession::ImplDesc PXC_Impl_Description;
typedef PXCProjection* PXC_Projection;
typedef PXCGesture::GeoNode::Label PXC_Geo_Label;
typedef PXCGesture::GeoNode PXC_GeoNode;
typedef PXCGesture::Gesture PXC_Gesture_Data;
typedef PXCGesture::ProfileInfo PXC_Gesture_Profile_Info;
template<typename Num_t>
struct J_Vertex{
	Num_t x, y, z;
};

typedef J_Vertex<short> Depth_Vertex_t;



std::wstring get_body_name(PXC_Geo_Label);
std::wstring get_gesture_name(PXC_Geo_Label);
std::wstring get_side(PXC_Geo_Label);

class J_PXC_Gesture_Processor : public J_Stream_Processor{
public:
	typedef std::function<void(PXC_Gesture_Data)> Gesture_Callback_t;
	typedef std::function<void(PXC_GeoNode*, PXC_GeoNode*, bool mirror_status)> 
		GeoNode_Callback_t;

	J_PXC_Gesture_Processor(J_PXC_Stream_Shared_t i_stream);
	void push_data()override;

	void add_gesture_notification_callback(Gesture_Callback_t);
	void add_geonode_notification_callback(GeoNode_Callback_t);
	~J_PXC_Gesture_Processor();

private:
	PXC_Gesture M_gesture_module;
	PXC_Impl_Description M_gesture_description;
	PXC_Gesture_Profile_Info M_gesture_profile_info;

	typedef ex_array<PXC_Gesture_Data> Gesture_Cont_t;
	Gesture_Cont_t M_gesture_stack;
	ex_array<Gesture_Cont_t> M_gesture_history;

	void derived_process_image_async(PXC_Image*, PXC_SyncPoint*)override;
	void derived_finish_processing()override;
	void push_geonodes_data();

	///void poll_gestures()
	/*																			*/
	/*		Notify the manager of a gesture										*/
	/*																			*/
	void push_gesture_data();

	void process_gestures();

	void process_geonodes();

	ex_array<Gesture_Callback_t> M_gesture_callbacks;

	ex_array<GeoNode_Callback_t> M_geonode_callbacks;

	PXC_GeoNode M_node_data[NUM_GEO_SIDES][NUM_NODES_PER_GEO_SIDE];
};

}

#endif
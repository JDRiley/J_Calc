#ifndef J_PXC_PROCESSOR_H
#define J_PXC_PROCESSOR_H

#include "J_PXC_Fwd_Decl.h"
//
#include "j_pxc_face.h"
//
#include "J_PXC_Face_Module.h"
//
#include "J_Stream_Processor.h"
//
#include "J_PXC_Stream.h"
namespace jomike{

class J_PXC_Face_Processor : public J_Stream_Processor{
public:
	J_PXC_Face_Processor(J_PXC_Stream_Shared_t);
	typedef std::function<void(Face_Data_Cont_t
		, const Landmark_Data_Cont_t&, bool i_mirror_status)> Face_Data_Callback_t;
	void add_face_notification_callback(Face_Data_Callback_t);
	void push_data()override;
	~J_PXC_Face_Processor();
private:
	
	PXCFaceAnalysis::ProfileInfo M_face_profile_info;
	ex_array<Face_Data_Callback_t> M_face_data_callbacks;
	PXCCapture::VideoStream::DataDesc::StreamDesc M_face_input_description;
	PXCFaceAnalysis::Landmark::ProfileInfo M_landmark_profile;
	PXCFaceAnalysis::Detection::ProfileInfo M_face_detection_profile;
#ifdef VS_2013

	J_Face_Module* M_j_face_module = nullptr;
	PXC_Face_Module M_face_module = nullptr;
	/*Do not release this pointer. */
	/*See documentation on releasing from dynamic cast*/
	PXC_Face_Landmark M_landmark_module = nullptr;
	PXC_Face_Detection M_face_detection_module = nullptr;
#else
	J_Face_Module* M_j_face_module;
	PXC_Face_Module M_face_module;
	/*Do not release this pointer. */
	/*See documentation on releasing from dynamic cast*/
	PXC_Face_Landmark M_landmark_module;
	PXC_Face_Detection M_face_detection_module;
	void default_initialization();
#endif //VS_2013
	Face_Data_Cont_t M_face_data;
	void process_face_data();
	Landmark_Data_Cont_t M_landmark_data;

	void derived_finish_processing()override;
	void derived_process_image_async(PXC_Image*, PXC_SyncPoint*)override;
	void push_face_data()const;
};

}

#endif //J_PXC_PROCESSOR_H


#ifndef J_FACE_STREAM_PROCESSOR_H
#define J_FACE_STREAM_PROCESSOR_H

#include "J_Stream_Processor.h"
//


namespace jomike{
namespace J_PXC_Face{

struct J_Face_Data;

}

class J_Face_Module;

class J_Face_Stream_Processor : public J_Stream_Processor{
public:
	J_Face_Stream_Processor(J_PXC_Stream_Shared_t i_stream);
	~J_Face_Stream_Processor();
	void push_data()override;
private:
	J_Face_Module* M_j_face_module; 
	J_PXC_Face::J_Face_Data* M_face_data;
	void derived_process_image_async(PXC_Image*, PXC_SyncPoint*)override;
	void derived_finish_processing()override;
};

}

#endif //J_FACE_STREAM_PROCESSOR_H


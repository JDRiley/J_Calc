#include "J_Stream_Processor.h"


namespace jomike{

void J_Stream_Processor::process_image_async(PXC_Image* i_image, PXC_SyncPoint* i_syncpoint){
	assert(!M_processing_status);
	M_processing_status = true;
	derived_process_image_async(i_image, i_syncpoint);
}

void J_Stream_Processor::finish_processing(){
	assert(M_processing_status);
	M_processing_status = false;
	derived_finish_processing();
}

bool J_Stream_Processor::processing_status()const{
	return M_processing_status;
}

const J_PXC_Stream& J_Stream_Processor::get_src_stream()const{
	assert(!M_src_stream.expired()); 
	return *(M_src_stream.lock());
}

void J_Stream_Processor::set_parent(J_PXC_Stream_Shared_t i_stream){
	M_src_stream = i_stream;
}

J_Stream_Processor::~J_Stream_Processor(){

}

void J_Stream_Processor::alert_mirror_status_change(){

}

void J_Stream_Processor::set_mirror_status(bool i_status){
	if(M_mirror_status == i_status){
		return;
	}

	M_mirror_status = i_status;
	alert_mirror_status_change();
}

bool J_Stream_Processor::mirror_status()const{
	return M_mirror_status;
}
#ifndef VS_2013
void J_Stream_Processor::default_initialization(){
	M_processing_status = M_mirror_status = false;
}


J_Stream_Processor::J_Stream_Processor(){

	default_initialization();

}
#endif //!VS_2013


}




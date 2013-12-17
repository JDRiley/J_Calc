#include "J_PXC_Stream.h"
//
#include <J_Utile.h>
//
//

#ifdef _DEBUG
#include <iostream>

using std::cerr;
#endif
using std::dynamic_pointer_cast;

namespace jomike{

static J_PXC_Error_Manager s_pxc_error;



J_PXC_Stream::J_PXC_Stream(PXC_Video_Stream i_stream, J_PXC_Device_Shared_t i_device):M_sync_points(MAX_STREAM_PROCESSES){
	M_device = i_device;
	M_image_stream = i_stream;
	assert(M_image_stream);

	set_to_zero(&M_stream_desc);
	M_image_stream->QueryStream(&M_stream_info);
	M_image_stream->QueryProfile(&M_stream_profile);
	M_stream_desc.sizeMin.width = M_stream_profile.imageInfo.width;
	M_stream_desc.sizeMin.height = M_stream_profile.imageInfo.height;
	M_stream_desc.sizeMax.width = M_stream_profile.imageInfo.width;
	M_stream_desc.sizeMax.height = M_stream_profile.imageInfo.height;
	M_stream_desc.format = M_stream_profile.imageInfo.format;

	init_syncpoints();

}

void J_PXC_Stream::init_syncpoints(){
	int size = M_sync_points.QuerySize();
	for(int i = 0; i < size; i++){
		M_sync_points[i] = nullptr;
	}
}

void J_PXC_Stream::add_stream_processor(J_Stream_Processor_Shared_t i_stream_process){
	assert(!M_stream_processes_by_ptr.count(i_stream_process));
	M_stream_processes.push_back(i_stream_process);
	M_stream_processes_by_ptr.insert(i_stream_process);
	i_stream_process->set_parent(dynamic_pointer_cast<J_PXC_Stream>(shared_from_this()));
}

void J_PXC_Stream::start_stream_processing(){
	const j_size_t num_process = M_stream_processes.size();
	assert(M_stream_processes_by_ptr.size() == safe_uns_cast(num_process));
	for(int i = 0; i < num_process; i++){
		assert(!M_sync_points[i]);
		M_stream_processes[i]->process_image_async(&M_image, &M_sync_points[i]);
	}

}

bool J_PXC_Stream::synchronize_processes(j_uint i_timeout /*= std::numeric_limits<j_uint>::max()*/){
	assert(processing_status());




	s_pxc_error = M_sync_points.SynchronizeEx(nullptr, i_timeout);
	
	if(pxc_error()){
		pxc_clear_error();
		return false;
	}

	for(int i = 0; i < M_stream_processes_by_ptr.size(); i++){
		auto& sync_point = M_sync_points[i];
		if(!sync_point){ 
			continue; 
		}
		s_pxc_error = sync_point->Synchronize(i_timeout);
		assert(PXC_STATUS_EXEC_ABORTED != pxc_status());
		if(PXC_STATUS_EXEC_INPROGRESS == pxc_status()){
			pxc_clear_error();
			return false;
		}
	}


	return true;
}

void J_PXC_Stream::derived_process_image_async(PXC_Image* i_image
	, PXC_SyncPoint* i_syncpoint){
	s_pxc_error = M_image_stream->ReadStreamAsync(i_image, i_syncpoint);

	NO_PXC_ERROR;
}



J_PXC_Device& J_PXC_Stream::device()const{
	return *M_device;
}

void J_PXC_Stream::finish_stream_processing(){
	assert(ready_for_processing());
	for(auto& processor : M_stream_processes){
		processor->finish_processing();
	}

	

	M_sync_points.ReleaseRefs();
	J_PXC_RELEASE(M_image);
}

int J_PXC_Stream::width()const{
	return M_stream_profile.imageInfo.width;
}

int J_PXC_Stream::height()const{
	return M_stream_profile.imageInfo.height;
}

Image_Format convert_format(PXCImage::ColorFormat i_format){
	switch(i_format){
	case PXCImage::COLOR_FORMAT_RGB24:
		return Image_Format::RGB24_UBYTE;
	case PXCImage::COLOR_FORMAT_RGB32:
		return Image_Format::RGBA32_UBYTE;
	case PXCImage::COLOR_FORMAT_NV12:
		return Image_Format::NV12;
	case PXCImage::COLOR_FORMAT_DEPTH:
		return Image_Format::DEPTH16;
	case PXCImage::COLOR_FORMAT_VERTICES:
		return Image_Format::DEPTH_VERTICES;
	case PXCImage::COLOR_FORMAT_YUY2:
		return Image_Format::YUY2;
	default:
			assert(!"Undefined Image Format");
	}
	return (Image_Format)0;
}

Image_Format J_PXC_Stream::format()const{
	return convert_format(M_stream_profile.imageInfo.format);
}

void J_PXC_Stream::derived_finish_processing(){

}

void J_PXC_Stream::init(){
	assert(M_stream_processes.empty());
	assert(M_stream_processes_by_ptr.empty());

	add_stream_processor(shared_from_this());

}

J_PXC_Stream::~J_PXC_Stream(){
	synchronize_processes(20000u);
	M_sync_points.ReleaseRefs();
	J_PXC_RELEASE(M_image);
	J_PXC_RELEASE(M_image_stream);
	clear_stream_processes();
}

bool J_PXC_Stream::ready_for_processing(){
	return !processing_status() || synchronize_processes(0u);
}

void J_PXC_Stream::push_stream_data(){
	for(auto& processor : M_stream_processes){
		processor->push_data();
	}
}

J_PXC_Stream_Shared_t J_PXC_Stream::make_shared_ptr(){
	return J_PXC_Stream_Shared_t(this);
}

void J_PXC_Stream::push_data(){
	//does nothing
}

void J_PXC_Stream::set_stream_mirror_status(bool i_status){
	for(auto& stream_process : M_stream_processes){
		stream_process->set_mirror_status(i_status);
	}
}

void J_PXC_Stream::clear_stream_processes(){
	M_stream_processes.clear()
		; M_stream_processes_by_ptr.clear();
}





}
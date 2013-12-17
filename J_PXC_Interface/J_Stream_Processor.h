#ifndef J_STREAM_PROCESSOR_H
#define J_STREAM_PROCESSOR_H

#include "J_PXC_Fwd_Decl.h"
//
#include "pxccapture.h"
//
#include "pxcsmartptr.h"
//
#include <J_Image.h>
//

namespace jomike{

typedef PXCCapture::VideoStream::ProfileInfo PXC_Video_Stream_Profile;
typedef PXCCapture::Device::StreamInfo PXC_Stream_Info;
typedef PXCCapture::VideoStream::DataDesc::StreamDesc PXC_Stream_Desc;
typedef PXCCapture::VideoStream* PXC_Video_Stream;
typedef PXCScheduler::SyncPoint* PXC_SyncPoint;

class J_Stream_Processor{
public:
	
	void process_image_async(PXC_Image* i_image, PXC_SyncPoint* i_syncpoint);
	void finish_processing();
	void set_parent(J_PXC_Stream_Shared_t i_stream);
	virtual void push_data() = 0;
	virtual ~J_Stream_Processor();
#if VS_2013
	J_Stream_Processor() = default;
	J_Stream_Processor(const J_Stream_Processor&) = delete;
#else
	J_Stream_Processor();
#endif
	void set_mirror_status(bool i_status);
	
protected:
	const J_PXC_Stream& get_src_stream()const;
	bool processing_status()const;
	
	bool mirror_status()const;
private:
	virtual void alert_mirror_status_change();
	virtual void derived_process_image_async(PXC_Image*, PXC_SyncPoint*) = 0;
	virtual void derived_finish_processing() = 0;
	J_PXC_Stream_Weak_t M_src_stream;
#ifdef VS_2013
	bool M_processing_status = false;
	bool M_mirror_status = false;
#else
	bool M_processing_status;
	bool M_mirror_status;
	void default_initialization();
	J_Stream_Processor(const J_Stream_Processor&);
#endif

};




}

#endif //J_STREAM_PROCESSOR_H


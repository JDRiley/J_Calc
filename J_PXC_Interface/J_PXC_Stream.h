#ifndef J_PXC_STREAM_H
#define J_PXC_STREAM_H

#include "J_PXC_Fwd_Decl.h"

//
#include <RB_Tree.h>
//
#include <ex_array.h>
//
#include <limits>
#include "J_Stream_Processor.h"
namespace jomike{



const int MAX_STREAM_PROCESSES = 10;


class J_PXC_Stream : private J_Stream_Processor
	, public std::enable_shared_from_this<J_PXC_Stream>{
public:
	J_PXC_Stream(PXC_Video_Stream i_stream, J_PXC_Device_Shared_t i_device);
	J_PXC_Stream_Shared_t make_shared_ptr();
	int width()const;
	int height()const;
	Image_Format format()const;;
	void add_stream_processor(J_Stream_Processor_Shared_t i_stream_process);
	void clear_stream_processes();
	void start_stream_processing();
	void push_stream_data();
	void set_stream_mirror_status(bool i_status);
	bool synchronize_processes(j_uint i_timeout = std::numeric_limits<j_uint>::max());
	void init_syncpoints();
	void finish_stream_processing();
	J_PXC_Device& device()const;
	bool ready_for_processing();
	void init();

#ifdef VS_2013
	J_PXC_Stream(const J_PXC_Stream&) = delete;
	J_PXC_Stream& operator=(const J_PXC_Stream&) = delete;
#endif // VS_2013
	~J_PXC_Stream();
protected:
	
private:
	PXC_Image M_image;
	PXCSmartSPArray M_sync_points;
	PXC_Video_Stream M_image_stream;
	PXC_Stream_Desc M_stream_desc;
	PXC_Stream_Info M_stream_info;
	PXC_Video_Stream_Profile M_stream_profile;
	RB_Tree<J_Stream_Processor_Shared_t> M_stream_processes_by_ptr;
	ex_array<J_Stream_Processor_Shared_t> M_stream_processes;
	J_PXC_Device_Shared_t M_device;
	void derived_process_image_async(PXC_Image*, PXC_SyncPoint*)override;
	void derived_finish_processing()override;
	friend J_Stream_Processor_Shared_t;
	friend std::_Ptr_base<J_Stream_Processor>;
	void push_data()override;
#ifdef VS_2013

#else
	J_PXC_Stream(const J_PXC_Stream&);
	J_PXC_Stream& operator=(const J_PXC_Stream&);
#endif

};

}

#endif //J_PXC_STREAM_H


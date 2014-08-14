#ifndef J_FORWARD_DECLARATIONS
#define J_FORWARD_DECLARATIONS

#include <cassert>

#ifdef WIN32


#define WIN32_LEAN_AND_MEAN

#include <vld\vld.h> 


#undef ERROR
#undef max
#undef min
#else
static_assert(0, "No VLD Found");
#endif

#include <memory>
#include <utility>


#ifdef _M_X64

#ifdef _DEBUG
#define J_LIB_APPEND "_D.lib"
#else
#define J_LIB_APPEND ".lib"
#endif

#else

#ifdef _DEBUG
#define J_LIB_APPEND "32_D.lib"
#else
#define J_LIB_APPEND "32.lib"
#endif

#endif




namespace jomike{}

namespace jtl = jomike;

namespace jomike{

#if _M_IX86
typedef int j_size_t;

#elif _M_X64

typedef long long j_size_t;
#endif



typedef float j_float;
typedef unsigned j_uint;
typedef unsigned long j_ulint;
typedef unsigned char j_ubyte;
typedef double j_dbl;
typedef double Dbl_t;
typedef int32_t j_int;
typedef int16_t j_short;
typedef long long j_llint;

const j_size_t J_SIZE_T_ZERO = 0;
const j_int J_INT_ZERO = 0;
const j_uint J_UINT_ZERO = 0;
const j_dbl J_DBL_ZERO = 0;
const j_float J_FLOAT_ZERO = 0;
const j_ubyte J_UBYTE_ZERO = 0;
const j_short J_SHORT_ZERO = 0;

typedef std::pair<int,int> Pen_Pos_t;

typedef std::pair<j_float, j_float> Pen_Pos_FL_t;

#define J_STRUCT_FWD_DECL(class_name) struct class_name;\
	typedef std::shared_ptr<class_name> class_name##_Shared_t; \
	typedef std::weak_ptr<class_name> class_name##_Weak_t; \
	typedef std::unique_ptr<class_name> class_name##_Unique_t;

#define J_FWD_DECL(class_name) class class_name;\
	typedef std::shared_ptr<class_name> class_name##_Shared_t; \
	typedef std::weak_ptr<class_name> class_name##_Weak_t; \
	typedef std::unique_ptr<class_name> class_name##_Unique_t;

#define J_FWD_DECL_LOWER(class_name) class class_name;\
	typedef std::shared_ptr<class_name> class_name##_shared_t; \
	typedef std::weak_ptr<class_name> class_name##_weak_t; \
	typedef std::unique_ptr<class_name> class_name##_unique_t;


#define J_POINTER_DECL(class_name, ptr_typedef_name) class class_name;\
	 typedef class_name* ptr_typedef_name; 

#define J_STRUCT_POINTER_DECL(class_name, ptr_typedef_name) struct class_name;\
	  typedef class_name* ptr_typedef_name; 


//creates a lock_guard for a given mutex
#define J_LOCK(lock) std::lock_guard<decltype(lock)> MD_LOCK_GUARD(lock)

J_FWD_DECL(J_Test_Suite)
}





#endif
#ifndef EX_ARRAY_CONTENT_MANAGER_H
#define EX_ARRAY_CONTENT_MANAGER_H

#include "ex_array.h"


namespace jomike{

template<typename St, typename Creater_t, typename Copier_t, typename Deleter_t
	, typename Alloc_t = std::allocator<St>>
class ex_array_content_manager : public ex_array<St, Alloc_t>{
public:
	ex_array_content_manager(const ex_array_content_manager&);
	ex_array_content_manager(ex_array_content_manager&&);
	ex_array_content_manager();

private:
	typedef ex_array<St, Alloc_t> Base_t;

	Creater_t M_creator;
	Copier_t M_copier;
	Deleter_t M_deleter;
};

namespace ex_array_utile{
template < typename St>
class no_default_creater_class{
public:
	St operator()();
};

}


}

#endif //EX_ARRAY_CONTENT_MANAGER_H


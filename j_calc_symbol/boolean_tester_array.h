#ifndef BOOLEAN_TESTER_ARRAY_H
#define BOOLEAN_TESTER_ARRAY_H



#include <ex_array.h>


namespace jomike{
class Boolean_Tester;
class Boolean_Tester_Allocator{
public:
	Boolean_Tester** allocate(j_size_t);
	void deallocate(Boolean_Tester**, ptrdiff_t);

	void construct(Boolean_Tester** , Boolean_Tester*);

	void destroy(Boolean_Tester** );
};


typedef ex_array<Boolean_Tester*, Boolean_Tester_Allocator>
		boolean_tester_array;

}

#endif //BOOLEAN_TESTER_ARRAY_H


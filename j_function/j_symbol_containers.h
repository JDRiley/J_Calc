#ifndef J_SYMBOL_ARRAY_H
#define J_SYMBOL_ARRAY_H

#include "J_Symbol_Fwd_Decl.h"
//
#include <ex_array.h>
//
#include <map>
//
#include <J_UI/J_UI_String.h>
//
#include <RB_Tree.h>

namespace jomike{



class j_symbol_allocator{
public:
	j_symbol** allocate(j_size_t);
	void deallocate(j_symbol**, ptrdiff_t);
	void construct(j_symbol** p, j_symbol*);
	void destroy(j_symbol**);
};


typedef RB_Tree<j_symbol*, J_UI_String
	, std::less<j_symbol*>, j_symbol_allocator> j_symbol_tree;

typedef ex_array<j_symbol*, j_symbol_allocator> j_symbol_array;

typedef std::map<J_UI_String, j_symbol*
	, std::less<J_UI_String>, j_symbol_allocator> j_symbol_map;

}
#endif //J_SYMBOL_ARRAY_H


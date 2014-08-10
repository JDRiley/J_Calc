#include "j_cursor_management.h"

#include <map>
#include <Windows.h>
//
#include <WinUser.h>

//
#include <windef.h>

//
#include <cassert>
//
#include <J_Utile.h>
namespace jomike{
	
HANDLE get_cursor_handle(int i_ordinal){
	return LoadImage(NULL, MAKEINTRESOURCE(i_ordinal), IMAGE_CURSOR
					 , 0, 0, LR_DEFAULTSIZE);
}

const std::map<int, HANDLE> sk_cursors = {
	{J_NO_CURSOR_ID, HANDLE()}
	, {J_NORMAL_CURSOR_ID, get_cursor_handle(OCR_NORMAL) }
	, {J_I_BEAM_CURSOR_ID, get_cursor_handle(OCR_IBEAM)}
};

void j_set_cursor_type(int i_cursor_id){
	if(j_true){
		return;
	}
	auto cursor_it = sk_cursors.find(i_cursor_id);
	assert(sk_cursors.end() != cursor_it);
	SetCursor(static_cast<HCURSOR>(cursor_it->second));
}



}




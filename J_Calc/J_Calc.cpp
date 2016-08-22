#include <J_Fwd_Decl.h>

#include "J_Calc_Controller.h"
//
#include <J_Error.h>


#ifdef WIN32

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif //WIN32

namespace jtl = jomike;
#ifdef RELEASE_BUILD

int wWinMain(HINSTANCE , HINSTANCE , PTSTR  , int ){
	int argc = 0;
	char ** argv = nullptr;
#else
int main(int argc, char** argv){
#endif
	(void)argc;
	(void)argv;

	if(jtl::j_true){
		//return 0;
	}
	jtl::Instance_Pointer<jtl::J_Calc_Controller> controller;
	try{
		controller->construct_ui();
	controller->execute();
	}catch(jtl::J_Error<jtl::jc_string_t::value_type>& error){
		error.print();
#ifdef WIN32
  		MessageBoxW(0,error.message().c_str(), L"Error", MB_OK);
#endif
		return 1;
	}

	return 0;
}

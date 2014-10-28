#include <J_Fwd_Decl.h>

#include "J_Calc_Controller.h"
//
#include <J_Error.h>


#ifdef WIN32

#define WINDOWS_LEAN_AND_MEAN
#include <Windows.h>
#endif //WIN32

namespace jtl = jomike;
#ifdef soijs //RELEASE_BUILD

int wWinMain(HINSTANCE , HINSTANCE , PTSTR  , int ){
	int argc = 0;
	char ** argv = nullptr;
#else
int main(int argc, char** argv){
#endif

	if(jtl::j_true){
		//return 0;
	}
	jtl::Instance_Pointer<jtl::J_Calc_Controller> controller;
	try{
	controller->init(argc, argv);
	controller->execute();
	}catch(jtl::J_Error& error){
		error.print();
#ifdef WIN32
  		MessageBoxA(0,error.message(), "Error", MB_OK);
#endif
		return 1;
	}

	return 0;
}

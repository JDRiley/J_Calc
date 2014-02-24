#include <J_Fwd_Decl.h>

#include "J_Calc_Controller.h"
#include <J_Error.h>



namespace jtl = jomike;
#if 0//RELEASE_BUILD

int wWinMain(HINSTANCE , HINSTANCE , PTSTR  , int ){
	int argc = 0;
	char ** argv = nullptr;
#else
int main(int argc, char** argv){
#endif

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
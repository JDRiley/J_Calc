#include <J_Fwd_Decl.h>

#include "J_Calc_Controller.h"
//
#include <J_Error.h>
//
#include <J_Test_Suite.h>


namespace jtl = jomike;
#ifdef RELEASE_BUILD

int wWinMain(HINSTANCE , HINSTANCE , PTSTR  , int ){
	int argc = 0;
	char ** argv = nullptr;
#else
int main(int argc, char** argv){
#endif

	jtl::J_Test_Suite().test_ui_string();
	jtl::J_Test_Suite().test_ui_multi_string();
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

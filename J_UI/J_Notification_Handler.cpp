#include "J_Notification_Handler.h"


namespace jomike{

static j_uint s_handler_id = 0;

J_UI_Handler::J_UI_Handler():M_ID(++s_handler_id){}

j_uint J_UI_Handler::get_ID(){
	return M_ID;
}

}




#ifndef J_NOTIFICATION_HANDLER_H
#define J_NOTIFICATION_HANDLER_H

#include <J_Fwd_Decl.h>


namespace jomike{

class J_UI_Handler{
public:
	J_UI_Handler();
	j_uint get_ID();
	
private:
	const j_uint M_ID;
};

class J_Update_Handler{
public:
	virtual void update() =0;
};
}

#endif //J_NOTIFICATION_HANDLER_H


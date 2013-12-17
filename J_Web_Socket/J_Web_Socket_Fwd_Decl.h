#ifndef J_WEB_SOCKET_FWD_DECL_H
#define J_WEB_SOCKET_FWD_DECL_H

#include <J_Fwd_Decl.h>

#pragma comment(lib, "J_Web_Socket"J_LIB_APPEND)

namespace websocketpp{
	typedef std::weak_ptr<void> connection_hdl;
}

namespace jomike{
enum class Connection_Alert;
J_FWD_DECL(J_Socket_Server)
J_FWD_DECL(Gesture_Socket_Handler)
}

#endif //J_WEB_SOCKET_FWD_DECL_H


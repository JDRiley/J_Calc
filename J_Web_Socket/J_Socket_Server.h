#ifndef J_SOCKET_H
#define J_SOCKET_H


//
#include "J_Web_Socket_Fwd_Decl.h"
//
#include <string>
//
#include <ex_array.h>
//
#include <j_pxc_gesture.h>
//
#include <RB_Tree.h>
//
#include <websocketpp/server.hpp>
//
#include <websocketpp/config/asio_no_tls.hpp>




namespace websocketpp{
	template<typename config>
	class server;
}

namespace web{
	namespace json{ class value; }
}

namespace jomike{
typedef web::json::value json_val;

J_FWD_DECL(J_Socket_Server)

json_val get_json_data(PXC_Gesture_Data);

std::string get_json_data_string(PXC_Gesture_Data);
enum class Connection_Alert{ OPEN, CLOSE };


class J_Socket_Server{
public:
	J_Socket_Server(uint16_t port_num);
	void poll_events();
	//Can only be called from same thread
	void queue_json_data(const json_val&);
	void queue_string(const std::string&);
	void send_data();
	void launch_server();
	void close_server();



	typedef std::function<void(websocketpp::connection_hdl
		, const std::string, Connection_Alert)> 
		Connection_Callback_t;

	void add_open_close_connection_callback(const Connection_Callback_t&);
	~J_Socket_Server();

	typedef PairIterator<std::map<websocketpp::connection_hdl, std::string,
		 std::owner_less<websocketpp::connection_hdl >> ::const_iterator>
			connection_name_iter;
	
	void close_connection(websocketpp::connection_hdl);

	connection_name_iter connection_names_begin()const;
	connection_name_iter connection_names_end()const;
	std::string ip_addresses_string();

private:
	ex_array<Connection_Callback_t> M_connection_callbacks;

	uint16_t M_port_number;
#ifdef VS_2013
	bool M_launched_flag = false;
#else
	bool M_launched_flag;
	void default_initialization();
#endif // VS_2013

	


	typedef RB_Tree<websocketpp::connection_hdl, websocketpp::connection_hdl
		, std::owner_less<websocketpp::connection_hdl>> M_Connection_Cont_t;
	
	M_Connection_Cont_t M_connections;

	std::map<websocketpp::connection_hdl, std::string
		, std::owner_less<websocketpp::connection_hdl>> M_connection_names;

	typedef websocketpp::server<websocketpp::config::asio> Server_t;
	
	typedef ex_array<std::string> Message_Cont_t;
	Message_Cont_t M_pending_messages;
	void send_message(const std::string&);
	Server_t M_server;

	
	void on_open(M_Connection_Cont_t::value_type);
	void on_close(M_Connection_Cont_t::value_type);
	void on_message(M_Connection_Cont_t::value_type, Server_t::message_ptr);
	void run_server();
	void terminate(M_Connection_Cont_t::value_type);
	void terminate_message(M_Connection_Cont_t::value_type, Server_t::message_ptr);
};


}

#endif
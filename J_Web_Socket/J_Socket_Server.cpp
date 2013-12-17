#include "J_Socket_Server.h"
//
#include <cpprest/json.h>
//
#include <J_Error.h>
//
#include <iostream>
//
#include <J_String.h>

using std::wcerr;
using std::cerr;
using std::bind; using namespace std::placeholders;
using std::cout; using std::endl; using std::wcout;
using std::for_each; using std::ostringstream;
namespace json = web::json;
using std::wstring; using std::wcerr;
using std::pair; using std::make_pair;
namespace opcode = websocketpp::frame::opcode;
using std::string;

#define SOCKET_PRINTING 1

namespace jomike{

const j_dbl K_SYNC_TIME = 1.0 /25.0;

string J_Socket_Server::ip_addresses_string(){
	string ac;
	ac.resize(80);
	ostringstream o_str;
	if(gethostname(&ac[0], static_cast<int>(ac.size()*sizeof(string::value_type))) == SOCKET_ERROR) {
		cerr << "Error " << WSAGetLastError() <<
			" when getting local host name." << endl;
		return string();
	}
	cout << "Host name is " << ac << "." << endl;

	struct hostent * hostname = gethostbyname(ac.data());
	if(!hostname) {
		cerr << "Yow! Bad host lookup." << endl;
		return string();
	}

	for(int i = 0; hostname->h_addr_list[i]; ++i) {
		struct in_addr addr;
		memcpy(&addr, hostname->h_addr_list[i], sizeof(struct in_addr));
		o_str << "Address " << i << ": " << inet_ntoa(addr) << endl;

	}
	o_str << "At Port: " << M_port_number;
	return o_str.str();
}
json_val get_json_data(PXC_Gesture_Data i_gesture){
	json_val j_val;
	j_val[L"side"] = json_val::string(get_side(i_gesture.body));
	j_val[L"group"] = json_val::string(L"arm");
	j_val[L"body"] = json::value::string(get_body_name(i_gesture.body));
	j_val[L"gesture"] = json::value::string(get_gesture_name(i_gesture.label));
	//auto position_vector = json::value::element_vector();
	//position_vector.push_back(make_pair(, json::value::number(1)));
	//position_vector.push_back(make_pair(json::value::number(1), json::value::number(1)));
	//j_val[L"position"] = json::value::array(position_vector);
	j_val[L"gesture_active"] = json::value::boolean(i_gesture.active);
	j_val[L"found"] = json::value::boolean(true);
	j_val[L"x_pos"] = json::value::number(1);
	j_val[L"y_pos"] = json::value::number(1);
	return j_val;
}

string get_json_data_string(PXC_Gesture_Data i_gesture){
	auto json_string =  get_json_data(i_gesture).to_string();
	return string(json_string.begin(), json_string.end());
}

J_Socket_Server::J_Socket_Server(uint16_t i_port_number) :M_port_number(i_port_number){
#ifndef VS_2013
	default_initialization();
#endif //!VS_2013

	M_server.init_asio();

	// Register handler callbacks
	M_server.set_open_handler(bind(&J_Socket_Server::on_open, this, _1));
	M_server.set_close_handler(bind(&J_Socket_Server::on_close, this, _1));
	M_server.set_message_handler(bind(&J_Socket_Server::on_message, this, _1, _2));
}

#ifndef VS_2013
void J_Socket_Server::default_initialization(){
	M_launched_flag = false;
}
#endif //!VS_2013

void J_Socket_Server::queue_json_data(const json_val& i_json_val){
	auto val_string = i_json_val.to_string();

	M_pending_messages.emplace_back(string(val_string.begin(), val_string.end()));

	wcerr << L"\nJson Val: " << val_string;// M_pending_messages.back();

}

void J_Socket_Server::queue_string(const string& i_data){

	M_pending_messages.emplace_back(i_data);
#if SOCKET_PRINTING
	cerr << "\nQueued String: " << i_data;//M_pending_messages.back();
#endif

}

J_Socket_Server::~J_Socket_Server(){
	for (auto& connection : M_connections){
		M_server.close(connection, websocketpp::close::status::going_away
			, "J_Socket_Server Ending");
	}

}

void J_Socket_Server::send_data(){
	if (M_pending_messages.empty()){
		return;
	}

	for (auto& message : M_pending_messages){
		send_message(message);
	}
	M_pending_messages.clear();
}

void J_Socket_Server::send_message(const string& irk_message){
	for (auto& connection : M_connections){
		M_server.send(connection, irk_message, opcode::text);
	}
}

void J_Socket_Server::on_open(M_Connection_Cont_t::value_type i_connection){
	static int s_connection_count = 0;
	//J_LOCK(M_connection_cont_mutex);
	assert(!M_connections.count(i_connection));
	auto insert_results = M_connections.insert(i_connection);
	cerr << "\nConnection Number: " << ++s_connection_count;
	assert(insert_results.second);
	(void)insert_results;

	assert(!M_connection_names.count(i_connection));
	string endl_str("\n");
	auto connection = M_server.get_con_from_hdl(i_connection);
	string connection_name_str = connection->get_origin() + endl_str + connection->get_host()
		+ connection->get_resource() + endl_str + string("At port: ") + number_to_string(connection->get_port());
	
	M_connection_names[i_connection] = connection_name_str;
	
	for(auto& f_callback : M_connection_callbacks){
		f_callback(i_connection, connection_name_str, Connection_Alert::OPEN);
	}
}

void J_Socket_Server::on_message(M_Connection_Cont_t::value_type i_connection, Server_t::message_ptr i_msg){
	auto con = M_server.get_con_from_hdl(i_connection);

	cout << "\nMessage from connection: " << con << "\nRecieved message : " << i_msg->get_payload()
		<< endl;
}

void J_Socket_Server::on_close(M_Connection_Cont_t::value_type i_connection){
	//J_LOCK(M_connection_cont_mutex);
	auto iter_pos = M_connections.find(i_connection);

	assert(iter_pos != M_connections.end());

	M_connections.erase(iter_pos);
	auto name_pos = M_connection_names.find(i_connection);
	assert(name_pos != M_connection_names.end());

	for(auto& f_callback : M_connection_callbacks){
		f_callback(i_connection, string(), Connection_Alert::CLOSE);
	}

	M_connection_names.erase(name_pos);
}

void J_Socket_Server::launch_server(){
	if (M_launched_flag){
		cerr << "\nWarning server already launched";
		return;
	}
	
	M_server.listen(M_port_number);
#ifndef VS_2013
	VLDDisable();
#endif
	M_server.start_accept();
#ifndef VS_2013
	VLDEnable();
#endif
	M_launched_flag = true;
}

void J_Socket_Server::poll_events(){
	assert(M_launched_flag);
	M_server.poll();
}

void J_Socket_Server::close_server(){
	M_server.stop_listening();
	M_server.stop();

}

void J_Socket_Server::run_server(){
	M_server.listen(M_port_number);
	M_server.start_accept();
	try{
		M_server.run();
	}
	catch (J_Error& error){
		error.print();
		cout << "\nEnding Server";
	}
	
	
}

void J_Socket_Server::terminate(M_Connection_Cont_t::value_type){
	throw J_Error("End Server");
}

void J_Socket_Server::terminate_message(M_Connection_Cont_t::value_type, Server_t::message_ptr){
	throw J_Error("End Server");
}

void J_Socket_Server::close_connection(websocketpp::connection_hdl i_handle){
	
	M_server.close(i_handle, websocketpp::close::status::normal, "User Ended this Connection");

}

J_Socket_Server::connection_name_iter J_Socket_Server::connection_names_begin()const{
	return make_pair_iter(M_connection_names.begin());
}

J_Socket_Server::connection_name_iter J_Socket_Server::connection_names_end()const{
	return make_pair_iter(M_connection_names.end());
} 

void J_Socket_Server::add_open_close_connection_callback(
	const Connection_Callback_t& i_callback){
	M_connection_callbacks.push_back(i_callback);
}

}


#ifndef __WEB_SOCKET_SERVER_HPP
#define __WEB_SOCKET_SERVER_HPP

#include <rapidjson/document.h>

#include <gst/gst.h>
#include <gst/sdp/gstsdpmessage.h>


#include <boost/bind/bind.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;
typedef server::message_ptr message_ptr;



void test()
{
    
}
class WebSocketServer
{
public:
    boost::asio::io_service &_io_service;
    server web_socket_server;
public:
    WebSocketServer(boost::asio::io_service & io_service)
        : _io_service(io_service)
    {

    }
    ~WebSocketServer()
    {

    }
    void listen(uint16_t port)
    {
        web_socket_server.set_access_channels(websocketpp::log::alevel::all);
        web_socket_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

        web_socket_server.init_asio(&_io_service);
        
        web_socket_server.set_message_handler(boost::bind(&WebSocketServer::on_message, this, boost::placeholders::_1, boost::placeholders::_2));
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
        web_socket_server.set_reuse_addr(true);
        web_socket_server.listen(endpoint);
        web_socket_server.start_accept();

    }
    void on_message(websocketpp::connection_hdl hdl, message_ptr msg)
    {
        std::cout << "on_message called with hdl: " << hdl.lock().get()
                << " and message: " << msg->get_payload()
                << std::endl;
        
        // check for a special command to instruct the server to stop listening so
        // it can be cleanly exited.
        if (msg->get_payload() == "stop-listening") {
            web_socket_server.stop_listening();
            return;
        }
        rapidjson::Document document;
        
        document.Parse(msg->get_payload().c_str());

        if(!document.IsObject())
            return;
        if(!document.HasMember("command"))
            return;
        if(!document["command"].IsString())
            return;
        if(strcmp(document["command"].GetString(), "offer") == 0)
        {
            if(!document.HasMember("sdp"))
                return;
            if(!document["sdp"].IsString())
                return;
            
            std::cout << "sdp : " << document["sdp"].GetString() << std::endl;
            
            GstSDPMessage *message;
            glong length = -1;
            gst_sdp_message_new(&message);
            gst_sdp_message_parse_buffer((guint8 *)document["sdp"].GetString(), length, message);

            int i = 0;
            while(1)
            {
                const GstSDPMedia *media = gst_sdp_message_get_media(message, i);
                if(media == NULL || media->media == NULL) break;
                g_print("media : %s %s %s %d %d (%s %d)\n", 
                    media->media,
                    media->proto,
                    media->information,
                    media->num_ports,
                    media->port,
                    __func__, __LINE__);
                guint len = gst_sdp_media_attributes_len(media);
                g_print("sdp attributes len : %d\n", len);
                // int j = 0;
                // while(1)
                // {
                //     const GstSDPAttribute * attr = gst_sdp_media_get_attribute(media, j);
                //     if(attr == NULL || attr->key == NULL || attr->value == NULL) break;
                //     g_print("sdp attr key:%s, value:%s\n", attr->key, attr->value);

                //     j++;
                // }

                i++;

            }

        }
        

        // try {
        //     web_socket_server.send(hdl, msg->get_payload(), msg->get_opcode());
        // } catch (websocketpp::exception const & e) {
        //     std::cout << "Echo failed because: "
        //             << "(" << e.what() << ")" << std::endl;
        // }
    }


};

#endif
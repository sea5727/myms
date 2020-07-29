#ifndef __WEB_SOCKET_SERVER_HPP
#define __WEB_SOCKET_SERVER_HPP

#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
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
        std::cout << "on_message called with hdl: " << hdl.lock().get() << " opcode : " << msg->get_opcode() 
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

            
            guint len = gst_sdp_message_medias_len(message);
            for(int i = 0 ; i < len ; i++)
            {
                const GstSDPMedia *media = gst_sdp_message_get_media(message, i);
                g_print("media : %s %s %s %d %d (%s %d)\n", 
                    media->media,
                    media->proto,
                    media->information,
                    media->num_ports,
                    media->port,
                    __func__, __LINE__);
                guint media_len = gst_sdp_media_formats_len(media);
                for(int j = 0 ; j < media_len ; j++)
                {
                    const gchar* media_format = gst_sdp_media_get_format(media, j);
                    g_print("[%d] media format : %s\n", j, media_format);
                    GstCaps * media_caps = gst_sdp_media_get_caps_from_media(media, atoi(media_format));
                    g_print("caps : %s\n", gst_caps_to_string(media_caps));
                    guint media_caps_len = gst_caps_get_size(media_caps);
                    g_print("gst_caps_get_size : %d\n", media_caps_len);
                    for(int k = 0 ; k < media_caps_len ; k++)
                    {
                        GstStructure *media_caps_structure = gst_caps_get_structure(media_caps, k);
                        // g_print("%s\n", gst_structure_to_string(media_caps_structure));
                        // gst_structure_get_string(media_caps_structure)
                    }
                    

                }
            }

            const char * result_sdp = 
            "v=0\n"
            "o=MyStreamer 2398026505 2307593197 IN IP4 10.20.30.40\n"
            "s=MyStreamer Audio Session\n"
            "t=0 0\n"
            // "m=video 5000 UDP/TLS/RTP/SAVPF 96\n"
            "m=video 5000 RTP/AVP 96\n"
            "c=IN IP4 192.168.0.34\n"
            "a=fingerprint:sha-256 D4:73:0F:27:ED:14:D8:15:23:E5:04:9B:F8:00:A1:D2:7F:A4:6F:C8:75:75:10:E7:72:8C:29:CC:12:F6:A5:2B\n"
            "a=ice-ufrag:W2TGCZw2NZHuwlnf\n"
            "a=ice-pwd:xdQEccP40E+P0L5qTyzDgfmW\n"
            "a=rtcp-mux\n"
            "a=rtpmap:96 H264/90000\n";

            
            rapidjson::Document response_doc;
            response_doc.SetObject();
            rapidjson::Document::AllocatorType& allocator = response_doc.GetAllocator();


            rapidjson::Value sdp(rapidjson::kStringType);
            sdp.Set(result_sdp);

            response_doc.AddMember("command", "answer", allocator);
            response_doc.AddMember("sdp", sdp, allocator);

            rapidjson::StringBuffer buffer;
            buffer.Clear();
            rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
            response_doc.Accept(writer);


            web_socket_server.send(hdl, buffer.GetString(), websocketpp::frame::opcode::value::text);


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
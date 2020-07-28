#include <iostream>
#include <glib.h>
#include <gst/sdp/gstsdpmessage.h>



static int a;
struct handler_register_t {
  handler_register_t(int index) {
    a = index;
  }
};



static struct handler_init_t {
  handler_init_t() {
      a = 10;
  }
}_init;


static handler_register_t test(99);


handler_init_t teststruct();

static class TestClass
{
public:
    TestClass() {

    }
}testc;


/* *INDENT-OFF* */
static const gchar *sdp = "v=0\r\n"
    "o=- 123456 0 IN IP4 127.0.0.1\r\n"
    "s=TestSessionToCopy\r\n"
    "c=IN IP4 127.0.0.1\r\n"
    "t=0 0\r\n"

    "a=sendrecv\r\n"
    "m=video 3434 RTP/AVP 96 97 99\r\n"
    "a=rtpmap:96 MP4V-ES/90000\r\n"
    "a=rtpmap:97 H263-1998/90000\r\n"
    "a=rtpmap:99 H263/90000\r\n"

    "a=sendrecv\r\n"
    "m=video 6565 RTP/AVP 98\r\n"
    "a=rtpmap:98 VP8/90000\r\n"

    "a=sendrecv\r\n"
    "m=audio 4545 RTP/AVP 14\r\n"

    "a=sendrecv\r\n"
    "m=audio 1010 TCP 14\r\n";


static const gchar caps_video_string1[] =
    "application/x-unknown, media=(string)video, payload=(int)96, "
    "clock-rate=(int)90000, encoding-name=(string)MP4V-ES";

static const gchar caps_video_string2[] =
    "application/x-unknown, media=(string)video, payload=(int)97, "
    "clock-rate=(int)90000, encoding-name=(string)H263-1998";

static const gchar caps_audio_string[] =
    "application/x-unknown, media=(string)audio, payload=(int)14, "
    "clock-rate=(int)90000";
static const gchar mycap[] = 
"application/x-rtp, "
"media=(string)video, "
"clock-rate=(int)90000, "
"encoding-name=(string)H264, "
"packetization-mode=(string)1, "
"profile-level-id=(string)4d401f, "
"sprop-parameter-sets=(string)\"Z01AH9oBQBbsBEAAAAMAQAAADIPGDKg\\=\\,aO88gA\\=\\=\", "
"payload=(int)96, "
"ssrc=(uint)3312487744, "
"timestamp-offset=(uint)1700793591, "
"seqnum-offset=(uint)30165, "
"a-framerate=(string)25";
void test1()
{
    GValue value = G_VALUE_INIT;
    GValue value_copy = G_VALUE_INIT;

    GstSDPMessage *message, *copy;
    gchar *message1_str, *mesage2_str, *copy_str;

    const gchar *repeat1[] = { "789", "012", NULL};

    gst_sdp_message_new(&message);
    gst_sdp_message_parse_buffer((guint8 *)sdp, -1, message);

    gst_sdp_message_add_time(message, "123", "456", repeat1);

    g_value_init(&value, GST_TYPE_SDP_MESSAGE);
    g_value_init(&value_copy, GST_TYPE_SDP_MESSAGE);

    g_value_set_boxed(&value, message);
    message1_str = gst_sdp_message_as_text(message);
    g_print("message1_str : %s\n", message1_str);

}
void caps_from_media()
{
    GstSDPMessage *message;
    glong length = -1;
    const GstSDPMedia *media1, *media2, *media3;
    GstCaps *caps_video1, *caps_video2, *caps_audio;
    GstCaps *result_video1, *result_video2, *result_audio, *result_mycap;

    gst_sdp_message_new (&message);
    gst_sdp_message_parse_buffer ((guint8 *) sdp, length, message);

    media1 = gst_sdp_message_get_media (message, 0);
    g_print("media1 : %s %s %s %d %d (key type:%s, data:%s)(%s %d)\n",
        media1->media,
        media1->proto,
        media1->information,
        media1->num_ports,
        media1->port,
        media1->key.type,
        media1->key.data,
        __func__, __LINE__);
    // fail_unless (media1 != NULL);
    guint media1_len = gst_sdp_media_formats_len(media1);
    const gchar* m1_fmt0 = gst_sdp_media_get_format(media1, 0);
    const gchar* m1_fmt1 = gst_sdp_media_get_format(media1, 1);
    const gchar* m1_fmt2 = gst_sdp_media_get_format(media1, 2);
    g_print("media1 formats_len:%d, fmt0:%s, fmt1:%s, fmt2:%s\n", media1_len, m1_fmt0, m1_fmt1, m1_fmt2);
    
    GstCaps* m1_caps0 = gst_sdp_media_get_caps_from_media(media1, atoi(m1_fmt0));
    GstStructure *m1_caps0_st0 = gst_caps_get_structure(m1_caps0, 0);
    g_print("m1_caps0_st0 get media : %s\n", gst_structure_get_string(m1_caps0_st0, "media"));
    gint v1 = 0;
    gst_structure_get_int(m1_caps0_st0, "payload", &v1);
    g_print("m1_caps0_st0 get payload : %d\n", v1);
    g_print("m1_caps0_st0 : %s\n", gst_structure_to_string(m1_caps0_st0));



    gchar * m1_caps0_str = gst_caps_to_string(m1_caps0);
    guint m1_caps0_size = gst_caps_get_size(m1_caps0);
    g_print("media1 caps0 :%s, size;%d\n",m1_caps0_str , m1_caps0_size);

    
    GstCapsFeatures * m1_caps0_feat0 = gst_caps_get_features(m1_caps0, 0);
    g_print("m1_caps0_feat0 : %s\n", gst_caps_features_to_string(m1_caps0_feat0));



    media2 = gst_sdp_message_get_media (message, 1);
    g_print("media2 : %s %s %s %d %d (key type:%s, data:%s)(%s %d)\n",
        media2->media,
        media2->proto,
        media2->information,
        media2->num_ports,
        media2->port,
        media2->key.type,
        media2->key.data,
        __func__, __LINE__);
    // fail_unless (media2 != NULL);

    media3 = gst_sdp_message_get_media (message, 2);
    g_print("media3 : %s %s %s %d %d (key type:%s, data:%s)(%s %d)\n",
        media3->media,
        media3->proto,
        media3->information,
        media3->num_ports,
        media3->port,
        media3->key.type,
        media3->key.data,
        __func__, __LINE__);
    const GstSDPMedia *media4 = gst_sdp_message_get_media (message, 3);
    g_print("media4 : %s %s %s %d %d (key type:%s, data:%s)(%s %d)\n",
        media4->media,
        media4->proto,
        media4->information,
        media4->num_ports,
        media4->port,
        media4->key.type,
        media4->key.data,
        __func__, __LINE__);
    // fail_unless (media2 != NULL);
    g_print("(%s %d)\n", __func__, __LINE__);
    caps_video1 = gst_sdp_media_get_caps_from_media (media1, 96);
    caps_video2 = gst_sdp_media_get_caps_from_media (media1, 97);
    caps_audio = gst_sdp_media_get_caps_from_media (media3, 14);
    GstCaps *test_caps = gst_caps_new_simple ("video/x-raw",
        "format", G_TYPE_STRING, "I420",
        "framerate", GST_TYPE_FRACTION, 25, 1,
        "pixel-aspect-ratio", GST_TYPE_FRACTION, 1, 1,
        "width", G_TYPE_INT, 320,
        "height", G_TYPE_INT, 240,
        NULL);
    g_print("(%s %d)\n", __func__, __LINE__);    
    g_print("gst_caps_to_string...%s\n", gst_caps_to_string(test_caps));
    g_print("(%s %d)\n", __func__, __LINE__);
    GstCaps *videosinkcaps0 = gst_caps_from_string ("foo/bar");
    g_print("(%s %d)\n", __func__, __LINE__);
    GstCaps *videosinkcaps01 = gst_caps_from_string ("audio/mpeg,mpegversion=4");
    
    g_print("(%s %d)\n", __func__, __LINE__);
    GstCaps *videosinkcaps1 = gst_caps_from_string ("video/x-raw,width=320,height=240,framerate=10/1");
    g_print("(%s %d)\n", __func__, __LINE__);
    GstCaps *videosinkcaps2 = gst_caps_from_string ("video/x-raw-rgb, bpp=(int)24, depth=(int)24");
    g_print("(%s %d)\n", __func__, __LINE__);
    result_mycap = gst_caps_from_string (mycap);
    g_print("(%s %d)\n", __func__, __LINE__);
    result_video1 = gst_caps_from_string (caps_video_string1);
    g_print("(%s %d)\n", __func__, __LINE__);
    if(gst_caps_is_strictly_equal (caps_video1, result_video1))
    {
        g_print("gst_caps_is_strictly_equal is true\n");
    }
    gst_caps_unref (result_video1);
    gst_caps_unref (caps_video1);

    result_video2 = gst_caps_from_string (caps_video_string2);
    g_print("(%s %d)\n", __func__, __LINE__);
    if(gst_caps_is_strictly_equal (caps_video2, result_video2))
    {
        g_print("gst_caps_is_strictly_equal is true\n");
    }
    gst_caps_unref (result_video2);
    gst_caps_unref (caps_video2);

    g_print("(%s %d)\n", __func__, __LINE__);
    result_audio = gst_caps_from_string (caps_audio_string);
    if(gst_caps_is_strictly_equal (caps_audio, result_audio))
    {
        g_print("gst_caps_is_strictly_equal is true\n");
    }
    gst_caps_unref (result_audio);
    gst_caps_unref (caps_audio);

    gst_sdp_message_free (message);
}
int main(int argc, char* argv[]) {
    _init;
    std::cout << "a : " << a << std::endl;
    return 0;
    std::cout << "Hello, world!\n";
    // test1();
    gst_init(&argc, &argv);
    caps_from_media();
}

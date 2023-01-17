#ifndef __ZFTP_SERVER_MESSAGEHANDLE
#define __ZFTP_SERVER_MESSAGEHANDLE
#include <map>
#include "zlk_messagehandle.h"
class zftp_server_handleMessage : public zlk_messagehandle
{
public:
    zftp_server_handleMessage();
    void hand_ls(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_rm(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_pull(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_push(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);

protected:
};

#endif
#ifndef __ZFTP_CLIENT_MESSAGEHANDLE
#define __ZFTP_CLIENT_MESSAGEHANDLE
#include <map>
#include "zlk_messagehandle.h"
class zftp_client_handleMessage : public zlk_messagehandle
{
public:
    zftp_client_handleMessage();
    void hand_ls(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_rm(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_pull(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_push(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);

    void hand_pwd(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_mkdir(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);
    void hand_cd(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res);

protected:
};

#endif
#ifndef __ZFTP_MESSAGEHANDLE
#define __ZFTP_MESSAGEHANDLE
#include <map>
#include "zlk_messagehandle.h"
class zftp_handleMessage : public zlk_messagehandle
{
public:
    zftp_handleMessage();
    void hand_ls(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res);
    void hand_rm(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res);
    void hand_pull(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res);
    void hand_push(const zftp_message::Pakcet &packet, zftp_message::response_msg &zlk_res);

protected:
};




#endif
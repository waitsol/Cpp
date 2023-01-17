#include "zftp_client_handmessage.h"
#include "zlk_log.h"
zftp_client_handleMessage::zftp_client_handleMessage()
{
    m_mapFunc[zlkMsg::rm] = std::bind(&zftp_client_handleMessage::hand_rm, this, std::placeholders::_1, std::placeholders::_2);
    m_mapFunc[zlkMsg::ls] = std::bind(&zftp_client_handleMessage::hand_ls, this, std::placeholders::_1, std::placeholders::_2);
};
void zftp_client_handleMessage::hand_ls(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
    zftp_message::ls_response res;
    if (res.ParseFromString(packet.msgbody()))
    {
        for (int i = 0; i < res.list_size(); i++)
        {
            DBG("res %d %s", res.list(i).id(), res.list(i).name().data());
        }
    }
    else
    {
        ERR("ParseFromString error");
    }
};
void zftp_client_handleMessage::hand_rm(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
}
void zftp_client_handleMessage::hand_pull(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
}
void zftp_client_handleMessage::hand_push(const zftp_message::Pakcet &packet, zftp_message::Pakcet &zlk_res)
{
}